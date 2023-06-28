#ifndef _GOLAIN_H
#define _GOLAIN_H
#include <stdlib.h>
#include <stdio.h>
#include <Arduino.h>
#include <pb.h>
#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "mqtt_config.h"

#ifdef GOLAIN_DEVICE_HEALTH_ENABLED
#include "deviceHealth.h"
#endif

#ifdef GOLAIN_P_LOGS_ENABLED
#include "persistent_logs.h"
#endif

#ifdef GOLAIN_DEVICE_SHADOW_ENABLED
#include <shadow.pb.h>
void shadow_callback(char *topic, byte *payload, unsigned int length);
void (*user_shadow_callback_ptr)() = NULL;
#endif

typedef struct _golain_config
{

    char *root_topic;
    char *ca_cert;
    char *device_cert;
    char *device_pvt_key;
    char *client_id;
#ifdef GOLAIN_DEVICE_SHADOW_ENABLED
    void (*user_shadow_callback)();
#endif
#ifdef GOLAIN_OTA_ENABLED
    void (*ota_callback)(char *, byte *, unsigned int);
#endif

} golain_config;

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

void mqtt_connect(golain_config *clientt)
{
    espClient.setCACert(clientt->ca_cert);
    espClient.setCertificate(clientt->device_cert);
    espClient.setPrivateKey(clientt->device_pvt_key);
    client.setServer(mqtt_server, mqtt_port);
#ifdef GOLAIN_DEVICE_SHADOW_ENABLED
    client.setCallback(shadow_callback);
    user_shadow_callback_ptr = clientt->user_shadow_callback;
#endif

    while (!client.connected())
    {
        Serial.println("Connecting to MQTT broker...");
        if (client.connect(clientt->client_id))
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

    bool ret = client.publish(DEVICE_SHADOW_TOPIC_U, data, len);

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

#ifdef GOLAIN_DATA_POINT_ENABLED
uint8_t device_data_buffer[CONFIG_GOLAIN_DATA_BUFFER_MAX_SIZE];

void golain_post_device_data(void *device_data_struct, const pb_msgdesc_t *message_fields, char *topic, size_t message_size)
{
    bool status;
    pb_ostream_t stream = pb_ostream_from_buffer(device_data_buffer, message_size);

    status = pb_encode(&stream, message_fields, device_data_struct);

    if (!status)
    {
        Serial.printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
        return;
    }
    else
    {
        Serial.printf("Encoding successful\n");
    }
    String temp = String(DEVICE_DATA_TOPIC) + String(topic);
    client.publish((temp).c_str(), (const char *)device_data_buffer, stream.bytes_written - 1);
}

#endif

#ifdef GOLAIN_DEVICE_SHADOW_ENABLED

Shadow global_shadow = Shadow_init_zero;

uint8_t receive_buf[Shadow_size];

void golain_shadow_set(uint8_t *buffer, size_t *message_length)

{
    bool status;

    pb_ostream_t stream = pb_ostream_from_buffer(buffer, Shadow_size);

    status = pb_encode(&stream, Shadow_fields, &global_shadow);
    *message_length = stream.bytes_written;

    if (!status)
    {
        Serial.printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
        return;
    }
    else
    {
        Serial.printf("Encoding successful\n");
    }
}

void golain_shadow_get(uint8_t *buffer, size_t message_length)
{

    bool status;
    pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);

    status = pb_decode(&stream, Shadow_fields, &global_shadow);

    // if (!status)
    // {
    //     Serial.printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
    //     return;
    // }
    // else
    // {
    //     Serial.println("Decoded Successfully");
    // }
    // user_shadow_callback();
    if (user_shadow_callback_ptr != NULL)
    {
        user_shadow_callback_ptr();
    }
}

void shadow_callback(char *topic, byte *payload, unsigned int length)
{
    memset(receive_buf, 0, Shadow_size);
 
    for (int i = 0; i < length; i++)
    {
        receive_buf[i] = payload[i];
    }

    golain_shadow_get(receive_buf, length);
}

#endif

void golain_set_root_topic(golain_config *client, char *str)
{

    client->root_topic = str;
}

void golain_set_ca_cert(golain_config *client, char *str)
{

    client->ca_cert = str;
}

void golain_set_device_cert(golain_config *client, char *str)
{

    client->device_cert = str;
}

void golain_set_device_pvt_key(golain_config *client, char *str)
{

    client->device_pvt_key = str;
}

void golain_set_client_id(golain_config *client, char *str)
{

    client->client_id = str;
}

void golain_set_user_shadow_callback(golain_config *client, void (*callback)())
{

    client->user_shadow_callback = callback;
}

void golain_init(golain_config *clientt)
{
    if (clientt->ca_cert == NULL || clientt->device_cert == NULL || clientt->device_pvt_key == NULL || clientt->client_id == NULL || clientt->root_topic == NULL)
    {
        Serial.println("Please set all the parameters of the client");
        return;
    }
    mqtt_connect(clientt);
#ifdef GOLAIN_DEVICE_SHADOW_ENABLED
    client.subscribe(DEVICE_SHADOW_TOPIC_R, 1);
#endif
}

#endif