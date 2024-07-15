#ifndef _GOLAIN_H
#define _GOLAIN_H

#include "golain_types.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shadow.pb.h"
#include <pb.h>
#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>

#include <Arduino.h>
#include <PubSubClient.h>

#define GOLAIN_MQTT_SERVER "broker.golain.io"
#define GOLAIN_MQTT_PORT    8083

#ifndef CONFIG_GOLAIN_USE_GSM
    #ifndef CONFIG_GOLAIN_USE_WIFI
        #define CONFIG_GOLAIN_USE_WIFI
    #endif
#endif

#ifdef CONFIG_GOLAIN_USE_GSM
#include <SSLClient.h>
#include <TinyGsmClient.h>
#endif

#define ROOT_TOPIC_PLACEHOLDER "%s"
#define DEVICE_NAME_PLACEHOLDER "%s"

#define DEVICE_SHADOW_TOPIC ROOT_TOPIC_PLACEHOLDER DEVICE_NAME_PLACEHOLDER "/device-shadow"
#define DEVICE_SHADOW_TOPIC_R DEVICE_SHADOW_TOPIC "/r" // read device shadow
#define DEVICE_SHADOW_TOPIC_U DEVICE_SHADOW_TOPIC "/u" // update device shadow
#define DEVICE_SHADOW_TOPIC_P DEVICE_SHADOW_TOPIC "/p" // ping request for device shadow 

#define DEVICE_FLAG ROOT_TOPIC_PLACEHOLDER DEVICE_NAME_PLACEHOLDER "/flag"
#define DEVICE_FLAG_SET     DEVICE_FLAG "/s"
#define DEVICE_FLAG_RESET   DEVICE_FLAG "/rs"
#define DEVICE_FLAG_TOGGLE  DEVICE_FLAG "/t"
#define DEVICE_FLAG_READ    DEVICE_FLAG "/r"

#define DEVICE_OTA_TOPIC ROOT_TOPIC_PLACEHOLDER DEVICE_NAME_PLACEHOLDER "/device-ota"
#define DEVICE_OTA_TOPIC_SUB      DEVICE_OTA_TOPIC "/+" // 1 for request
#define DEVICE_OTA_TOPIC_REQUEST  DEVICE_OTA_TOPIC "/request" // 1 for request
#define DEVICE_OTA_TOPIC_FIRMWARE DEVICE_OTA_TOPIC "/firmware" // firmware data
#define DEVICE_OTA_TOPIC_STATUS   DEVICE_OTA_TOPIC "/status" // 1 for success, 0 for failure

#define USER_ASSOC_TOPIC ROOT_TOPIC_PLACEHOLDER DEVICE_NAME_PLACEHOLDER "/user" // (posts user id to the topics)
#define USER_ASSOC_TOPIC_ASSOC    USER_ASSOC_TOPIC "/a" // associate user
#define USER_ASSOC_TOPIC_DISASSOC USER_ASSOC_TOPIC "/d" // disassociate user 

#define DEVICE_DATA_TOPIC ROOT_TOPIC_PLACEHOLDER DEVICE_NAME_PLACEHOLDER "/device-data/"
// special data type topics
#define DEVICE_LOCATION DEVICE_DATA_TOPIC "location"
#define DEVICE_HEALTH_TOPIC ROOT_TOPIC_PLACEHOLDER DEVICE_NAME_PLACEHOLDER "/health"
#define DEVICE_LOG_TOPIC ROOT_TOPIC_PLACEHOLDER DEVICE_NAME_PLACEHOLDER "/log"


#ifdef CONFIG_GOLAIN_CLOUD_LOGGING
#include <nvs.h>
#include <nvs_flash.h>
#include <golain_logs.h>
#include <golain_types.h>
#endif

#ifdef GOLAIN_DEVICE_HEALTH_ENABLED
#include "deviceHealth.h"
#endif

#ifdef GOLAIN_P_LOGS_ENABLED
#include "persistent_logs.h"
#endif

#define CONFIG_GOLAIN_DATA_BUFFER_MAX_SIZE 256

#ifndef Shadow_size
#define Shadow_size 256
#endif

typedef struct _GolainData {
    const char* name;
    const pb_msgdesc_t* msg;
    size_t size;
} GolainData;

typedef union _Result {
    golain_err_t err;
    uint16_t value;
} Result;

typedef uint16_t GolainDataPoint;

#ifndef CONFIG_GOLAIN_MAX_DATA_POINTS
#define CONFIG_GOLAIN_MAX_DATA_POINTS 5
#endif

#ifndef CONFIG_GOLAIN_DATA_BUFFER_SIZE
#define CONFIG_GOLAIN_DATA_BUFFER_SIZE 256
#endif

extern Shadow GlobalShadow;
extern Shadow GlobalShadowPrevious;

#ifndef CONFIG_GOLAIN_INTERNAL_LOG_LEVEL
#define CONFIG_GOLAIN_INTERNAL_LOG_LEVEL LOG_WARN
#endif

#ifndef ROOT_TOPIC_PLACEHOLDER
#error "Please set the device name and root topic"
#endif

#ifndef DEVICE_NAME_PLACEHOLDER
#error "Please set the device name and root topic"
#endif

class Golain : public PubSubClient {
    public:
        Golain(char*, char*, char*, char*, char*);
        ~Golain();
        
        #ifdef CONFIG_GOLAIN_USE_GSM
        bool begin(TinyGsm*);
        #endif
        
        bool begin();
        void loop();
        // DATA POINTS // 
        Result registerDataPoint(const char* name, const pb_msgdesc_t* msg, int size);
        Result postData(GolainDataPoint, void* data);
        // SHADOW //
        golain_err_t updateShadow();
        golain_err_t setDeviceShadowCallback(void (*callback)(Shadow, Shadow));
        // FLAGS // 
        golain_err_t setDeviceFlagCallback(void (*callback)(uint8_t, uint64_t, uint64_t));
        golain_err_t toggleDeviceFlag(uint8_t flag);
        golain_err_t clearDeviceFlag(uint8_t flag);
        golain_err_t setDeviceFlag(uint8_t flag, uint64_t value);
        golain_err_t setDeviceFlagRegister(uint64_t flag_register);
        uint8_t readDeviceFlag(uint8_t flag);
        uint64_t readDeviceFlagRegister();

        // OTA //
        #ifdef CONFIG_GOLAIN_ENABLE_OTA
        golain_err_t setOtaCallback(void (*callback)(uint8_t));
        #endif

    private:
        // device id, root topic
        char* device_name;
        char* root_topic;

        // refs to certs
        char* ca_cert;
        char* device_cert;
        char* device_private_key;

        // topics
        char* device_shadow_r_topic;
        char* device_shadow_p_topic;
        char* device_shadow_u_topic;

        char* device_generic_data_topic;

        uint32_t mqttLastReconnectAttempt;

        uint64_t deviceFlagRegister;
        uint64_t deviceFlagRegisterPrevious;
        void (*user_shadow_callback_ptr)(Shadow, Shadow); // user set callback for shadow

        // buffers
        uint8_t transmission_buffer[Shadow_size];
        bool shadow_updated;

        // array safety
        uint8_t data_point_count;

        // mqtt functions
        bool mqtt_connect();
        bool mqtt_subscribe();
        void get_shadow(uint8_t* buffer, size_t message_length);
        static void internal_mqtt_callback(char* topic, uint8_t* payload, unsigned int len);
};
#endif // _GOLAIN_H