/**
 * \file
 *       ESP8266 MQTT Bridge
 * \author
 *       Tuan PM <tuanpm@live.com>
 */
#ifndef _MQTT_H_
#define _MQTT_H_

#include <stdint.h>
#include <Arduino.h>
#include "FP.h"
#include <espduino.h>

class MQTT {
private:
  uint32_t remote_instance;
  ESP *esp;

public:
  FP<void, void*> connectedCb;
  FP<void, void*> disconnectedCb;
  FP<void, void*> publishedCb;
  FP<void, void*> dataCb;

  
  MQTT(ESP *esp);
  boolean begin(const char* client_id, const char* user, const char* pass, uint16_t keep_alive, boolean clean_seasion);
  boolean lwt(const char* topic, const char* message, uint8_t qos, uint8_t retain);
  boolean lwt(const char* topic, const char* message);
  void connect(const char* host, uint32_t port, boolean security);
  void connect(const char* host, uint32_t port);
  void disconnect();
  void subscribe(const char* topic, uint8_t qos);
  void subscribe(const char* topic);
  void publish(const char* topic, uint8_t *data, uint16_t len, uint8_t qos, uint8_t retain);
  void publish(const char* topic, char* data, uint8_t qos, uint8_t retain);
  void publish(const char* topic, char* data);

};
#endif