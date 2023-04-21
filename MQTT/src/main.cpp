#include <Arduino.h>
#include "golain_mqtt.h"
#include <WiFi.h>

const char* wifi_ssid = "lol";
const char* wifi_password = "Casio@123";


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Attempting to connecting to the given ssid and password");
  WiFi.begin(wifi_ssid,wifi_password);
  while(WiFi.status() != WL_CONNECTED){
    Serial.println(".");
  }
  Serial.println("Connected to wifi");

  mqtt_app_start();

}

void loop() {
  // put your main code here, to run repeatedly:
}