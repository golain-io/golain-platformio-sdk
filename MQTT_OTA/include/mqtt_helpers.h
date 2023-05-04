#ifndef mqtt_helpers
#define mqtt_helpers
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <Update.h>
#include <configs.h>

void callback(char *topic, byte *payload, unsigned int length)
{

  if (strcmp(topic, DEVICE_OTA_LENGTH) == 0 && (CONFIG_OTA_ENABLE == 1))
  {
    char var[length];
    for (int i = 0; i < length; i++)
    {
      // Update.write((uint8_t)payload[i],sizeof);
      var[i] = payload[i];
    }
    // int temp = atoi(var);
    Serial.println("Firmware size is ");
    Serial.print(atoi(var));
    Serial.println();
    FIRMWARE_SIZE = atoi(var);
    mqttClient.publish("status", "1");
  }

  if (strcmp(topic, DEVICE_OTA_DATA) == 0 &&  (CONFIG_OTA_ENABLE == 1))
  {
    // Serial.print(length);
    Serial.println("Firmware update received");
    Update.begin(FIRMWARE_SIZE);
    uint8_t var[length];

    for (int i = 0; i < length; i++)
    {
      // Update.write((uint8_t)payload[i],sizeof);
      var[i] = payload[i];
    }
    Update.write(var, length);
    TOTAL_WRITTEN_FIRMWARE += length;
    Serial.printf("Received till now %d and total %d",TOTAL_WRITTEN_FIRMWARE,FIRMWARE_SIZE);
    if (TOTAL_WRITTEN_FIRMWARE >= FIRMWARE_SIZE)
    {
      if (Update.end())
      {
        Serial.println("Firmware update successful, restarting...");
        mqttClient.publish("firmware/update/status", "success");
        ESP.restart();
      }
      else
      {
        Serial.println("Firmware update failed");
        mqttClient.publish("firmware/update/status", "fail");
      }
    }
  }
}


void mqtt_start()
{
    wifiClient.setCACert(AWS_CERT_CA);
    wifiClient.setCertificate(AWS_CERT_CRT);
    wifiClient.setPrivateKey(AWS_CERT_PRIVATE);
    wifiClient.setInsecure();
    mqttClient.setServer(mqtt_server, mqtt_port);
    mqttClient.setCallback(callback);

    while (!mqttClient.connected())
    {
        Serial.println("Connecting to MQTT broker...");
        if (mqttClient.connect(client_id))
        {
            Serial.println("Connected to MQTT broker");
            mqttClient.subscribe(DEVICE_OTA_LENGTH);
            mqttClient.subscribe(DEVICE_OTA_DATA);
        }
        else
        {
            Serial.print("Failed to connect to MQTT broker, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" retrying in 5 seconds...");
            delay(5000);
        }
    }
}

#endif