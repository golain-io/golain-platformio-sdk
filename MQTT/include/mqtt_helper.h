#ifndef MQTT_HELPER
#define MQTT_HELPER

#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "mqtt_config.h"
#include "mqtt_helper.h"

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Received message [");
    Serial.print(topic);
    Serial.print("]: ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void mqtt_connect()
{
    // Set the client certificates
    espClient.setCACert(AWS_CERT_CA);
    espClient.setCertificate(AWS_CERT_CRT);
    espClient.setPrivateKey(AWS_CERT_PRIVATE);
    espClient.setInsecure();
    // Set the MQTT broker details
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);

    // Connect to MQTT broker
    while (!client.connected())
    {
        Serial.println("Connecting to MQTT broker...");
        if (client.connect(client_id))
        {
            Serial.println("Connected to MQTT broker");
        }
        else
        {
            Serial.print("Failed to connect to MQTT broker, rc=");
            Serial.println(client.state());
            delay(5000);
        }
    }
}

void postshadow(uint8_t *data, int len)
{

    bool ret = client.publish(DEVICE_SHADOW_TOPIC_U, (const char *)data, len);

    if (ret)
    {
        Serial.println("Shadow Successfully published ");
    }
    else
    {
        Serial.println("Some error while publishing shadow");
    }
}

void postData(char *data, size_t length, char *topic)
{
    char topic_to_publish[sizeof(DEVICE_DATA_TOPIC) + sizeof(topic)];
    sprintf(topic_to_publish, "%s/%s", DEVICE_DATA_TOPIC, topic);
    client.publish(topic_to_publish, data, length);
    Serial.printf("Published to the topic %s\n", topic_to_publish);
}

#endif