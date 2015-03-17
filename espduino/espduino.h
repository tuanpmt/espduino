/**
 * \file
 *       ESP8266 bridge arduino library 
 * \author
 *       Tuan PM <tuanpm@live.com>
 */
#ifndef _ARDUINO_WIFI_H_
#define _ARDUINO_WIFI_H_
#include <stdint.h>
#include <HardwareSerial.h>
#include <Arduino.h>
#include "FP.h"
#include "crc16.h"
#include "ringbuf.h"

#define ESP_TIMEOUT 2000

#define SLIP_START 0x7E
#define SLIP_END  0x7F
#define SLIP_REPL 0x7D
#define SLIP_ESC(x) (x ^ 0x20)

typedef enum
{
  CMD_NULL = 0,
  CMD_RESET,
  CMD_IS_READY,
  CMD_WIFI_CONNECT,
  CMD_MQTT_SETUP,
  CMD_MQTT_CONNECT,
  CMD_MQTT_DISCONNECT,
  CMD_MQTT_PUBLISH, 
  CMD_MQTT_SUBSCRIBE,
  CMD_MQTT_LWT,
  CMD_MQTT_EVENTS,
  CMD_REST_SETUP,
  CMD_REST_REQUEST,
  CMD_REST_SETHEADER,
  CMD_REST_EVENTS
}CMD_NAME;

enum WIFI_STATUS{
  STATION_IDLE = 0,
  STATION_CONNECTING,
  STATION_WRONG_PASSWORD,
  STATION_NO_AP_FOUND,
  STATION_CONNECT_FAIL,
  STATION_GOT_IP
};

typedef struct{
  uint8_t *buf;
  uint16_t bufSize;
  uint16_t dataLen;
  uint8_t isEsc;
  uint8_t isBegin;
}PROTO;

typedef struct __attribute((__packed__)) {
  uint16_t len;
  uint8_t data;
} ARGS;

typedef struct __attribute((__packed__)) {
  uint16_t cmd;
  uint32_t callback;
  uint32_t _return;
  uint16_t argc;
  ARGS args;
}PACKET_CMD;

class ESP;
class RESPONSE;
class MQTT;

class RESPONSE {
private:
  uint16_t arg_num;
  uint8_t *arg_ptr;
  PACKET_CMD* cmd;
public:
  RESPONSE(void *response);
  uint16_t getArgc();
  int32_t popArgs(uint8_t *data, uint16_t maxLen);
  uint16_t argLen();
  String popString();
  void popString(String* data);
};


class ESP
{
public:
  ESP(Stream *serial, Stream* debug, int chip_pd);
  ESP(Stream *serial, int chip_pd);
  Stream *_debug;

  FP<void, void*> wifiCb;
  uint32_t return_value;
  uint16_t return_cmd;
  boolean is_return;

  void wifiConnect(const char* ssid, const char* password);
  void process();
  uint16_t request(uint16_t cmd, uint32_t callback, uint32_t _return, uint16_t argc);
  uint16_t request(uint16_t crc_in, uint8_t* data, uint16_t len);
  uint16_t request(uint16_t crc);
  void reset();
  boolean ready();
  void enable();
  void disable();
  boolean waitReturn(uint32_t timeout);
  boolean waitReturn();

private:
  Stream *_serial;
  
  boolean _debugEn;
  PROTO _proto;
  uint8_t _protoBuf[512];
  int _chip_pd;
  

  void init();
  void INFO(String info);
  void protoCompletedCb(void);
  void write(uint8_t data);
  void write(uint8_t* data, uint16_t len);
};
#endif
