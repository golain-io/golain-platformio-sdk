#ifndef _GOLAIN_H
#define _GOLAIN_H
#include "mqtt_helper.h"
#include <stdlib.h>
#include <stdio.h>

#include <Arduino.h>
#include <pb.h>
#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include <shadow.pb.h>
#define GOLAIN_DEVICE_SHADOW_ENABLED 1


Shadow global_shadow = Shadow_init_zero;

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
    else{
        Serial.printf("Encoding successful\n");
    }
}


void golain_shadow_get(uint8_t *buffer, size_t message_length)
{

    bool status;
    pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);

    status = pb_decode(&stream, Shadow_fields, &global_shadow);

    if (!status)
    {
        Serial.printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
        return;
    }
}


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

void golain_set_callback(golain_config *client, void (*callback)(char *, byte *, unsigned int))
{

    client->callback = callback;
}

void golain_init(golain_config *clientt)
{
    if (clientt->ca_cert == NULL || clientt-> device_cert == NULL || clientt->device_pvt_key == NULL || clientt->client_id == NULL || clientt->root_topic == NULL)
    {
        Serial.println("Please set all the parameters of the client");
        return;
    }
    mqtt_connect(clientt);
    #ifdef GOLAIN_DEVICE_SHADOW_ENABLED
        client.subscribe(DEVICE_SHADOW_TOPIC_R);
    #endif
}

#endif