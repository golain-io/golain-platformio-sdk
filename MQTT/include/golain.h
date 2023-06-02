#ifndef _GOLAIN_H
#define _GOLAIN_H
#include "mqtt_helper.h"
#include <stdlib.h>
#include <stdio.h>


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

void golain_init(golain_config *client)
{
    if(client->callback == NULL){
        Serial.println("Failed to set callback");
    }
    else  if(client->client_id == NULL){
        Serial.println("Failed to set client id");
    }
    else  if(client->device_pvt_key == NULL){
        Serial.println("Failed to set device_pvt_key");
    }
    mqtt_connect(client);
}

#endif