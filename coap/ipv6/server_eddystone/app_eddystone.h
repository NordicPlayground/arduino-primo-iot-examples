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
 
 /** @file
 *
 * @defgroup app_eddystone Eddystone beacons.
 * @{
 * @ingroup app_eddystone
 *
 * @brief   Eddystone URL beacon advertisement.
 *
 * @details This module enables the use of Eddystone beacons within commissioning.
 *          The URL can be no longer than 17 bytes, but prefix and top level
 *          domains have a dedicated one-byte encoding.
 *          
 * @note    For this module to work, the COMMISSIONING_ENABLED and EDDYSTONE_ENABLED flags must be set.
 */
 
 
#ifndef APP_EDDYSTONE_H__
#define APP_EDDYSTONE_H__

#include <stdbool.h>
#include <stdint.h>
#include "ipv6_medium_ble.h"


#define APP_EDDYSTONE_COM_SLASH  '\x00'
#define APP_EDDYSTONE_ORG_SLASH  '\x01'
#define APP_EDDYSTONE_EDU_SLASH  '\x02'
#define APP_EDDYSTONE_NET_SLASH  '\x03'
#define APP_EDDYSTONE_INFO_SLASH '\x04'
#define APP_EDDYSTONE_BIZ_SLASH  '\x05'
#define APP_EDDYSTONE_GOV_SLASH  '\x06'
#define APP_EDDYSTONE_COM        '\x07'
#define APP_EDDYSTONE_ORG        '\x08'
#define APP_EDDYSTONE_EDU        '\x09'
#define APP_EDDYSTONE_NET        '\x0a'
#define APP_EDDYSTONE_INFO       '\x0b'
#define APP_EDDYSTONE_BIZ        '\x0c'
#define APP_EDDYSTONE_GOV        '\x0d'
//14..32 	0x0e..0x20 	Reserved for future use
//127..255 	0x7f..0xff 	Reserved for future use

#define APP_EDDYSTONE_PREFIX_HTTP_WWW  0
#define APP_EDDYSTONE_PREFIX_HTTPS_WWW 1
#define APP_EDDYSTONE_PREFIX_HTTP      2
#define APP_EDDYSTONE_PREFIX_HTTPS     3

/**@brief Structure for storing settings necessary for broadcasting Eddystone URLs. */
typedef struct
{
    int8_t  rssi;   /**< Approximate signal strength in dBm. */
    uint8_t prefix; /**< Prefix used in the URL. For example: http:// */
    char *  p_url;  /**< Char array containing the rest of the URL. Use [Eddystone-URL HTTP URL encoding] (https://github.com/google/eddystone/tree/master/eddystone-url#eddystone-url-http-url-encodingspecial) for the domain. */
    uint8_t size;   /**< Size of the array stored in p_url. */
} eddystone_url_settings_t;


/**
 * @brief Function for initializing Eddystone beacon broadcasting.
 *
 * @param[in] p_settings Pointer to the URL broadcasting data.
 *
 * @retval NRF_SUCCESS             If the operation was successful.
 * @retval NRF_ERROR_DATA_SIZE     If the operation failed because not all the requested data could fit into the
 *                                 Eddystone beacon. The maximum size of the Eddystone beacon
 *                                 is @ref APP_EDDYSTONE_FRAME_MAX_SIZE.
 * @retval NRF_ERROR_INVALID_DATA  If the URL contains more than one top level domain or
 *                                 unsupported characters.
 * @retval NRF_ERROR_INVALID_PARAM If the RSSI value is outside the bounds of the specification or the prefix is undefined.
 * @retval NRF_ERROR_NULL          If the p_settings.p_url pointer is NULL.
 */
uint32_t app_eddystone_init(eddystone_url_settings_t * p_settings);


/**
 * @brief Function for updating the URL that is being broadcasted.
 *
 * @param[in] p_settings Pointer to the URL broadcasting data.
 * 
 * @retval NRF_SUCCESS             If the operation was successful.
 * @retval NRF_ERROR_DATA_SIZE     If the operation failed because not all the requested data could fit into the
 *                                 Eddystone beacon. The maximum size of the Eddystone beacon
 *                                 is @ref APP_EDDYSTONE_FRAME_MAX_SIZE.
 * @retval NRF_ERROR_INVALID_DATA  If the URL contains more than one top level domain or
 *                                 unsupported characters.
 * @retval NRF_ERROR_INVALID_STATE If the URL is updated before app_eddystone_init was called.
 * @retval NRF_ERROR_INVALID_PARAM If the RSSI value is outside the bounds of the specification or the prefix is undefined.
 * @retval NRF_ERROR_NULL          If the p_settings.p_url pointer is NULL.
 */
uint32_t app_eddystone_update_url(eddystone_url_settings_t * p_settings);


/**
 * @brief Function for getting the advertising parameters.
 *
 * @param[in] pp_node_adv_params Pointer to pointer of node advertisement parameters.
 *
 * @retval NRF_SUCCESS             If the operation was successful.
 * @retval NRF_ERROR_INVALID_STATE If the Eddystone module is not initialized.
 * @retval NRF_ERROR_NULL          If pp_node_adv_params is NULL.
 */
uint32_t app_eddystone_adv_params_get(ipv6_medium_ble_adv_params_t ** pp_node_adv_params);

#endif	// APP_EDDYSTONE_H__
