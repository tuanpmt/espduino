espduino
========
This is MQTT client for arduino connect to broker via ESP8266 AT command, port from [MQTT client library for Contiki](https://github.com/esar/contiki-mqtt)

You can find the Native MQTT client library for ESP8266 work well here: 
[https://github.com/tuanpmt/esp_mqtt](https://github.com/tuanpmt/esp_mqtt)


Features
========


- Support subscribing, publishing, authentication, will messages, keep alive pings and all 3 QoS levels (it should be a fully functional client).
- Easy to setup and use

Warning
========

you have to change the SERIAL_BUFFER_SIZE minimum is 64 bytes in 

```
C:\Program Files (x86)\Arduino\hardware\arduino\cores\arduino\HardwareSerial.cpp
```

Status
========
**WORKING**


Usage
=======

```c
#include <espduino.h>
#include <SoftwareSerial.h>

SoftwareSerial debugPort(2, 3); // RX, TX

ESP esp(&Serial, &debugPort, 13);


void wifiCb(uint8_t status)
{
  debugPort.println("WIFI: Connected");
  esp.mqttConnect("mqtt.domain.io", 1880);
}

void mqttConnected(uint32_t* args)
{
  debugPort.println("MQTT:Connected");
  esp.subscribe("/topic");
}

void mqttDisconnected(uint32_t* args)
{
  debugPort.println("MQTT:Disconnected");
}

void mqttPublished(uint32_t* args)
{
  debugPort.println("MQTT:Published");
}

void mqttData(uint32_t* args)
{
  mqtt_event_data_t *event_data = (mqtt_event_data_t *)args;
  char topic[16];
  char data[32];

  memcpy(topic, event_data->topic, event_data->topic_length);
  memcpy(data, event_data->data, event_data->data_length);
  topic[event_data->topic_length] = 0;
  data[event_data->data_length] = 0;
  debugPort.print("Received, topic:");
  debugPort.print(topic);
  debugPort.print(", data:");
  debugPort.println(data);
}

const char clientId[] = "clientId";
const char user[] = "user";
const char pass[] = "pass";


void setup() {
  delay(200);
  Serial.begin(115200);
  debugPort.begin(9600);

  /* setup event */
  esp.wifiCb.attach(&wifiCb);
  esp.mqttConnected.attach(&mqttConnected);
  esp.mqttDisconnected.attach(&mqttDisconnected);
  esp.mqttPublished.attach(&mqttPublished);
  esp.mqttData.attach(&mqttData);

  /* Init data */
  esp.initMqttClient(clientId, user, pass, 30);

  /* wifi connect */
  esp.wifiConnect("DVES_HOME", "yourpassword");
}

void loop() {
  esp.process();
}

```

ISSUE
=====
Almost everything is done, can send MQTT connect message, subscribe message, and receive data, BUT you need to implement for automatic reconnect ...


[MQTT Broker for test](https://github.com/mcollina/mosca)

[MQTT Client for test](https://chrome.google.com/webstore/detail/mqttlens/hemojaaeigabkbcookmlgmdigohjobjm?hl=en)

**Contributing:**

***Feel free to contribute to the project in any way you like!***

**Requried:**

ESP8266 with AT-Command 0.2

**Authors:**
[Tuan PM](https://twitter.com/TuanPMT)

**Donations**

Invite me to a coffee
[![Donate](https://www.paypalobjects.com/en_US/GB/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=JR9RVLFC4GE6J)


**LICENSE - "MIT License"**

Copyright (c) 2014-2015 Tuan PM, https://twitter.com/TuanPMT

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.