// this file contains golain internal defs
#pragma once
#include <Arduino.h>
#include<stdio.h>
#include<stdlib.h>
#define DEVICE_SHADOW_TOPIC TOPIC_ROOT DEVICE_NAME "/device-shadow"
#define DEVICE_SHADOW_TOPIC_R TOPIC_ROOT DEVICE_NAME "/device-shadow/r"
#define DEVICE_SHADOW_TOPIC_U TOPIC_ROOT DEVICE_NAME "/device-shadow/u"
#define DEVICE_OTA_TOPIC TOPIC_ROOT DEVICE_NAME "/ota"
#define DEVICE_DATA_TOPIC TOPIC_ROOT DEVICE_NAME "/device-data/"
#define DEVICE_FLAG_SET TOPIC_ROOT DEVICE_NAME "/device-status/flag-set"
#define DEVICE_FLAG_RESET TOPIC_ROOT DEVICE_NAME "/device-status/flag-reset"
#define DEVICE_FLAG_TOGGLE TOPIC_ROOT DEVICE_NAME "/device-status/flag-toggle"
#define DEVICE_FLAG_R TOPIC_ROOT DEVICE_NAME "/device-status/flag-read"

#define mqtt_server "dev.golain.io"
#define mqtt_port 8083

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
#ifdef GOLAIN_DEVICE_FLAG_ENABLED
    void (*user_flag_callback)();
#endif

} golain_config;