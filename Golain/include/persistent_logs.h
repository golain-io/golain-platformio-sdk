#ifndef P_LOGS_H
#define P_LOGS_H

#ifdef GOLAIN_P_LOGS_ENABLED
#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>
#include "logs.pb.h"
#include "pb_common.h"
#include "pb_decode.h"
#include "pb_encode.h"
#include "pb.h"
#include "nvs_flash.h"
#include "nvs.h"

#ifndef CONFIG_MAX_PERSISTENT_LOGS
#define CONFIG_MAX_PERSISTENT_LOGS 50
#endif

void write_p_log(String tag, String log, String func, Level level);
void write_to_nvs(uint8_t *buffer, size_t len);
void read_old_logs();
#define P_LOG_I(tag, log)                                                                         \
    \    
Serial.println(String("INFO_LOG -> ") + String(tag) + "  (" + __func__ + ")  -> " + String(log)); \
    write_p_log(tag, log, String(__func__), Level_INFO);

#define P_LOG_W(tag, log)                                                                          \
    \                          
 Serial.println(String("WARN_LOG -> ") + String(tag) + "  (" + __func__ + ")  -> " + String(log)); \
    write_p_log(tag, log, String(__func__), Level_WARNING);

#define P_LOG_E(tag, log)                                                                          \
    \                        
  Serial.println(String("ERR_LOG -> ") + String(tag) + "  (" + __func__ + ")  -> " + String(log)); \
    write_p_log(tag, log, String(__func__), Level_ERROR);

void write_p_log(String tag, String log, String func, Level level)
{

    PLog plog = PLog_init_default;
    plog.level = level;

    plog.tag.funcs.encode = &encode_string;
    plog.tag.arg = (void *)tag.c_str();

    plog.function.funcs.encode = &encode_string;
    plog.function.arg = (void *)func.c_str();

    plog.message.funcs.encode = &encode_string;
    plog.message.arg = (void *)log.c_str();

    uint8_t out_buffer[256];
    pb_ostream_t ostream = pb_ostream_from_buffer(out_buffer, sizeof(out_buffer));
    pb_encode(&ostream, PLog_fields, &plog);

    Serial.println(ostream.bytes_written);
    write_to_nvs(out_buffer, ostream.bytes_written);
}

void write_to_nvs(uint8_t *buffer, size_t len)
{

    esp_err_t err;

    err = nvs_open("p_logs", NVS_READWRITE, &p_log_handle);
    if (err != ESP_OK)
    {
        Serial.println("Could not open nvs");
        return;
    }

    // get last log id
    int32_t last_log_id;
    if (nvs_get_i32(p_log_handle, "last_log_id", &last_log_id) ==
        ESP_ERR_NVS_NOT_FOUND)
    {
        last_log_id = 0;
    }

    if (last_log_id == CONFIG_MAX_PERSISTENT_LOGS)
    {
        read_old_logs();
        last_log_id = 0;
    }
    else
        last_log_id++;

    // write to nvs flash
    char key[2];
    sprintf(key, "%d", last_log_id);
    err = nvs_set_blob(p_log_handle, key, buffer, len);
    if (err != ESP_OK)
    {
        Serial.println("Could not set blob");
        Serial.println(esp_err_to_name(err));
        return;
    }

    // store current log id
    nvs_set_i32(p_log_handle, "last_log_id", last_log_id);

    err = nvs_commit(p_log_handle);
    if (err != ESP_OK)
    {
        Serial.println("Could not commit last changes to the nvs");
        return;
    }
    nvs_close(p_log_handle);
}

void read_old_logs()
{
    uint8_t buffer[256];
    size_t len;
    char key[2];
    esp_err_t err;
    int32_t last_log_id = 0;
    memset(buffer, 0, 256);
    err = nvs_open("p_logs", NVS_READWRITE, &p_log_handle);
    if (err != ESP_OK)
    {

        Serial.println("Error opening NVS " + String(esp_err_to_name(err)));
        return;
    }

    nvs_get_i32(p_log_handle, "last_log_id", &last_log_id);

    Serial.println("total " + String(last_log_id) + " logs have been logged");

    while (last_log_id != 0)
    {
        sprintf(key, "%d", last_log_id);

        nvs_get_blob(p_log_handle, key, buffer, &len);
        /*
        do whatever you want with the logs
        */
        Serial.printf("giving log id %d\n", last_log_id);
        // do what you want with logs
        for (int i = 0; i < len; i++)
        {
            Serial.print((char)buffer[i]);
        }
        Serial.println();

        memset(buffer, 0, 256); // clear the buffer
        last_log_id--;
    }
    
}
#endif
#endif