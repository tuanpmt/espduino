#include "espduino.h"
#include "FP.h"


#define CHECK(f, msg)  if(f) { INFO(msg); return FALSE; }


ESP::ESP(Stream *serial, int chip_pd):
_serial(serial), _chip_pd(_chip_pd)
{
  _debugEn = FALSE;
  pinMode(_chip_pd, OUTPUT);
}
ESP::ESP(Stream *serial, Stream* debug, int chip_pd):
_serial(serial), _debug(debug), _chip_pd(_chip_pd)
{
  _debugEn = TRUE;
  pinMode(_chip_pd, OUTPUT);
}

void ESP::INFO(String info)
{
  if(_debugEn)
    _debug->println(info);
}

BOOL ESP::initMqttClient(const char *clientId, const char *user, const char *pass, U32 keepalive)
{
  memset(&connect_info, 0, sizeof(mqtt_connect_info_t));
  connect_info.client_id = clientId;
  connect_info.username = user;
  connect_info.password = pass;
  connect_info.keepalive = keepalive;
  connect_info.clean_session = 1;

  mqtt_state.in_buffer = _buffSend;
  mqtt_state.in_buffer_length = MQTT_BUF_SIZE;
  mqtt_state.out_buffer =  _buffRecv;
  mqtt_state.out_buffer_length = MQTT_BUF_SIZE;
  mqtt_state.connect_info = &connect_info;

  keepAliveTick = 0;
  reconnectTick = 0;

  QUEUE_Init(&topicQueue, _subscribeBuf, MAX_SUB_LEN, MAX_SUB_TOPIC);
  connState = MQTT_IDLE;
}


BOOL ESP::wifiConnect(const char* ssid, const char* pass)
{
  char temp[128];
  sprintf(temp, "AT+CWJAP=\"%s\",\"%s\"", ssid, pass);
  CHECK(!reset(), "RESET ESP failed");
  INFO("RESET ESP done");
  CHECK(!exec("ATE0", "OK", 1000), "ATE0 failed");
  CHECK(!exec("AT+CWMODE=1", "OK", 1000), "CWMODE failed");
  delay(200);
  CHECK(!exec(temp, "OK", 30000), "CWJAP failed");
  wifiCb(0);
  return TRUE;
}

BOOL ESP::mqttConnect(const char* host, U16 port)
{
  char temp[64];
  CHECK(!exec("AT+CIPMUX=0", "OK", 1000), "CWJAP failed");
  exec("AT+CIPCLOSE", "OK", 500);
  sprintf(temp, "AT+CIPSTART=\"TCP\",\"%s\",%d", host, port);
  CHECK(!exec(temp, "OK", 20000), "TCP: Connection failed");
  INFO("TCP: Connected");
  connState = MQTT_CONNECT_SEND;
}
BOOL ESP::subscribe(char* topic)
{
  INFO("MQTT: subscribe, topic");

  if(QUEUE_Puts(&topicQueue, topic) == -1){
    INFO("MQTT: Exceed the amount of queues\r\n");
  } 
  else {
    if(connState == MQTT_DATA)
      connState = MQTT_SUBSCIBE_SEND;
  }
}
void ESP::deliver_publish(uint8_t* message, int length)
{

  mqtt_event_data_t event_data;

  event_data.topic_length = length;
  event_data.topic = (char*)mqtt_get_publish_topic(message, &event_data.topic_length);

  event_data.data_length = length;
  event_data.data = (char*)mqtt_get_publish_data(message, &event_data.data_length);
  mqttData((uint32_t*) &event_data);


}
void ESP::process()
{
  U16 len;
  char topic[MAX_SUB_LEN], temp[32];
  uint8_t msg_type;
  uint8_t msg_qos;

  uint16_t msg_id;

  switch(connState) {
  case TCP_RECONNECT:

    break;
  case MQTT_CONNECT_SEND:
    mqtt_msg_init(&mqtt_state.mqtt_connection, mqtt_state.out_buffer, mqtt_state.out_buffer_length);
    mqtt_state.outbound_message = mqtt_msg_connect(&mqtt_state.mqtt_connection, mqtt_state.connect_info);
    if(!send(mqtt_state.outbound_message->data, mqtt_state.outbound_message->length)) {
      connState = TCP_RECONNECT_REQ;
      return;
    }
    mqtt_state.outbound_message = NULL;
    connState = MQTT_CONNECT_SENDING;
    break;
  case MQTT_SUBSCIBE_SEND:
    if(QUEUE_Gets(&topicQueue, topic) == 0){
      mqtt_state.outbound_message = mqtt_msg_subscribe(&mqtt_state.mqtt_connection,
      topic, 0,
      &mqtt_state.pending_msg_id);
      mqtt_state.pending_msg_type = MQTT_MSG_TYPE_SUBSCRIBE;
      if(!send(mqtt_state.outbound_message->data, mqtt_state.outbound_message->length)) {
        connState = TCP_RECONNECT_REQ;
        return;
      }

      mqtt_state.outbound_message = NULL;
      connState = MQTT_SUBSCIBE_SENDING;
    } 
    else {
      connState = MQTT_DATA;
    }

    break;
  case MQTT_DATA:
    if(mqtt_state.outbound_message != NULL){

      if(!send(mqtt_state.outbound_message->data, mqtt_state.outbound_message->length)) {
        connState = TCP_RECONNECT_REQ;
        return;
      }
      mqtt_state.outbound_message = NULL;
      if(mqtt_state.pending_msg_type == MQTT_MSG_TYPE_PUBLISH && mqtt_state.pending_msg_id == 0)
        INFO("MQTT: Publish message is done!\r\n");
      break;
    }
    break;
  }

  _serial->setTimeout(300);

  if(receive(mqtt_state.in_buffer, &len) && len > 0) {
    switch(connState){
    case MQTT_CONNECT_SENDING:
      if(mqtt_get_type(mqtt_state.in_buffer) != MQTT_MSG_TYPE_CONNACK){
        INFO("MQTT: Invalid packet CONNACK\r\n");
        sprintf(temp, "Len:%d", len);
        INFO(temp);
        connState = TCP_RECONNECT_REQ;
        return;
      } 
      else {
        INFO("MQTT: Connected");
        connState = MQTT_DATA;
        mqttConnected(0);
      }
      break;
    case MQTT_SUBSCIBE_SENDING:
      if(mqtt_get_type(mqtt_state.in_buffer) != MQTT_MSG_TYPE_SUBACK){
        INFO("MQTT: Invalid packet\r\n");
        connState = TCP_RECONNECT_REQ;
      } 
      else {
        if(QUEUE_IsEmpty(&topicQueue)){
          connState = MQTT_DATA;
        } 
        else {
          connState = MQTT_SUBSCIBE_SEND;
        }
        INFO("MQTT: Subscribe successful\r\n");
      }
      break;
    case MQTT_DATA:
      mqtt_state.message_length_read = len;
      mqtt_state.message_length = mqtt_get_total_length(mqtt_state.in_buffer, mqtt_state.message_length_read);
      msg_type = mqtt_get_type(mqtt_state.in_buffer);
      msg_qos = mqtt_get_qos(mqtt_state.in_buffer);
      msg_id = mqtt_get_id(mqtt_state.in_buffer, mqtt_state.in_buffer_length);
      switch(msg_type)
      {
      case MQTT_MSG_TYPE_SUBACK:
        if(mqtt_state.pending_msg_type == MQTT_MSG_TYPE_SUBSCRIBE && mqtt_state.pending_msg_id == msg_id)
          INFO("MQTT: Subscribe successful");
        break;
      case MQTT_MSG_TYPE_UNSUBACK:
        if(mqtt_state.pending_msg_type == MQTT_MSG_TYPE_UNSUBSCRIBE && mqtt_state.pending_msg_id == msg_id)
          INFO("MQTT: UnSubscribe successful\r\n");
        break;
      case MQTT_MSG_TYPE_PUBLISH:
        if(msg_qos == 1)
          mqtt_state.outbound_message = mqtt_msg_puback(&mqtt_state.mqtt_connection, msg_id);
        else if(msg_qos == 2)
          mqtt_state.outbound_message = mqtt_msg_pubrec(&mqtt_state.mqtt_connection, msg_id);

        deliver_publish(mqtt_state.in_buffer, mqtt_state.message_length_read);
        break;
      case MQTT_MSG_TYPE_PUBACK:
        if(mqtt_state.pending_msg_type == MQTT_MSG_TYPE_PUBLISH && mqtt_state.pending_msg_id == msg_id){
          INFO("MQTT: Publish successful\r\n");
          mqttPublished(0);
        }

        break;
      case MQTT_MSG_TYPE_PUBREC:
        mqtt_state.outbound_message = mqtt_msg_pubrel(&mqtt_state.mqtt_connection, msg_id);
        break;
      case MQTT_MSG_TYPE_PUBREL:
        mqtt_state.outbound_message = mqtt_msg_pubcomp(&mqtt_state.mqtt_connection, msg_id);
        break;
      case MQTT_MSG_TYPE_PUBCOMP:
        if(mqtt_state.pending_msg_type == MQTT_MSG_TYPE_PUBLISH && mqtt_state.pending_msg_id == msg_id){
          INFO("MQTT: Public successful\r\n");
          mqttPublished(0);
        }
        break;
      case MQTT_MSG_TYPE_PINGREQ:
        mqtt_state.outbound_message = mqtt_msg_pingresp(&mqtt_state.mqtt_connection);
        break;
      case MQTT_MSG_TYPE_PINGRESP:
        // Ignore
        break;
      }
      // NOTE: this is done down here and not in the switch case above
      // because the PSOCK_READBUF_LEN() won't work inside a switch
      // statement due to the way protothreads resume.
      if(msg_type == MQTT_MSG_TYPE_PUBLISH)
      {
        uint16_t len;

        // adjust message_length and message_length_read so that
        // they only account for the publish data and not the rest of the
        // message, this is done so that the offset passed with the
        // continuation event is the offset within the publish data and
        // not the offset within the message as a whole.
        len = mqtt_state.message_length_read;
        mqtt_get_publish_data(mqtt_state.in_buffer, &len);
        len = mqtt_state.message_length_read - len;
        mqtt_state.message_length -= len;
        mqtt_state.message_length_read -= len;

        if(mqtt_state.message_length_read < mqtt_state.message_length)
        {
          msg_type = MQTT_PUBLISH_RECV;
        }

      }
      break;
    case MQTT_PUBLISH_RECV:
      /*
				 * Long publish message, not implement yet
       				 * TODO: Implement method used deliver_publish_continuation
       				 */

      break;
    }
  } 
  

  static unsigned long time = 0;
  if(millis() - time > 1000) {
    time = millis();
    if(connState == MQTT_DATA){
      keepAliveTick ++;
      if(keepAliveTick > mqtt_state.connect_info->keepalive){

        INFO("MQTT: Send keepalive packet");
        mqtt_state.outbound_message = mqtt_msg_pingreq(&mqtt_state.mqtt_connection);
        keepAliveTick = 0;

      }

    } 
    else if(connState == TCP_RECONNECT_REQ){
      reconnectTick ++;
      if(reconnectTick > MQTT_RECONNECT_TIMEOUT) {
        reconnectTick = 0;
        connState = TCP_RECONNECT;

      }
    }
  }




}
BOOL ESP::exec(const char* cmd, const char* waitFor, U32 timeout)
{
  clearBuffer();
  _serial->setTimeout(timeout);
  _serial->println(cmd);
  return _serial->find((char*)waitFor);
}
BOOL ESP::send(U8 *data, U16 len)
{
  char temp[64];
  sprintf(temp, "AT+CIPSEND=%d", len);
  INFO(temp);
  CHECK(!exec(temp, "> ", 1000), "TCP: Send failed");
  while(len--)
    _serial->write(*data++);
  _serial->flush();
  return TRUE;
}

BOOL ESP::receive(U8 *data, U16 *len)
{
  char lenData[10];
  U16 lenInt;
  _serial->setTimeout(100);
  if(_serial->find("+IPD,")) {
    *len = _serial->parseInt();
    _serial->read(); /* read ':' */
    lenInt = _serial->readBytes((char*)data, *len);
    /* ISSUSE HERE: You need to change SERIAL_BUFFER_SIZE to minimun 32 bytes in 
       C:\Program Files (x86)\Arduino\hardware\arduino\cores\arduino\HardwareSerial.cpp
    */
    return (*len == lenInt);  
  }
  return FALSE;
}
void ESP::clearBuffer()
{
  while(_serial->available())
    _serial->read(); 
}

void ESP::enable()
{
  digitalWrite(_chip_pd, HIGH);
}

void ESP::disable()
{
  digitalWrite(_chip_pd, LOW);
}

BOOL ESP::reset()
{
  return exec("AT+RST", "\r\nready\r\n", 5000);
}











