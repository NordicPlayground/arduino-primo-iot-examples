# Arduino Primo IoT Examples
Extension to nRF5 IOT SDK with examples for Arduino Primo

## Prerequisites
* Router with [bluetooth_6lowpand](https://github.com/NordicSemiconductor/Linux-ble-6lowpan-joiner) installed and running in Authentication Mode.
* Android Device to install Configuration tool for the kits.
* 2 pcs of Arduino Primos.

## The CoAP Server Example

![ArduinoPrimoSetup](/images/arduino_primo_setup.png)

The CoAP server has LED2 representing a Boolean state. If the CoAP client toggles a button press, a coap PUT message will be sent 
to the server and LED2 on the remote Arduino Primo board will toggle.

## The CoAP Client Example
The CoAP client button (BUTTON1) is used to trigger the client to send a CoAP message over IPv6 to the CoAP server in order to change the LED state.

The server address is automatically resolved by using the IPv6 prefix of its own address, combining it 
with the servers hardcoded EUI48 address. The client cannot connect to any other server than the one provided 
in this example bundle. 
    
## Install Android application, SDK and Arduino Primo examples add-on component

1. Download the nrf5_iot_sdk zip file from [here](http://developer.nordicsemi.com/nRF5_IoT_SDK/nRF5_IoT_SDK_v0.9.x/)

2. Unzip the SDK. The targer folder chosen when extracting the zip file will from now on be called "\<sdk-root\>".
    
3. Install flash programming toolchain
  * If you want to use __JLink__ and __nrfjprog__ to flash, download and install the programming tools:
    1. Navigate to [http://www.nordicsemi.com/eng/Products/Bluetooth-Smart-Bluetooth-low-energy/nRF52832](http://www.nordicsemi.com/eng/Products/Bluetooth-Smart-Bluetooth-low-energy/nRF52832) in a browser and select the “Downloads” tab.
    2. Download the “nRF5x-Command-Line-Tools” for your architecture.
    3. Install the tools.

  * If you want to use __OpenOCD__ to flash, download the OpenOCD application supporting nrf52 for your OS:
    * [OpenOCD for OSX](http://download.arduino.org/tools/openocd-primo/OpenOCD-0.9.0-arduino.org-apple-darwin13.4.0-nrf52.tar.bz2)
    * [OpenOCD for Linux 32bit](http://download.arduino.org/tools/openocd-primo/OpenOCD-0.9.0-arduino.org-i686-linux-gnu-nrf52.tar.bz2)
    * [OpenOCD for Linux 64bit](http://download.arduino.org/tools/openocd-primo/OpenOCD-0.9.0-arduino.org-x86_64-linux-gnu-nrf52.tar.bz2)
    * [OpenOCD for Windows](http://download.arduino.org/tools/openocd-primo/OpenOCD-0.9.0-arduino.org-win32-nrf52.tar.bz2)

4. Do a git clone of the Arduino Primo SDK add-on:
    * Navigate to __\<sdk-root\>/examples/__ in your extracted SDK and perform a git clone of the add-on by issuing the following command:
   ```
   git clone https://github.com/NordicSemiconductor/arduino-primo-iot-examples.git arduino_primo 
   ```
    
5. Install "nRF BLE Joiner" on you Android device from [https://play.google.com/store/apps/details?id=no.nordicsemi.android.nrfblejoiner](https://play.google.com/store/apps/details?id=no.nordicsemi.android.nrfblejoiner)

## Flash the CoAP Server using JLink programmer

1. Connect the JLink Programmer to the 10-pin header close to the nRF52 chip as seen in the following image:

   ![JLinkCableConnect](/images/jlink_cable_connect.png)
   
2. Connect the device to a USB power source.
3. To flash the BLE SoftDevice onto the Arduino Primo board, run the following commands:
   ```
   nrfjprog --eraseall -f nrf52
   nrfjprog --program <sdk-root>\components\softdevice\s1xx_iot\s1xx-iot-prototype3_nrf52_softdevice.hex -f nrf52
   ```
   
4. To flash the CoAP server onto the Arduino Primo board, run the following command:
   ```
   nrfjprog --program <sdk-root>\examples\arduino_primo\coap\ipv6\server\hex\nrf52832_xxaa_s1xx_iot.hex -f nrf52
   ```
    
5. To reset the Arduino Primo board, run the following command:
   ```
   nrfjprog --reset -f nrf52
   ```
   
6. Configure the Arduino Primo board with commissioning credentials using the "nRF BLE Joiner" application as described [here](#configure-commissioning-details-on-the-kits).

## Flash the CoAP Client using JLink programmer

The procedure for flashing the client is otherwise the same as for the server, except for the firmware used in step 4. The firmware used for the CoAP client is located at **<sdk-root>\examples\arduino_primo\coap\ipv6\client\hex\nrf52832_xxaa_s1xx_iot.hex**.

## Flash the CoAP Server using OpenOCD

1. Connect a USB cable from your OpenOCD enabled PC to the Arduino Primo board as seen in the following image:

   ![USBcableConnected](/images/usb_cable_connect.png)
   
2. To flash the BLE SoftDevice onto the Arduino Primo board, run the following commands:
   ```
   <openocd-root-dir>\bin\openocd -s <openocd-root-dir>\share\openocd\scripts -f board\arduino_primo.cfg -c "program <sdk-root>\components\softdevice\s1xx_iot\s1xx-iot-prototype3_nrf52_softdevice.hex verify reset exit"
   ```
   
3. To flash the CoAP server onto the Arduino Primo board, run the following command:
   ```
   <openocd-root-dir>\bin\openocd -s <openocd-root-dir>\share\openocd\scripts -f board\arduino_primo.cfg -c "program <sdk-root>\examples\arduino_primo\coap\ipv6\server\hex\nrf52832_xxaa_s1xx_iot.hex verify reset exit"
   ```
    
4. To reset the Arduino Primo board, unplug and replug the USB cable.

5. Configure the Arduino Primo board with commissioning credentials using the "nRF BLE Joiner" application as described [here](#configure-commissioning-details-on-the-kits).

## Flash the CoAP Client using OpenOCD

The procedure for flashing the client is otherwise the same as for the server, except for the firmware used in step 3. The firmware used for the CoAP client is located at **<sdk-root>\examples\arduino_primo\coap\ipv6\client\hex\nrf52832_xxaa_s1xx_iot.hex**.

## Configure Commissioning details on the kits

An un-configured kit (server or client) will always start without any commissioning details. 
If there is any need for resetting the kit to default state, power cycle the kit and press BUTTON1 before it 
establishes a BLE connection. Then power cycle the kit again. 

Then follow the [Configure Node for Joining](http://developer.nordicsemi.com/nRF5_IoT_SDK/doc/0.9.0/html/a00079.html) tutorial to configure the node. 

__Note: The device MUST be configured using the "nRF BLE Joiner" application to enable it to connect to the router.__

###### Notes to the tutorial

* To make your device to start Joining by default without any interaction from the "nRF BLE Joiner" application after the first configuration:
    * Select the kit to configure.
    * Press the “Edit default configuration”. 
      
      This allows you to configure which mode to start from after a reset. 
    * Select "No Change" when configuring the "Select a backup mode in case of action failure" option.
    
      This will make it go to Joining mode after any reset (reset is a failure).

* If you select “Identify” action in the "nRF BLE Joiner" application, LED2 will blink by default for approx. 20 seconds in order to show you which Arduino Primo board you are going to configure.

## LEDs

The following LEDs are used by the applications:

| Application LED | Arduino Primo LED   |
| --------------- | ------------------- |
| LED1            | L13                 |
| LED2            | USER2               |

The position of the LEDs is shown in the following image:

![Leds](/images/leds.png)

__LED1__ (yellow) shows the connection state of the application. If LED1 on the Arduino Primo board is blinking, it means 
that it is advertising. If LED1 is steady-on, it means that it has a BLE connection.

__LED2__ (blue) is application-specific. If "Identify" mode is issued from the Android application during configuration, the LED2 will be blinking. In connection the server example is the only one using LED2 to represent toggle requests from the client application.

## Button

The following button is used by the clients:

| Application Button | Arduino Primo Button |
| ------------------ | -------------------- |
| BUTTON1            | USER2                |

The position of BUTTON1 is shown in the following image:

![Button](/images/button.png)

BUTTON1 is application-specific. Before any kind of BLE connection has been established, this button is used for 
erasing the commissioning details stored in flash on the Arduino Primo board. When the Arduino Primo board enters a BLE connection state, the button 
cannot be used anymore to erase the commissioning details. In connection the CoAP client is the only one using this button 
in order to trigger an IPv6 CoAP message to the server.

__Note: Commissioning credentials cannot be erased while in a connection.__
