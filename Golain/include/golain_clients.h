#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <string.h>
#include <topics.h>


#define CONFIG_GOLAIN_DATA_BUFFER_MAX_SIZE 256
WiFiClientSecure espClient;
PubSubClient client(espClient);
