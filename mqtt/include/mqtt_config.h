#ifndef MQTT_CONFIG
#define MQTT_CONFIG
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

#define mqtt_server  "dev.golain.io"
#define mqtt_port  8083

// #define TOPIC_ROOT "lol"
// #define DEVICE_NAME "ok"
#define DEVICE_SHADOW_TOPIC     TOPIC_ROOT DEVICE_NAME "/device-shadow" 
#define DEVICE_SHADOW_TOPIC_R   TOPIC_ROOT DEVICE_NAME "/device-shadow/r"
#define DEVICE_SHADOW_TOPIC_U   TOPIC_ROOT DEVICE_NAME "/device-shadow/u"
#define DEVICE_OTA_TOPIC        TOPIC_ROOT DEVICE_NAME "/ota"
#define DEVICE_DATA_TOPIC       TOPIC_ROOT DEVICE_NAME "/device-data/"

#define CONFIG_GOLAIN_DATA_BUFFER_MAX_SIZE 256
WiFiClientSecure espClient;
PubSubClient client(espClient);








#endif