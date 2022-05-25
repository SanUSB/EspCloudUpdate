//****This data must be filled in by the user****************
const char* Profile = "----------";  // The same profile name on http://sanusb.org/espupdate
const char* ssid = "-----------";   // Wifi 
const char* password = "-----------";
//***********************************************************
#include "EspCloudUpdate.h"
//***********************************************************
void setup() {
  wifi_config();
  firebase_config();
  init_update();

}
//**********************************************************
void loop() {  
  digitalWrite(led, !digitalRead(led));
  delay(100);  
  ++timecount;
  if(timecount == 50){ //check update 50 x 100 ms
    timecount=0;
    EspCloudUpdate.esp_update(); //it must be in loop
  } 
//***************loop content*******************************


//**********************************************************
}
