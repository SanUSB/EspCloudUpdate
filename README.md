# EspCloudUpdate [![N|Solid](http://sanusb.blogspot.com.br/favicon.ico)](http://sanusb.org/)

This project for all shows an ESP32 and ESP266 microcontrollers Update Environment for Internet programming with same generic sketch using the website  [sanusb.org/espupdate](http://sanusb.org/espupdate). In this project, the free version of Firebase (Google JSON objects Database) was implemented to generate the cloud update trigger for .bin files. 

The update (OTA) transmits the compiled binary *.bin* files via Internet. To generate a .bin file from your sketch, go to Sketch menu of the Arduino IDE > Export compiled Binary. To perform this cloud firmware update, the users need to write in the sketch only the Wifi ssid, the password and the same name as the profile entered on the site and when uploading the .bin to site, remote firmware update of ESP microcontrollers via the Internet takes place. The user profile name entered on the website [sanusb.org/espupdate](http://sanusb.org/espupdate) may be alphanumeric (for example: sandro190575 or sandrojuca).

It is possible to test this transmission application via the Internet, accessing the site http://sanusb.org/espupdate/ through the laboratory or residence network and the ESP32/ESP8266 microcontroller anchored to the smartphone connected to the 4G mobile network, or vice versa.

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
 
It is worth considering that, through tests carried out, files compiled .bin as with the same name and with sequential downloads for updating in the cloud, it may happen that the .bin file sent last for the update is not downloaded by ESP microcontroller, but a .bin file previously submitted, because they have the same name and the same download URL. For this reason, in this project, the name of the .bin files submitted on the site has versioned (modified) names, consequently the download URL address as well, this prevents a previously sent file or another file with the same name from being downloaded, generating a unique and exclusive URL. In this case, to version the name and download URL of the .bin file, it was used: year, month, day, hour and the upload cyclic order.

For the update condition {if (newVersionInt> InitialVersionInt)}, it is considered between the variables greater than (>) and not different (!=), to ensure that the latest .bin file version is always greater than the previous .bin version and to prevent that variables reading can generate unwanted update trigger during operation.

In summary, download and install the sample library .zip available on https://github.com/SanUSB/EspCloudUpdate and dependencies for ESP8266 (https://github.com/mobizt/Firebase-ESP8266) or ESP32 (https://github.com/mobizt/Firebase-ESP32). To install the librarys, follow the steps: Arduino IDE -> Sketch -> Include Library -> Add .zip Libraries. Unzip the .zip folder and open the example EspUpdateStart.ino. Write the name of your profile and WiFi network in the sketch. Update the sketch only the first time using the USB port. So it is now possible to transfer over the internet generating the .bin file.  For this, go to Sketch menu of the Arduino IDE > Export compiled Binary.

This project, the loop sketch function is practically free so you can implement your projects and be able to update them in a simple and remote way over the Internet. Tutorial: https://youtu.be/En_hFO5f4U8.
 
*Have fun!*
