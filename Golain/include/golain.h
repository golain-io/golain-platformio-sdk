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
#include <string.h>
#include <golain_config.h>
#include <golain_clients.h>
#include <device_shadow.h>
#include <data_points.h>
#include <device_flags.h>
#include <ext_hashmap.h>
#ifdef GOLAIN_DEVICE_HEALTH_ENABLED
#include "deviceHealth.h"
#endif

#ifdef GOLAIN_P_LOGS_ENABLED
#include "persistent_logs.h"
#endif
void Internal_callback(char *topic, byte *payload, unsigned int length);
void test_callback(char *topic, byte *payload, unsigned int length)
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
    client.setCallback(Internal_callback);

#ifdef GOLAIN_DEVICE_FLAG_ENABLED
    user_flag_callback = clientt->user_flag_callback;
#endif
#ifdef GOLAIN_DEVICE_SHADOW_ENABLED
    user_shadow_callback_ptr = clientt->user_shadow_callback;
#endif

    while (!client.connected())
    {
        Serial.println("Connecting to Golain...");
        if (client.connect(clientt->client_id))
        {
            Serial.println("Connected to Golain");
        }
        else
        {
            Serial.print("Failed to connect to Golain , rc=");
            Serial.println(client.state());
            delay(5000);
        }
    }
}

void Internal_callback(char *topic, byte *payload, unsigned int length)
{

#ifdef GOLAIN_DEVICE_SHADOW_ENABLED

    if (strcmp(topic, DEVICE_SHADOW_TOPIC_R) == 0)
    {

        memset(receive_buf, 0, Shadow_size);

        for (int i = 0; i < length; i++)
        {
            receive_buf[i] = payload[i];
        }

        golain_shadow_get(receive_buf, length);
    }
#endif

#ifdef GOLAIN_DEVICE_FLAG_ENABLED
    void (*user_flag_callback)() = NULL;

    if (strcmp(topic, DEVICE_FLAG_R) == 0)
    {
        if (user_flag_callback == NULL)
        {
            return;
        }
        else
        {
            user_flag_callback();
        }
    }
#endif
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
#ifdef GOLAIN_DEVICE_SHADOW_ENABLED
void golain_set_user_shadow_callback(golain_config *client, void (*callback)())
{

    client->user_shadow_callback = callback;
}
#endif
void golain_init(golain_config *clientt)
{
    if (clientt->ca_cert == NULL || clientt->device_cert == NULL || clientt->device_pvt_key == NULL || clientt->client_id == NULL || clientt->root_topic == NULL)
    {
        Serial.println("Please set all the parameters of the client");
        return;
    }
    global_map = createHashMap();
    mqtt_connect(clientt);
#ifdef GOLAIN_DEVICE_SHADOW_ENABLED
    client.subscribe(DEVICE_SHADOW_TOPIC_R, 1);
#endif
#ifdef GOLAIN_DEVICE_FLAG_ENABLED
    client.subscribe(DEVICE_FLAG_R, 1);
#endif
}

bool encode_string(pb_ostream_t *stream, const pb_field_t *field, void *const *arg)
{
    char *string_to_encode = (char *)*arg;
    if (!pb_encode_tag_for_field(stream, field))
        return false;

    return pb_encode_string(stream, (uint8_t *)string_to_encode, strlen(string_to_encode));
}

bool decode_string(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    uint8_t buffer[1024] = {0};

    /* We could read block-by-block to avoid the large buffer... */
    if (stream->bytes_left > sizeof(buffer) - 1)
        return false;

    if (!pb_read(stream, buffer, stream->bytes_left))
        return false;

    sprintf((char *)*arg, "%s", buffer);
    return true;
}

#endif