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
  esp.initMqttClient(clientId, user, pass, 30);

  /* wifi connect */
  esp.wifiConnect("DVES_HOME", "yourpassword");
}

void loop() {
  esp.process();
}




