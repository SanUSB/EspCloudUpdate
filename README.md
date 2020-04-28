# EspCloudUpdate [![N|Solid](http://sanusb.blogspot.com.br/favicon.ico)](http://sanusb.org/)

Let's see how it works?

ESP32 and ESP266 microcontrollers Update Environment for Internet programming with same generic sketch using the website sanusb.org/espupdate. In this project, the free version of Firebase (Google JSON objects Database) was implemented to generate the update trigger for .bin files. 
To perform this firmware update in the cloud, the users need to write in the sketch only the Wifi ssid, the password and the same name 
as the profile entered on the site and when uploading the .bin to site, remote firmware update of ESP microcontrollers via the Internet takes place. The user profile name entered on the website sanusb.org/espupdate may be alphanumeric (for example: sandro190575 or sandrojuca).

// It is worth considering that, through tests performed, files compiled .bin as with the same name for ESP to update
// with sequential downloads in the cloud, it may happen that the .bin file sent last for the update is not downloaded by ESP,
// but a file. previous bin, also due to network latency, because they have the same name and the same download URL.
// The name of the .bin files submitted on the site are versioned (modified), hence the download URL address as well,
// this prevents a previously uploaded file or another file with the same name from being downloaded, generating a unique and exclusive URL.
// In this case, to version the name and download URL of the .bin file, it was used: year, month, day, time and order cyclic upload.

Download and install the sample library .zip available on this link https://github.com/SanUSB/EspCloudUpdate To install the library, follow the steps: Arduino IDE -> Sketch -> Include Library -> Add .zip Libraries. Unzip the .zip folder and open the example CloudUpdateStart.ino. Write the name of your profile and WiFi network in the sketch. Update the sketch only the first time using the USB port. So it is now possible to transfer over the internet generating the .bin file.  For this, go to Sketch menu of the Arduino IDE > Export compiled Binary.

EspCloudUpdate provides:

  - This environment also makes it possible to update the firmware *.bin* via WiFi OTA (*Over the Air*). To generate a .bin file from your sketch, go to Sketch menu of the Arduino IDE > Export compiled Binary.

 To install the ESP32 and ESP8266 boards in Arduino IDE, follow these next instructions:
 In your Arduino IDE, go to File> Preferences. Insert into the "Additional Board Manager URLs":
 * https://dl.espressif.com/dl/package_esp32_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json
 
 Open the Boards Manager: Go to Tools > Board > Boards Manager:
 Search for ESP32 and press install button for the "ESP32 by Espressif Systems". Also search for ESP8266 and press install button for the "ESP8266 by ESP8266 Community".
 
 The dependencies of the EspCloudUpdate tool are the Firebase libraries. For ESP8266:
 
 * https://github.com/mobizt/Firebase-ESP8266
  
 For ESP32:
 
 * https://github.com/mobizt/Firebase-ESP32
 
 To install all the libraries, including https://github.com/SanUSB/EspCloudUpdate , follow the steps: 
Arduino IDE -> Sketch -> Include Library -> Add .zip Libraries.
 
 You can usually find the installed ESP32 and ESP8266 libraries at:
  
 On Windows:    
*        C:\Users\UserName\Documents\Arduino\libraries (tested)
     
 On Linux:   
*       /home/UserName/Arduino/Libraries (tested).

On OSX:
*       ~/Documents/Arduino/libraries.

 In the installed **EspCloudUpdate** library folder there is an example for testing this proposed tool called **EspUpdateStart.ino**.
 
 Other possible applications: post data to an IoT cloud server, modify operating parameters of the ESP microcontroller in real time, such as duty cycle PWM, online processing and calculators, etc..
 
*Have fun!*
