/**
 * \file
 *       ESP8266 Pushbullet example
 * \author
 *       Tuan PM <tuanpm@live.com>
 */

#include <SoftwareSerial.h>
#include <espduino.h>
#include <rest.h>
#include <dht.h>

dht DHT;


SoftwareSerial debugPort(4, 5); // RX, TX

ESP esp(&Serial, &debugPort, 6);

REST rest(&esp);

boolean wifiConnected = false;
boolean readyToSend = false;
int pirState = 0;
const char pirStatus[][4] = {"OFF", "ON"};

void wifiCb(void* response)
{
  uint32_t status;
  RESPONSE res(response);

  if(res.getArgc() == 1) {
    res.popArgs((uint8_t*)&status, 4);
    if(status == STATION_GOT_IP) {
      debugPort.println("WIFI CONNECTED");
     
      wifiConnected = true;
    } else {
      wifiConnected = false;
    }
    
  }
}
void pirChange()
{
  pirState = digitalRead(2);
  readyToSend = true;
}

void setup() {
  Serial.begin(19200);
  debugPort.begin(19200);
  esp.enable();
  delay(500);
  esp.reset();
  delay(500);
  while(!esp.ready());

  debugPort.println("ARDUINO: setup rest client");
  if(!rest.begin("api.pushbullet.com", 443, true)) {
    debugPort.println("ARDUINO: failed to setup rest client");
    while(1);
  }
  rest.setContentType("application/json");
  rest.setHeader("Authorization: Bearer <your_api_key>\r\n");

  /*setup wifi*/
  debugPort.println("ARDUINO: setup wifi");
  esp.wifiCb.attach(&wifiCb);

  esp.wifiConnect("DVES_HOME","wifipassword");
  debugPort.println("ARDUINO: system started");

  attachInterrupt(0, pirChange, CHANGE);
}

void loop() {
  char data_buf[256];
  esp.process();
  if(wifiConnected && readyToSend) {
      sprintf(data_buf, "{\"type\": \"note\", \"title\": \"PIR Motion\", \"body\": \"Status = [%s]\"}", pirStatus[pirState]);
      debugPort.println(data_buf);
      rest.post("/v2/pushes", (const char*)data_buf);
      debugPort.println("ARDUINO: send post");

      if(rest.getResponse(data_buf, 256) == HTTP_STATUS_OK){
        debugPort.println("ARDUINO: POST successful");

    
      } else {
        debugPort.println("ARDUINO: POST error");
      }
      delay(2000);
      readyToSend = false;
      
    } 
}