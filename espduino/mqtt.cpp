/**
 * \file
 *       ESP8266 MQTT Bridge
 * \author
 *       Tuan PM <tuanpm@live.com>
 */
 
#include "mqtt.h"


MQTT::MQTT(ESP *esp8266)
{
  esp = esp8266;
}
boolean MQTT::begin(const char* client_id, const char* user, const char* pass, uint16_t keep_alive, boolean clean_seasion)
{
  uint16_t crc;
  uint32_t wait, cb_data;

  crc = esp->request(CMD_MQTT_SETUP, 0, 1, 9);
  crc = esp->request(crc,(uint8_t*)client_id, strlen(client_id));
  crc = esp->request(crc,(uint8_t*)user, strlen(user));
  crc = esp->request(crc,(uint8_t*)pass, strlen(pass));
  crc = esp->request(crc,(uint8_t*)&keep_alive, 2);
  crc = esp->request(crc,(uint8_t*)&clean_seasion, 1);
  cb_data = (uint32_t)&connectedCb;
  crc = esp->request(crc,(uint8_t*)&cb_data, 4);

  cb_data = (uint32_t)&disconnectedCb;
  crc = esp->request(crc,(uint8_t*)&cb_data, 4);

  cb_data = (uint32_t)&publishedCb;

  crc = esp->request(crc,(uint8_t*)&cb_data, 4);

  cb_data = (uint32_t)&dataCb;
  crc = esp->request(crc,(uint8_t*)&cb_data, 4);
  esp->request(crc);

  if(esp->waitReturn() == false || esp->return_cmd == 0 || esp->return_value == 0)
    return false;
  remote_instance = esp->return_value;
  return true;
}
boolean MQTT::lwt(const char* topic, const char* message, uint8_t qos, uint8_t retain)
{
  uint16_t crc;
  uint32_t wait;


  crc = esp->request(CMD_MQTT_LWT, 0, 1, 5);
  crc = esp->request(crc,(uint8_t*)&remote_instance, 4);
  crc = esp->request(crc,(uint8_t*)topic, strlen(topic));
  crc = esp->request(crc,(uint8_t*)message, strlen(message));
  crc = esp->request(crc,(uint8_t*)&qos, 1);
  crc = esp->request(crc,(uint8_t*)&retain, 1);
  esp->request(crc);
  if(esp->waitReturn() && esp->return_value)
    return true;
  return false;
}
boolean MQTT::lwt(const char* topic, const char* message)
{
  return lwt(topic, message, 0, 0);
}
void MQTT::connect(const char* host, uint32_t port, boolean security)
{
  uint16_t crc;
  crc = esp->request(CMD_MQTT_CONNECT, 0, 0, 4);
  crc = esp->request(crc,(uint8_t*)&remote_instance, 4);
  crc = esp->request(crc,(uint8_t*)host, strlen(host));
  crc = esp->request(crc,(uint8_t*)&port, 4);
  crc = esp->request(crc,(uint8_t*)&security, 1);
  esp->request(crc);
}
void MQTT::connect(const char* host, uint32_t port)
{
  connect(host, port, false);
}

void MQTT::disconnect()
{
  uint16_t crc;
  crc = esp->request(CMD_MQTT_DISCONNECT, 0, 0, 1);
  crc = esp->request(crc,(uint8_t*)&remote_instance, 4);
  esp->request(crc);
}
void MQTT::subscribe(const char* topic, uint8_t qos)
{
  uint16_t crc;
  crc = esp->request(CMD_MQTT_SUBSCRIBE, 0, 0, 3);
  crc = esp->request(crc,(uint8_t*)&remote_instance, 4);
  crc = esp->request(crc,(uint8_t*)topic, strlen(topic));
  crc = esp->request(crc,(uint8_t*)&qos, 1);
  esp->request(crc);

}
void MQTT::subscribe(const char* topic)
{
  subscribe(topic, 0);
}
void MQTT::publish(const char* topic, uint8_t *data, uint16_t len, uint8_t qos, uint8_t retain)
{
  uint16_t crc;
  crc = esp->request(CMD_MQTT_PUBLISH, 0, 0, 6);
  crc = esp->request(crc,(uint8_t*)&remote_instance, 4);
  crc = esp->request(crc,(uint8_t*)topic, strlen(topic));
  crc = esp->request(crc,(uint8_t*)data, len);
  crc = esp->request(crc,(uint8_t*)&len, 2);
  crc = esp->request(crc,(uint8_t*)&qos, 1);
  crc = esp->request(crc,(uint8_t*)&retain, 1);
  esp->request(crc);
}
void MQTT::publish(const char* topic, char* data, uint8_t qos, uint8_t retain)
{
  publish(topic, (uint8_t*)data, strlen(data), qos, retain);
}
void MQTT::publish(const char* topic, char* data)
{
  publish(topic, (uint8_t*)data, strlen(data), 0, 0);
}