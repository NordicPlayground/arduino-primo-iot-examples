# Arduino Primo IoT Examples
Extension to nRF5 IOT SDK with examples for Arduino Primo

## Prerequisites
* Router with [bluetooth_6lowpand](https://github.com/NordicSemiconductor/Linux-ble-6lowpan-joiner) installed and running in Authentication Mode.
* Android Device to install Configuration tool for the kits.
* 2 pcs of Arduino Primos.
    
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
    
4. To reset the kit unplug and replug the USB cable.

## Flash the CoAP Client using OpenOCD

The procedure for flashing the client is otherwise the same as for the server, except for the firmware used in step 3. The firmware used for the CoAP client is located at **<sdk-root>\examples\arduino_primo\coap\ipv6\client\hex\nrf52832_xxaa_s1xx_iot.hex**.

## LEDs

![Leds](/images/leds.png)

The two following LEDs are used by the applications:

LED1 = L13

LED2 = USER2

The position of the LEDs is shown in the figure above.

LED1 (yellow) has the purpose of showing connection state of the application. If the LED is blinking it means 
that it is advertising. If the LED is steady on, it means that it has a BLE connection.

LED2 (blue) is application specific. During configuration the LED will be blinking if “Identify” mode is issued from the Android application. 
In connection the server example is the only one using the LED, to represent toggle requests from the client.

## Button

![Button](/images/button.png)

The following button is used by the clients:

BUTTON1 = USER2

The position of the button is shown in the image above.

BUTTON1 is application specific. Before any kind of BLE connection has been established, this button is used for 
erasing the commissioning details stored in flash on the kit. When the kit enters a BLE connection state the button 
cannot be used to erase the commissioning details anymore. In connection the client is the only one using this button 
in order to trigger an IPv6 CoAP message to the server.

## Configure Commissioning details on the kits

An un-configured kit (server or client) will always start without any commissioning details. 
If there is any need for resetting the kit to default state, power cycle the kit and press BUTTON1 before it 
establishes a BLE connection. Then power cycle the kit again. 

Then follow the tutorial on how to configure the node using the 
[Configure Node for Joining](http://developer.nordicsemi.com/nRF5_IoT_SDK/doc/0.9.0/html/a00079.html) tutorial.

###### Notes to the tutorial

1. If you want your device to start Joining by default, without any interaction from the phone after the first configuration. Do the following steps:
    * When you have selected the kit to configure, press the “Edit default configuration”. This makes you able to configure which mode to start from after a reset. 
    * Make sure that you select backup mode “No change” when configuring “Connection Mode”. This will make it go to Joining mode after any reset (reset is a failure).
2. If you select “Identify” in the Android application, LED2 will blink (blue) by default for approx. 20 seconds in order to show you which kit you are going to configure.

## The CoAP Server Example

![ArduinoPrimoSetup](/images/arduino_primo_setup.png)

The server has a led representing a Boolean state (LED2). If the client toggles a button press, a coap PUT message will be sent 
to the server and the led on the remote kit will toggle.

## The CoAP Client Example
The clients button (BUTTON1) is used to trigger the client to send a CoAP message over IPv6 to the server in order to change the LED state.

The server address is automatically resolved by using the IPv6 prefix of its own address, combining it 
with a hardcoded EUI48 address of the server. The client cannot connect to any other server than the one provided 
in this example bundle. 
