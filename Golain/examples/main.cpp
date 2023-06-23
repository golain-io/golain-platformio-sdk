#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "mqtt_config.h"
#include "mqtt_helper.h"
// Set the WiFi credentials
const char* ssid = "lol";
const char* password = "Casio@123";


void setup() {
  // Start the Serial communication
  Serial.begin(9600);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  mqtt_connect();

}

void loop() {
  // Handle MQTT messages
  client.loop();

  // Publish a message every 5 seconds
  static unsigned long lastPublish = 0;
  if (millis() - lastPublish > 5000) {
    lastPublish = millis();
    client.publish("test/topic", "Hello, world!");
  }
}

