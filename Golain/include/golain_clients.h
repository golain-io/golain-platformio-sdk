#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <string.h>



WiFiClientSecure espClient;
PubSubClient client(espClient);
