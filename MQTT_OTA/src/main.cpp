#include <Arduino.h>
#include "configs.h"
#include "mqtt_handler.h"
#include "ota.h"
#include <WiFi.h>
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(WiFi.status()!= WL_CONNECTED){
    WiFi.begin("FLBS","hellowworld");
  }

}

void loop() {

  // put your main code here, to run repeatedly:
 
    mqtt_app_start();
}