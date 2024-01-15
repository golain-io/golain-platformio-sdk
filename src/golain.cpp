#include <golain.h>
#include <golain_logs.h>

#include <stdio.h>

#ifdef CONFIG_GOLAIN_USE_WIFI
#include <WiFi.h>
#include <WiFiClientSecure.h>
#endif

#include <Arduino.h>
#include <PubSubClient.h>

static Golain *_instance;
Shadow GlobalShadow;
Shadow GlobalShadowPrevious;

GolainData golain_data_points[CONFIG_GOLAIN_MAX_DATA_POINTS];
uint8_t golain_data_buffer[CONFIG_GOLAIN_DATA_BUFFER_SIZE];

#ifdef CONFIG_GOLAIN_USE_WIFI
    WiFiClientSecure secureClient;
#endif

#ifdef CONFIG_GOLAIN_USE_GSM
    #include <SSLClient.h>
    TinyGsmClient gsmClient;
    SSLClient secureClient(&gsmClient);
#endif

#ifdef CONFIG_GOLAIN_ENABLE_OTA
    #include <ArduinoOTA.h>
    #ifdef CONFIG_GOLAIN_USE_GSM
        TinyGsmClient gsmOTAClient;
        SSLClient secureOTAClient(&gsmOTAClient);
    #endif
#endif


Golain::Golain(char* ca_cert, char* device_cert, char* device_private_key) {
    this->ca_cert = ca_cert;
    this->device_cert = device_cert;
    this->device_private_key = device_private_key;
    _instance = this;
}

bool Golain::begin() {
    // check for wifi connection
    #ifdef CONFIG_GOLAIN_USE_WIFI
    if (WiFi.status() != WL_CONNECTED) {
        GOLAIN_LOGE("Golain", "WiFi not connected");
        return false;
    }
    #endif

    secureClient.setCACert(ca_cert);
    secureClient.setCertificate(device_cert);
    secureClient.setPrivateKey(device_private_key);

    PubSubClient::setClient(secureClient);
    PubSubClient::setServer(GOLAIN_MQTT_SERVER, GOLAIN_MQTT_PORT);
    PubSubClient::setCallback(internal_mqtt_callback);
    PubSubClient::setKeepAlive(30);

    // connect to mqtt
    uint8_t retries = 1;
    while (!mqtt_connect() && retries <= 5) {
        GOLAIN_LOGW("Golain", "failed to connect to mqtt, retrying... %d", retries);
        retries++;
    }
    if (!PubSubClient::connected()) {
        GOLAIN_LOGE("Golain", "failed to connect to mqtt after %d retries", retries);
        return false;
    }
    // subscribe to topics
    if (!mqtt_subscribe()) {
        GOLAIN_LOGE("Golain", "failed to subscribe to topics");
        return false;
    }
    // ask for shadow
    if (!PubSubClient::publish(DEVICE_SHADOW_TOPIC_P, nullptr, true)) {
        GOLAIN_LOGE("Golain", "failed to publish shadow request");
        return false;
    }
    return true;
}

#ifdef CONFIG_GOLAIN_USE_GSM
bool Golain::begin(TinyGsm* modem) {
    if (!modem->isNetworkConnected()) {
        GOLAIN_LOGE("Golain", "GSM not connected");
        return false;
    }
    gsmClient.init(modem, 0);
    #ifdef CONFIG_GOLAIN_ENABLE_OTA
        gsmOTAClient.init(modem, 1);
    #endif
    
    return this->begin();
}
#endif

bool Golain::mqtt_connect() {
    if (PubSubClient::connected())
    {
        return true;
    } else {
        if (millis() - mqttLastReconnectAttempt > 200) {
            mqttLastReconnectAttempt = millis();
            PubSubClient::disconnect();
            if (PubSubClient::connect(DEVICE_NAME)) {
                mqttLastReconnectAttempt = 0;
            }
        }
    }
    return PubSubClient::connected();
}

void Golain::loop() {
    if (PubSubClient::connected()) {
        PubSubClient::loop();
    } else {
        if (mqtt_connect()) {
            mqtt_subscribe();
        }
    }
}

bool Golain::mqtt_subscribe(){
    bool ret = PubSubClient::subscribe(DEVICE_SHADOW_TOPIC_R, 1);
    if (!ret) {
        #if CONFIG_GOLAIN_INTERNAL_LOG_LEVEL >= LOG_ERROR
            GOLAIN_LOGE("MQTT", "Failed to subscribe to shadow topic");
        #endif
        return false;
    }
    #ifdef CONFIG_GOLAIN_ENABLE_OTA
        ret = PubSubClient::subscribe(DEVICE_OTA_TOPIC_SUB, 1);
        if (!ret) {
            #if CONFIG_GOLAIN_INTERNAL_LOG_LEVEL >= LOG_ERROR
                GOLAIN_LOGE("MQTT", "Failed to subscribe to OTA topic");
            #endif
            return false;
        }
    #endif
    return ret;
}

Result Golain::registerDataPoint(const char* name, const pb_msgdesc_t* msg, int size) {
    if(data_point_count == CONFIG_GOLAIN_MAX_DATA_POINTS) {
        GOLAIN_LOGE("Golain", "Max data points reached");
        return {GOLAIN_FAIL};
    }
    golain_data_points[data_point_count].name = name;
    golain_data_points[data_point_count].msg = msg;
    golain_data_points[data_point_count].size = size;
    Result r = {value: data_point_count++};
    return r;
}

Result Golain::postData(GolainDataPoint id, void* data){
    char* topic = NULL;
    memset(golain_data_buffer,0,CONFIG_GOLAIN_DATA_BUFFER_SIZE);
    pb_ostream_t stream = pb_ostream_from_buffer(golain_data_buffer, CONFIG_GOLAIN_DATA_BUFFER_SIZE);
    if(pb_encode(&stream, golain_data_points[id].msg, data)){
        GOLAIN_LOGV("Golain", "Data point bytes written: %d\n", stream.bytes_written);
    } else {
        GOLAIN_LOGE("Golain", "Encoding failed while posting data: %s\n", PB_GET_ERROR(&stream));
        return {GOLAIN_FAIL};
    }

    topic = (char*)malloc(strlen(DEVICE_DATA_TOPIC)+strlen(golain_data_points[id].name)+1);
    if(!topic){
        GOLAIN_LOGE("Golain", "Failed to allocate memory for topic");
        return {GOLAIN_FAIL};
    }
    strcpy(topic, DEVICE_DATA_TOPIC);
    strcat(topic, golain_data_points[id].name);

    if(PubSubClient::publish(topic, golain_data_buffer, stream.bytes_written)){
        GOLAIN_LOGV("Golain", "Published data point");
        free(topic);
        return {GOLAIN_OK};
    } else {
        GOLAIN_LOGW("Golain", "Failed to publish data point");
        free(topic);
        return {GOLAIN_FAIL};
    }
}

golain_err_t Golain::updateShadow()
{
    shadow_updated = false;
    bool status;
    memset(transmission_buffer,0,Shadow_size);
    pb_ostream_t stream = pb_ostream_from_buffer(transmission_buffer, Shadow_size);

    status = pb_encode(&stream, Shadow_fields, &GlobalShadow);
    GOLAIN_LOGV("Golain", "Wrote %d bytes to shadow buffer", stream.bytes_written);
    if(!status)
    {
        GOLAIN_LOGE("Golain", "Encoding failed while updating shadow: %s\n", PB_GET_ERROR(&stream));
        return GOLAIN_FAIL;
    }
    else
    {
        if (PubSubClient::publish(DEVICE_SHADOW_TOPIC_U, transmission_buffer, stream.bytes_written)){
            GOLAIN_LOGV("Golain", "Published shadow update");
            shadow_updated = true;
            return GOLAIN_OK;
        } else {
            GOLAIN_LOGW("Golain", "Failed to publish shadow update");
            return GOLAIN_FAIL;
        }
    }
}

golain_err_t Golain::setDeviceShadowCallback(void (*callback)(Shadow, Shadow)) {
    if (callback == NULL) {
        return GOLAIN_FAIL;
    }
    this->user_shadow_callback_ptr = callback;
    return GOLAIN_OK;
}

void Golain::internal_mqtt_callback(char* topic, uint8_t* payload, unsigned int len){
    if (strcmp(topic, DEVICE_SHADOW_TOPIC_R) == 0) {
        _instance->get_shadow(payload, len);
    }
    #ifdef CONFIG_GOLAIN_ENABLE_OTA
        else if (strncmp(topic, DEVICE_OTA_TOPIC, sizeof(DEVICE_OTA_TOPIC)) == 0) {

        }
    #endif
}

void Golain::get_shadow(uint8_t* buffer, size_t message_length){
    bool status;
    pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);
    GlobalShadowPrevious = GlobalShadow;
    status = pb_decode(&stream, Shadow_fields, &GlobalShadow);
    if (status)
    {
        if (user_shadow_callback_ptr != NULL)
            user_shadow_callback_ptr(GlobalShadow, GlobalShadowPrevious);
    }
    else
    {
        GlobalShadow = GlobalShadowPrevious;
        GOLAIN_LOGE("Golain", "Decoding failed while updating shadow: %s\n", PB_GET_ERROR(&stream));
    }
}

Golain::~Golain() {
    PubSubClient::disconnect();
}