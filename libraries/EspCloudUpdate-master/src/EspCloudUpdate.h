/**********************************************************sanusb.org*************************************************************
This project shows an ESP32 and ESP266 microcontrollers Update Environment for Internet programming with same generic sketch 
using the website sanusb.org/espupdate. In this project, the free version of Firebase (Google JSON objects Database) was 
implemented to generate the cloud update trigger for .bin files.

The update (OTA) transmits the compiled binary .bin files via Internet. To generate a .bin file from your sketch, go to Sketch 
menu of the Arduino IDE > Export compiled Binary. To perform this cloud firmware update, the users need to write in the sketch 
only the Wifi ssid, the password and the same name as the profile entered on the site and when uploading the .bin to site, remote 
firmware update of ESP microcontrollers via the Internet takes place. The user profile name entered on the website 
sanusb.org/espupdate may be alphanumeric (for example: sandro190575 or sandrojuca).

To install the ESP32 and ESP8266 boards in Arduino IDE, follow these next instructions: In your Arduino IDE, go to File> Preferences. 
Insert into the "Additional Board Manager URLs":

https://dl.espressif.com/dl/package_esp32_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json
Open the Boards Manager: Go to Tools > Board > Boards Manager: Search for ESP32 and press install button for the "ESP32 by Espressif 
Systems". Also search for ESP8266 and press install button for the "ESP8266 by ESP8266 Community".

The dependencies of the EspCloudUpdate tool are the Firebase libraries. For ESP8266:

https://github.com/mobizt/Firebase-ESP8266
For ESP32:

https://github.com/mobizt/Firebase-ESP32

To install all the libraries, including https://github.com/SanUSB/EspCloudUpdate , follow the steps:

Arduino IDE -> Sketch -> Include Library -> Add .zip Libraries.

You can usually find the installed ESP32 and ESP8266 libraries at:

On Windows:

   C:\Users\UserName\Documents\Arduino\libraries (tested)
On Linux:

  /home/UserName/Arduino/Libraries (tested).
On OSX:

  ~/Documents/Arduino/libraries.
In the installed EspCloudUpdate library folder there is an example for testing this proposed tool called EspUpdateStart.ino.

It is worth considering that, through tests carried out, files compiled .bin as with the same name and with sequential downloads 
for updating in the cloud, it may happen that the .bin file sent last for the update is not downloaded by ESP microcontroller, 
but a .bin file previously submitted, because they have the same name and the same download URL. For this reason, in this project, 
the name of the .bin files submitted on the site has versioned (modified) names, consequently the download URL address as well, 
this prevents a previously sent file or another file with the same name from being downloaded, generating a unique and exclusive 
URL. In this case, to version the name and download URL of the .bin file, it was used: year, month, day, hour and the upload cyclic 
order.

For the update condition {if (newVersionInt> InitialVersionInt)}, it is considered between the variables greater than (>) and not 
different (!=), to ensure that the latest .bin file version is always greater than the previous .bin version and to prevent that 
variables reading can generate unwanted update trigger during operation.

In summary, download and install the sample library .zip available on this link https://github.com/SanUSB/EspCloudUpdate. To install 
the library, follow the steps: Arduino IDE -> Sketch -> Include Library -> Add .zip Libraries. Unzip the .zip folder and open the 
example EspUpdateStart.ino. Write the name of your profile and WiFi network in the sketch. Update the sketch only the first time 
using the USB port. So it is now possible to transfer over the internet generating the .bin file. For this, go to Sketch menu of 
the Arduino IDE > Export compiled Binary.

After the update of the .bin file is finished and the microcontroller is automatically reset, the new verification code is sent to 
http://sanusb.org/espupdate/YourProfile/conf.php in order to confirm the update on the website.

This project, the loop sketch function is practically free so you can implement your projects and be able to update them in a simple 
and remote way over the Internet.
***********************************************************************************************************************************/
#ifndef EspClouUpdate_h
#define EspCloudUpdate_h

#if defined(ESP8266)  
    #include <ESP8266WiFi.h> 
    #include <ESP8266WiFiMulti.h> 
    #include <ESP8266HTTPClient.h> 
    #include <ESP8266httpUpdate.h>
    #include <FirebaseESP8266.h>
    #define Serial Serial    
    ESP8266WiFiMulti WiFiMulti;
   
#elif defined(ESP32)
    #include <WiFi.h>
    #include <WiFiMulti.h>
    #include <HTTPClient.h>
    #include <HTTPUpdate.h>
    #include <FirebaseESP32.h>
    WiFiMulti WiFiMulti;    
#endif
    HTTPClient httpClient;

FirebaseData firebaseData;

#include <EEPROM.h>

String URL, VersionURL, newVersionSt, InitialVersionSt;
unsigned long newVersionInt, InitialVersionInt;
    
int httpCode;
int addr = 255, val = 0, timecount=0;
const int led = 2; //builtin led

const char* UrlBase = "http://sanusb.org/espupdate/";

#define FIREBASE_HOST "espupdate-4faf8.firebaseio.com/"
#define FIREBASE_AUTH "fP2VRR2GAfc8uZbuqVVNC8YGvBDbDwOOwWkruhP9"

WiFiClient client;

void init_update(void) {  
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  EEPROM.begin(256); 
//***********************************************************
	String UploadOK = String(Profile);
	UploadOK.concat("OK");
	val=EEPROM.read(addr);
	Serial.printf("previous cycle: %d\n", val);
	if (val == 255) {
		val = (InitialVersionInt%10);
		Serial.printf("Welcome!!! To upload, access: sanusb.org/espupdate\n", val);
	}
	Serial.printf("InitialVersionCicle: %d\n", (InitialVersionInt % 10));

	//Incremented code (val+1) & previous Reset -> Upload OK
	if ((InitialVersionInt%10) == (val+1) || ((InitialVersionInt%10)==0 && val==9)) {
    Serial.printf("Upload OK!!!\n");
    if (Firebase.setInt(firebaseData, UploadOK, InitialVersionInt)) 
    {
      Serial.println("Set int data success");
    }else{
    //Failed?, get the error reason
    Serial.print("Error in setInt, ");
    Serial.println(firebaseData.errorReason());
    }
    //***********************************************************/
    String Jsonconf = String(UrlBase); //upload confirmation
    Jsonconf.concat(Profile);
    Jsonconf.concat("/conf.php?s=");
    Jsonconf.concat(InitialVersionSt); 
  
    httpClient.begin(client, Jsonconf); 
    int  httpcode = httpClient.GET();
    if (httpcode <= 0) {
    Serial.printf("HTTP error: %s\n", 
    httpClient.errorToString(httpcode).c_str());
    }
    // Read the response.
    String payload = httpClient.getString();
    //Serial.print("Response HTTP = ");
    //Serial.println(payload);
//***********************************************************/    
  }
  EEPROM.write(addr, (InitialVersionInt % 10));
  EEPROM.commit();   
}

void wifi_config(void) {
  Serial.println();
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);
  WiFi.begin(ssid, password);
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to Wifi AP...");
    for (int i = 0; i < 300; i++) { //try to connect to WiFi for max 30s
      if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("it is connected after %d seconds", (i / 10));
        break;
      }
      yield();
      delay(100);
    }
  }
  if (WiFi.status() == WL_CONNECTED) {
    //if you get here you have connected to the WiFi
    Serial.println("\nconnected...yesss! :)");
  } else {
    Serial.println("TimeOut! Not Connected even after 10 Seconds trying...\n *** Something wrong happened. It did not connected... *** ");
  }
}

void firebase_config(void) {

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  //Enable auto reconnect the WiFi when connection lost
  Firebase.reconnectWiFi(true);

  if (Firebase.getInt(firebaseData, Profile)) //Success
  {
    InitialVersionSt = firebaseData.intData();

    InitialVersionInt = InitialVersionSt.toInt();
    Serial.print("InitialVersionInt = ");
    Serial.println(firebaseData.intData());

  } else {
    Serial.print("Error in getInt, ");
    Serial.println(firebaseData.errorReason());
  }

}

class EspCloudUpdateClass {
  public:

    void esp_update(void) {

      if ((WiFiMulti.run() == WL_CONNECTED)) { // wait for WiFi connection

        //*****************************************************************************   
        if (Firebase.getInt(firebaseData, Profile)) {
          //Success
          //Serial.print("Get int data success, int = ");
          //Serial.println(firebaseData.intData());

        } else {
          //Failed?, get the error reason from firebaseData

          Serial.print("Error in getInt, ");
          Serial.println(firebaseData.errorReason());
        }

        newVersionSt = firebaseData.intData();
        newVersionInt = newVersionSt.toInt();
        Serial.print("Online firmware version: ");
        Serial.println(newVersionSt);

        //Serial.print("Initial firmware version was: ");
        //Serial.println(InitialVersionSt);
        InitialVersionInt = InitialVersionSt.toInt();

        //*******************Upload function**********************************    
        if (newVersionInt > InitialVersionInt || (!(newVersionInt%10) && (InitialVersionInt%10)==9)) {

          Serial.println("Preparing to update");

          String ImageURL = String(UrlBase);
          ImageURL.concat(Profile);
          ImageURL.concat("/" + newVersionSt);
          ImageURL.concat(".bin");
          Serial.print(".bin firmware Address: ");
          Serial.println(ImageURL);

          #if defined(ESP8266)
          ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW); //optional. It appears in the led's brightness fluctuation
          t_httpUpdate_return ret = ESPhttpUpdate.update(client, ImageURL);

          #elif defined(ESP32)
          httpUpdate.setLedPin(LED_BUILTIN, LOW);         
          t_httpUpdate_return ret = httpUpdate.update(client, ImageURL);
          #endif

          switch (ret) {
          case HTTP_UPDATE_FAILED:
            #if defined(ESP8266)
            Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
            #elif defined(ESP32)
            Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
            #endif
            break;

          case HTTP_UPDATE_NO_UPDATES:
            Serial.println("HTTP_UPDATE_NO_UPDATES");
            break;

          case HTTP_UPDATE_OK:
            Serial.println("HTTP_UPDATE_OK");
            break;
          }
        }
      }
    }
	
	            void sethost() {
                  #if defined(ESP8266)
                  WiFi.hostname("sanusb.org");
                  Serial.println(WiFi.hostname());
                  #elif defined(ESP32)
                  WiFi.setHostname("sanusb.org");
                  Serial.println(WiFi.getHostname());
                  #endif
            }

  private:
    bool restartRequired = true;
};
EspCloudUpdateClass EspCloudUpdate;

#endif
