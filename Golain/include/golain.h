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
#include <topics.h>
#define mqtt_server "dev.golain.io"
#define mqtt_port 8083

#define CONFIG_GOLAIN_DATA_BUFFER_MAX_SIZE 256
WiFiClientSecure espClient;
PubSubClient client(espClient);

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

#ifdef GOLAIN_DEVICE_FLAG_ENABLED
void (*user_flag_callback)() = NULL;
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
#ifdef GOLAIN_DEVICE_FLAG_ENABLED
    void (*user_flag_callback)();
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
    client.setCallback(shadow_callback);

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

void postData(char *data, size_t length, char *topic)
{
    char topic_to_publish[sizeof(DEVICE_DATA_TOPIC) + sizeof(topic)];
    sprintf(topic_to_publish, "%s/%s", DEVICE_DATA_TOPIC, topic);
    client.publish(topic_to_publish, data, length);
    Serial.printf("Published to the topic %s\n", topic_to_publish);
}

void shadow_callback(char *topic, byte *payload, unsigned int length)
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
uint8_t transmit_buf[Shadow_size];
bool golain_shadow_set()

{
    bool status;

    pb_ostream_t stream = pb_ostream_from_buffer(transmit_buf, Shadow_size);

    status = pb_encode(&stream, Shadow_fields, &global_shadow);
    int temp = stream.bytes_written;
    // Serial.printf("Encoded length %d\n",temp);
    if (!status)
    {
        // Serial.printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
        return false;
    }
    else
    {
        // Serial.printf("Encoding successful\n");
        return client.publish(DEVICE_SHADOW_TOPIC_U, transmit_buf, temp);
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
    if (user_shadow_callback_ptr != NULL && status)
    {
        user_shadow_callback_ptr();
    }

    else
    {
        Serial.println("Decoding failed");
    }
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

#ifdef GOLAIN_DEVICE_FLAG_ENABLED

void golain_set_flag(int num)
{
    status_flag_bit |= (1 << num);
    return client.publish(DEVICE_FLAG_SET, String(num));
}

void golain_reset_flag(int num)
{
    status_flag_bit &= ~(1 << num);
    return client.publish(DEVICE_FLAG_RESET, String(num));
}

bool golain_check_flag(int num)
{
    return (status_flag_bit << num);
}

void golain_toggle_flag(int num)
{
    if (golain_check_flag(num) == true)
    {
        golain_set_flag(num);
    }
    else
    {
        golain_reset_flag(num);
    }
    return client.publish(DEVICE_FLAG_TOGGLE, String(num));
}
#endif

#endif