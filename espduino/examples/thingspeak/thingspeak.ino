/**
 * \file
 *       ESP8266 RESTful Bridge example
 * \author
 *       Tuan PM <tuanpm@live.com>
 */

#include <SoftwareSerial.h>
#include <espduino.h>
#include <rest.h>
#include <dht.h>

dht DHT;


SoftwareSerial debugPort(2, 3); // RX, TX

ESP esp(&Serial, &debugPort, 4);

REST rest(&esp);

boolean wifiConnected = false;

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

void setup() {
  Serial.begin(19200);
  debugPort.begin(19200);
  esp.enable();
  delay(500);
  esp.reset();
  delay(500);
  while(!esp.ready());

  debugPort.println("ARDUINO: setup rest client");
  if(!rest.begin("api.thingspeak.com")) {
    debugPort.println("ARDUINO: failed to setup rest client");
    while(1);
  }

  /*setup wifi*/
  debugPort.println("ARDUINO: setup wifi");
  esp.wifiCb.attach(&wifiCb);

  esp.wifiConnect("DVES_HOME","wifipassword");
  debugPort.println("ARDUINO: system started");
}

void loop() {
  char response[266];
  esp.process();
  if(wifiConnected) {
    int chk = DHT.read11(7);
    if(chk == DHTLIB_OK){
      char buff[64];
      char str_hum[6], str_temp[6];
      dtostrf(DHT.humidity, 4, 2, str_hum);
      dtostrf(DHT.temperature, 4, 2, str_temp);
      sprintf(buff, "/update?api_key=MAY03AKJDMPP4Y4I&field1=%s&field2=%s", str_hum, str_temp);
      debugPort.println(buff);
      rest.get((const char*)buff);
      debugPort.println("ARDUINO: send get");

      if(rest.getResponse(response, 266) == HTTP_STATUS_OK){
        debugPort.println("ARDUINO: GET successful");
        debugPort.println(response);
      }
      delay(30000);
      
    } else {
      debugPort.print("error,\r\n"); 
    }
    
    
  }
  
  
}