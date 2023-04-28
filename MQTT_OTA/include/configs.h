#ifndef mqtt_config
#define mqtt_config
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <Update.h>

// WiFi network credentials
const char *ssid = "lol";
const char *password = "Casio@123";
void callback(char *topic, byte *payload, unsigned int length);
// MQTT broker details
long FIRMWARE_SIZE = 0;
long TOTAL_WRITTEN_FIRMWARE = 0;
const char *mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char *mqtt_username = "";
const char *mqtt_password = "";
const char *mqtt_topic = "golain/firmware/update";
const char *ota_len = "golain/firmware/length";
const char *ota_publish = "golain/firmware/length/status";
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

















#endif