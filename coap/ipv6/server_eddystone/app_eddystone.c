 /* Copyright (c) 2016 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */
#include "app_eddystone.h"

#define RSSI_POS   1
#define PREFIX_POS 2
#define URL_POS    3

#define APP_CFG_NON_CONN_ADV_TIMEOUT    0                                                             /**< Time for which the device must be advertising in non-connectable mode (in seconds). 0 disables the time-out. */

#define EDDYSTONE_UUID               0xFEAA                                                           /**< UUID for Eddystone beacons according to specification. */
#define EDDYSTONE_FRAME_MAX_SIZE     20                                                               /**< Maximum size of the frame. */
#define EDDYSTONE_URL_FRAME_TYPE     0x10                                                             /**< URL Frame type is fixed at 0x10. */
#define DOMAIN_CODE_NUMBER           14                                                               /**< Number of top level domain codes. */
#define LEGAL_ASCII_START            32                                                               /**< First legal ASCII character that can be used in the URL. */
#define LEGAL_ASCII_STOP             126                                                              /**< Last legal ASCII character that can be used in the URL. */
#define RSSI_MAX                     20                                                               /**< Maximum TX power in dBm. */
#define RSSI_MIN                     -100                                                             /**< Minimum TX power in dBm. */

#define DEFAULT_RSSI                 -18                                                              /**< Default TX power in dBm. */
#define DEFAULT_URL_SCHEME           APP_EDDYSTONE_PREFIX_HTTPS                                       /**< Default prefix used. */


#define NON_CONNECTABLE_ADV_INTERVAL MSEC_TO_UNITS(100, UNIT_0_625_MS)                               /**< The advertising interval for non-connectable advertisement (100 ms). This value can vary between 100 ms and 10.24 s.*/

static const char                   m_default_url[]         = {'n','o','d','e','.','c','f','g'};     /**< Maximum 17 bytes. */
static ipv6_medium_ble_adv_params_t m_eddystone_adv_params;                                          /**< GAP parameters in Eddystone mode. */
static ble_uuid_t                   m_adv_uuids[]           = {{EDDYSTONE_UUID, BLE_UUID_TYPE_BLE}}; /**< Eddystone UUID. */
static ble_advdata_service_data_t   m_service_data;
static bool                         m_initialized = false;
static uint8_t                      m_eddystone_url_data[EDDYSTONE_FRAME_MAX_SIZE] =                 /**< Information advertised by the Eddystone URL frame type. */
{
    EDDYSTONE_URL_FRAME_TYPE   // Eddystone URL frame type.
};


/**
 * @brief URL validation
 * @note Specifications used: https://github.com/google/eddystone/tree/master/eddystone-url 
 */
static uint32_t validate_url(eddystone_url_settings_t * p_settings)
{ 
    if (p_settings->p_url == NULL)
    {
        return NRF_ERROR_NULL;
    }
    if (p_settings->prefix > APP_EDDYSTONE_PREFIX_HTTPS)
    {
        return NRF_ERROR_INVALID_PARAM;
    }
    
    // Check valid RSSI value.
    if (p_settings->rssi > RSSI_MAX || p_settings->rssi < RSSI_MIN)
    {
        return NRF_ERROR_INVALID_PARAM;
    }
        
    // Check if the URL can fit into frame.
    if (p_settings->size + URL_POS > EDDYSTONE_FRAME_MAX_SIZE || p_settings->size == 0)
    {
        return NRF_ERROR_DATA_SIZE;
    }
    
    uint32_t i; 
    bool one_domain = false;
    for (i = 0; i < p_settings->size; i++)
    {
        // More than one top level domain in the URL.
        if (p_settings->p_url[i] < DOMAIN_CODE_NUMBER)
        {
            if (one_domain == true)
            {
                return NRF_ERROR_INVALID_DATA;
            }
            one_domain = true;
        }
        
        // Use of undefined codes in the URL.
        if ((p_settings->p_url[i] >= DOMAIN_CODE_NUMBER
             && p_settings->p_url[i] < LEGAL_ASCII_START)
            || (p_settings->p_url[i] > LEGAL_ASCII_STOP))
        {
            
            return NRF_ERROR_INVALID_DATA;
        }
    }
    
    return NRF_SUCCESS;
}


uint32_t app_eddystone_update_url(eddystone_url_settings_t * p_settings)
{
    uint32_t err_code;
    eddystone_url_settings_t settings;
    
    if (p_settings == NULL)
    {
        settings.rssi   = DEFAULT_RSSI;
        settings.prefix = DEFAULT_URL_SCHEME;
        settings.p_url  = (char*)m_default_url;
        settings.size   = sizeof(m_default_url);
        
        p_settings      = &settings;
    }
    
    if (m_initialized == false)
    {
        return NRF_ERROR_INVALID_STATE;
    } 
    
    err_code = validate_url(p_settings);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    
    m_service_data.data.size         = p_settings->size + URL_POS;

    m_eddystone_url_data[RSSI_POS]   = p_settings->rssi;
    m_eddystone_url_data[PREFIX_POS] = p_settings->prefix;
    memcpy(m_eddystone_url_data + URL_POS, p_settings->p_url, p_settings->size);
    
    return NRF_SUCCESS;
}


uint32_t app_eddystone_init(eddystone_url_settings_t * p_eddystone_url)
{
    uint32_t err_code;
    uint8_t  flags              = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;

    m_service_data.service_uuid = EDDYSTONE_UUID;         
    m_service_data.data.p_data  = (uint8_t *) m_eddystone_url_data;
    m_service_data.data.size    = 0;

    // Build and set advertising data.
    memset(&m_eddystone_adv_params, 0, sizeof(m_eddystone_adv_params));

    m_eddystone_adv_params.advdata.name_type               = BLE_ADVDATA_NO_NAME;
    m_eddystone_adv_params.advdata.flags                   = flags;
    m_eddystone_adv_params.advdata.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    m_eddystone_adv_params.advdata.uuids_complete.p_uuids  = m_adv_uuids;
    m_eddystone_adv_params.advdata.p_service_data_array    = &m_service_data;
    m_eddystone_adv_params.advdata.service_data_count      = 1;

    m_eddystone_adv_params.advparams.type                  = BLE_GAP_ADV_TYPE_ADV_NONCONN_IND;
    m_eddystone_adv_params.advparams.p_peer_addr           = NULL;
    m_eddystone_adv_params.advparams.fp                    = BLE_GAP_ADV_FP_ANY;
    m_eddystone_adv_params.advparams.interval              = NON_CONNECTABLE_ADV_INTERVAL;
    m_eddystone_adv_params.advparams.timeout               = APP_CFG_NON_CONN_ADV_TIMEOUT;
    
    m_initialized = true;
        
    err_code = app_eddystone_update_url(p_eddystone_url);

    return err_code;
}


uint32_t app_eddystone_adv_params_get(ipv6_medium_ble_adv_params_t ** pp_node_adv_params)
{
    if (pp_node_adv_params == NULL)
    {
        return NRF_ERROR_NULL;
    }
    if (m_initialized == false)
    {
        return NRF_ERROR_INVALID_STATE;
    }
    else
    {
        *pp_node_adv_params = &m_eddystone_adv_params;
        return NRF_SUCCESS;
    }
}
