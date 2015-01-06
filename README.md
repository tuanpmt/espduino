espduino
========
This is MQTT client for arduino connect to broker via ESP8266 AT command

Features
========

Status
========
Working


Usage
=======
planned

```c
#include "esp8266.h"
#include <SoftwareSerial.h>

SoftwareSerial debugPort(2, 3); // RX, TX

ESP esp(&Serial, &debugPort, 13);

void wifiCb(uint8_t status)
{
  debugPort.println("WIFI: Connected");
  esp.mqttConnect();
}

void mqttConnected(uint32_t* args)
{
  debugPort.println("test");
}

void mqttDisconnected(uint32_t* args)
{
  debugPort.println("test");
}

void mqttPublished(uint32_t* args)
{
  debugPort.println("test");
}

void mqttData(uint32_t* args)
{
  debugPort.println("test");
}
void setup() {
  delay(2000);
  Serial.begin(115200);
  debugPort.begin(9600);
  
  /* setup event */
  esp.wifiCb.attach(&wifiCb);
  esp.mqttConnected.attach(&mqttConnected);
  esp.mqttDisconnected.attach(&mqttDisconnected);
  esp.mqttPublished.attach(&mqttPublished);
  esp.mqttData.attach(&mqttData);
  
  /* Init data */
  esp.initMqttConn("host", 1234, 0);
  esp.initMqttClient("id","user","pass", 120);
  
  /* wifi connect */
  esp.wifiConnect("ssid", "pass");
  
}

void loop() {
  esp.process();
}
```
