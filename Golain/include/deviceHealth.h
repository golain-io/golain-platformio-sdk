#ifndef DEVICE_HEALTH_H 
#define DEVICE_HEALTH_H
#ifdef GOLAIN_DEVICE_HEALTH_ENABLED

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <Arduino.h>
#include "pb.h"
#include "deviceHealth.pb.h"

//#include "persistent_logs.h"
#define reset_counter_key   "dksdnj"
#define deviceHealthKey     "nujsnn"
#define protobufSize         256
#define STORAGE_NAMESPACE   "kjsdsd"


esp_err_t deviceHealthStore(uint8_t *deviceHealthproto)
{

    nvs_handle_t my_handle;

    // Open
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
        return err;

    err = nvs_set_blob(my_handle, deviceHealthKey, deviceHealthproto, protobufSize);
    if (err != ESP_OK)
        return err;

    nvs_close(my_handle);
    return err;
}

int restart_counter() {
    int8_t num = 0;
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
     // ESP_LOGE(LOGGING_TAG, "Error reading NVS (%s)", esp_err_to_name(err));
      return num;
    }

    err = nvs_get_i8(my_handle, reset_counter_key, &num);
    if (err != ESP_OK) {
     // ESP_LOGE(LOGGING_TAG, "Error reading NVS (%s)", esp_err_to_name(err));
      return num;
    }

    num++;
    err = nvs_set_i8(my_handle, reset_counter_key, num);
    if (err != ESP_OK) {
    //  ESP_LOGE(LOGGING_TAG, "Error reading NVS (%s)", esp_err_to_name(err));
      return num;
    }
   
    nvs_close(my_handle);
    
    return num;
}

bool encode_message(uint8_t *buffer, size_t buffer_size, size_t *message_length)
{esp_chip_info_t info;
    char data[] = "Zephyr is better\n";
    bool status;
    esp_chip_info(&info);

    /* Allocate space on the stack to store the message data.
     *
     * Nanopb generates simple struct definitions for all the messages.
     * - check out the contents of simple.pb.h!
     * It is a good idea to always initialize your structures
     * so that you do not have garbage data from RAM in there.
     * 
     */
  


    deviceHealth message = deviceHealth_init_zero;

    /* Create a stream that will write to our buffer. */
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, buffer_size);
    message.lastRebootReason = (rebootReason)esp_reset_reason();
    message.deviceRevision = info.revision;
    //message.numberOferrorsSinceLastReboot = errorCountSinceLastReset;
    message.userNumericData = 569.78;
    message.numberOfReboots = restart_counter();
    message.userStringData.funcs.encode = encode_string;
    message.userStringData.arg = data;

    /* Now we are ready to encode the message! */
    status = pb_encode(&stream, deviceHealth_fields, &message);
    *message_length = stream.bytes_written;

    if (!status)
    {
        printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
    }

    return status;
}

bool decode_message(uint8_t *buffer, size_t message_length)
{
    bool status;
    char rx[20];
    /* Allocate space for the decoded message. */
    deviceHealth message = deviceHealth_init_zero;

    /* Create a stream that reads from the buffer. */
    pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);

    message.userStringData.funcs.decode = decode_string;
    message.userStringData.arg = rx;

    /* Now we are ready to decode the message. */
    status = pb_decode(&stream, deviceHealth_fields, &message);

    /* Check for errors... */
    if (status)
    {

        Serial.printf("was called from %s", __func__);
        Serial.printf( "number of errors since last reboot: %d", message.numberOferrorsSinceLastReboot);
        Serial.printf( "last reboot reason %d", message.lastRebootReason);
        Serial.printf("number of reboots: %d", message.numberOfReboots);
        Serial.printf( "chip revision: %d", message.deviceRevision);
        Serial.printf( "user numeric data: %f", message.userNumericData);
        Serial.printf( "user string data: %s", rx);
    }
    else
    {
        Serial.printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
    }

    return status;
}
#endif
#endif