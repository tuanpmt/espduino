/**
 * \file
 *       ESP8266 arduino library for process AT command
 *       Require AT command version 2.0
 * \author
 *       Tuan PM <tuanpm@live.com>
 */
#ifndef _ESP8266_
#define _ESP8266_
#include <stdint.h>
#include "typedef.h"
#include <HardwareSerial.h>
#include <Arduino.h>
#include "FP.h"
#include "mqtt_msg.h"
#include "queue.h"

#define MQTT_BUF_SIZE  128
#define MAX_SUB_LEN    16
#define MAX_SUB_TOPIC  2
#define MQTT_RECONNECT_TIMEOUT 5  /*second*/

typedef struct mqtt_event_data_t
{
  uint8_t type;
  char* topic;
  char* data;
  uint16_t topic_length;
  uint16_t data_length;
  uint16_t data_offset;
} 
mqtt_event_data_t;

typedef struct mqtt_state_t
{
  uint16_t port;
  int auto_reconnect;
  mqtt_connect_info_t* connect_info;
  uint8_t* in_buffer;
  uint8_t* out_buffer;
  int in_buffer_length;
  int out_buffer_length;
  uint16_t message_length;
  uint16_t message_length_read;
  mqtt_message_t* outbound_message;
  mqtt_connection_t mqtt_connection;
  uint16_t pending_msg_id;
  int pending_msg_type;
} 
mqtt_state_t;

typedef enum {
  MQTT_IDLE,
  MQTT_CONNECT_SEND,
  MQTT_CONNECT_SENDING,
  MQTT_SUBSCIBE_SEND,
  MQTT_SUBSCIBE_SENDING,
  MQTT_DATA,
  MQTT_PUBLISH_RECV,
  MQTT_PUBLISHING,
  TCP_RECONNECT_REQ,
  TCP_RECONNECT
} 
tConnState;

class ESP
{
public:
  ESP(Stream *serial, Stream* debug, int chip_pd);
  ESP(Stream *serial, int chip_pd);
  BOOL exec(const char* cmd, const char* waitFor, U32 timeout);
  BOOL initMqttClient(const char *clientId, const char *user, const char *pass, U32 keepalive);
  BOOL wifiConnect(const char* ssid, const char* pass);
  BOOL mqttConnect(const char* host, U16 port);
  BOOL subscribe(char* topic);
  void process();
  void enable();
  void disable();
  BOOL reset();
  BOOL send(U8 *data, U16 len);
  BOOL receive(U8 *data, U16 *len);
  FP<void, uint8_t> wifiCb;
  FP<void, uint32_t*> mqttConnected;
  FP<void, uint32_t*> mqttDisconnected;
  FP<void, uint32_t*> mqttPublished;
  FP<void, uint32_t*> mqttData;
private:
  Stream *_serial;
  Stream *_debug;
  BOOL _debugEn;
  mqtt_connect_info_t connect_info;
  mqtt_state_t  mqtt_state;
  tConnState connState;
  QUEUE topicQueue;
  U16 keepAliveTick;
  U16 reconnectTick;
  U8 _subscribeBuf[MAX_SUB_LEN*MAX_SUB_TOPIC];
  U8 _buffSend[MQTT_BUF_SIZE];
  U8 _buffRecv[MQTT_BUF_SIZE];
  int _chip_pd;
  void INFO(String info);
  void clearBuffer();
  String readData();
  void deliver_publish(uint8_t* message, int length);


};
#endif

