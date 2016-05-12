# Arduino Primo IoT Examples
Extension to nRF5 IOT SDK with examples for Arduino Primo

## Prerequisites
    * Router with [bluetooth_6lowpand](https://github.com/NordicSemiconductor/Linux-ble-6lowpan-joiner) installed and running in Authentication Mode.
    * Android Device to install Configuration tool for the kits.
    * 2 Arduino Primo.
    
## Install Android application, SDK and Arduino Primo examples add-on component

1. Download and install nrf5_iot_sdk zip file.
    * Download the zip from [here](http://developer.nordicsemi.com/nRF5_IoT_SDK/nRF5_IoT_SDK_v0.9.x/)
    * Unzip the SDK. The folder chosen when extracting will from now on be called "<sdk-root>".
    
2. If you would like to use Jlink and nrfjprog to flash, install the programming tools.
    * Navigate to [http://www.nordicsemi.com/eng/Products/Bluetooth-Smart-Bluetooth-low-energy/nRF52832](http://www.nordicsemi.com/eng/Products/Bluetooth-Smart-Bluetooth-low-energy/nRF52832) 
    in a browser and select the “Downloads” tab.
    * Download the “nRF5x-Command-Line-Tools” for your architecture.
    * Install the tools.
    
3. Clone the Arduino Primo SDK add-on. 
    * Navigate to <sdk-root>/examples/
    * Perform a git clone of the add-on by issuing the following command:
    ```
    git clone https://github.com/NordicSemiconductor/arduino-primo-iot-examples.git arduino_primo 
    ```
    
4. Install "nRF BLE Joiner" on you Android device.
    * [https://play.google.com/store/apps/details?id=no.nordicsemi.android.nrfblejoiner](https://play.google.com/store/apps/details?id=no.nordicsemi.android.nrfblejoiner)

## Flash the CoAP Server

1. Connect the Jlink Programmer to 10-pin header close to the nrf52 chip. As seen in the image below:

   ![JLinkCableConnect](/images/jlink_cable_connect.png)
   
2. Power the kit through the USB.
3. Run the following command to flash the BLE softdevice onto the kit:
    ```
    nrfjprog --eraseall -f nrf52
    nrfjprog --program <sdk-root>\components\softdevice\s1xx_iot\s1xx-iot-prototype3_nrf52_softdevice.hex -f nrf52
    ```
4. Run the following command to flash the server onto the kit:
    ```
    nrfjprog --program <sdk-root>\examples\arduino_primo\coap\ipv6\server\hex\nrf52832_xxaa_s1xx_iot.hex -f nrf52
    ```
    
5. Reset the kit by either unplug/plug the USB cable, or run the following command:
    ```
    nrfjprog --reset
    ```

## Flash the CoAP Client

The procedure for flashing the client is the exact same as for the server as described above. 
However, use the firmware located at **<sdk-root>\examples\arduino_primo\coap\ipv6\client\hex\nrf52832_xxaa_s1xx_iot.hex** when performing step 4.

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
