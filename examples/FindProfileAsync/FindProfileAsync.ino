/*
If you are on the same network and do not know the last profile used, enter the IP in a browser to find out the profile name  
Tutorial: https://www.youtube.com/watch?v=TP7psRuSwrM and https://www.youtube.com/watch?v=En_hFO5f4U8

The dependencies of the AsyncIOT tool are the Async libraries. For ESP8266 are ESPAsyncTCP and ESPAsyncWebServer at:
https://github.com/me-no-dev/ESPAsyncTCP
https://github.com/me-no-dev/ESPAsyncWebServer

For ESP32 are AsyncTCP and the same ESPAsyncWebServer at:
https://github.com/me-no-dev/AsyncTCP
https://github.com/me-no-dev/ESPAsyncWebServer

To install all the libraries, including https://github.com/SanUSB/EspCloudUpdate , follow the steps: Arduino IDE -> Sketch 
-> Include Library -> Add .zip Libraries.
*/
//***************This data must be filled in by the user*******************************
const char* Profile = "----------";  // The same profile name on http://sanusb.org/espupdate
const char* ssid = "-----------";   // Wifi 
const char* password = "-----------";
//***********************************************************
#if defined(ESP8266)
    #include <ESPAsyncTCP.h>
#elif defined(ESP32)
    #include <AsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
AsyncWebServer server(80);
//***********************************************************
#include "EspCloudUpdate.h"
//***********************************************************
void setup() {
  wifi_config();
  firebase_config();
  init_update();
//***********************************************************
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ //type only IP address
         AsyncResponseStream *response = request->beginResponseStream("text/html");
         response->printf("<meta charset='UTF-8'> <link rel='icon' type='image/x-icon' href='http://sanusb.blogspot.com.br/favicon.ico'/>");
         response->printf("<h2>Profile Name: %s </h2>", Profile);                              
         response->printf("<a href='http://sanusb.org/espupdate/'>Esp Cloud Update</a>");
         request->send(response);
    });    
   server.begin();
  Serial.println("Type the IP in a browser to find out the profile name");
}
//**********************************************************
void loop() {  
  digitalWrite(led, !digitalRead(led));
  delay(100);  
  ++timecount;
  if(timecount == 50){ //check update 50 x 100 ms
    timecount=0;
    EspCloudUpdate.esp_update(); //it must be in loop
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } 
//***************loop content*******************************


//**********************************************************
}
