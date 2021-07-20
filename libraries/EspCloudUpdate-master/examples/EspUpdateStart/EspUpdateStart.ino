
//***************This data must be filled in by the user*******************************
const char* Profile = "----------";  // The same profile name on http://sanusb.org/espupdate
const char* ssid = "-----------";   // Wifi 
const char* password = "-----------";
//*************************************************************************************

#include "EspCloudUpdate.h"
#include <EEPROM.h>

const int led = 2; //builtin led
//const int TIME_CHECK_UP = 60000;
const int TIME_CHECK_UP = 5000; // ms only to test
int addr = 255, val = 0;

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
//***********************************************************
void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  EEPROM.begin(256); 
  wifi_config();
  firebase_config();
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
//**********************************************************

void loop() {
  EspCloudUpdate.esp_update();

  digitalWrite(led, !digitalRead(led));
  delay(TIME_CHECK_UP);
}
