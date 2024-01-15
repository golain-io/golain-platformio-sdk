#ifndef _GOLAIN_LOGS_H
#define _GOLAIN_LOGS_H

#ifdef CONFIG_GOLAIN_CLOUD_LOGGING
#include <pb.h>
#include <pb_encode.h>
#include <logs.pb.h>

#include <esp_log.h>
#include <nvs.h>
/*------------------------------------------------------------------------Persistent Logs----------------------------------------------------*/

#include <golain_types.h>
#include <golain_encode.h>

#define CONFIG_GOLAIN_MAX_PERSISTENT_LOGS 20
#define CONFIG_GOLAIN_P_LOGS_FLUSH_INTERVAL 1
#define CONFIG_GOLAIN_P_LOGS_BUFFER_SIZE 256
#ifndef TAG 
#define TAG "PERSISTENT_LOGS"
#endif


golain_err_t _golain_hal_p_log_check_nvs_errors(esp_err_t err);

golain_err_t _golain_hal_p_log_write_to_nvs(uint8_t *data, size_t len);

golain_err_t _golain_hal_p_log_write(esp_log_level_t level, const char *func, const char *tag, const char *format, ...);

golain_err_t _golain_hal_p_log_read_old_logs(uint8_t *buffer);

golain_err_t _golain_hal_p_log_get_number_of_logs(int32_t* num);

golain_err_t _golain_hal_p_log_background_push_task_init();

void _golain_hal_p_log_background_push_task(void *pvParameters);

#endif

// persistent log functions
/**
 * @brief - Logs and stores the Info statements given as a param
 *
 * @param tag A string which is used to trace the origin of log
 *
 * @param format Format in which data is to be logged and stored
 *
 * @param ... n Number of variable arguments that can be added as a part of log statement
 *
 */
#ifdef CONFIG_GOLAIN_CLOUD_LOGGING
  #define GOLAIN_LOGI(tag, format, ...) \
    ESP_LOGI(tag, "(%s)-> " format, __func__, ##__VA_ARGS__); \
    _golain_hal_p_log_write(ESP_LOGINFO, __func__, "[%s]: " format, tag, ##__VA_ARGS__);
#else
  #define GOLAIN_LOGI(tag, format, ...) \
    ESP_LOGI(tag, "(%s)-> " format, __func__, ##__VA_ARGS__);
#endif



/**
 * @brief - Logs and stores the Warning statements given as a param
 *
 * @param tag A string which is used to trace the origin of log
 *
 * @param format Format in which data is to be logged and stored
 *
 * @param ... n Number of variable arguments that can be added as a part of log statement
 *
 */  
#ifdef CONFIG_GOLAIN_CLOUD_LOGGING
  #define GOLAIN_LOGW(tag, format, ...) \
    ESP_LOGW(tag, "(%s)-> " format, __func__, ##__VA_ARGS__); \
    _golain_hal_p_log_write(ESP_LOG_WARN, __func__, "[%s]: " format, tag, ##__VA_ARGS__);
#else
  #define GOLAIN_LOGW(tag, format, ...) \
    ESP_LOGW(tag, "(%s)-> " format, __func__, ##__VA_ARGS__);
#endif

/**
 * @brief 
 *
 * @param tag A string which is used to trace the origin of log
 *
 * @param format Format in which data is to be logged and stored
 *
 * @param ... n Number of variable arguments that can be added as a part of log statement
 *
 */

#define GOLAIN_LOGD(tag, format, ...) \
    ESP_LOGD(tag, "(%s)-> " format, __func__, ##__VA_ARGS__);


#ifdef CONFIG_GOLAIN_CLOUD_LOGGING
  #define GOLAIN_LOGE(tag, format, ...) \
    ESP_LOGE(tag, "(%s)-> " format, __func__, ##__VA_ARGS__); \
    _golain_hal_p_log_write(ESP_LOGERROR, __func__, "[%s]: " format, tag, ##__VA_ARGS__);
#else
  #define GOLAIN_LOGE(tag, format, ...) \
    ESP_LOGE(tag, "(%s)-> " format, __func__, ##__VA_ARGS__);
#endif

#ifdef CONFIG_GOLAIN_CLOUD_LOGGING
  #define GOLAIN_LOGV(tag, format, ...) \
    ESP_LOGV(tag, "(%s)-> " format, __func__, ##__VA_ARGS__); \
    _golain_hal_p_log_write(ESP_LOGVERBOSE, __func__, "[%s]: " format, tag, ##__VA_ARGS__);
#else
    #define GOLAIN_LOGV(tag, format, ...) \
        ESP_LOGV(tag, "(%s)-> " format, __func__, ##__VA_ARGS__);
#endif


#ifdef CONFIG_GOLAIN_CLOUD_LOGGING

golain_err_t _golain_p_log_send_mqtt_message(uint8_t* message, uint16_t message_len){
    return _golain_hal_mqtt_publish(GOLAIN_LOGTOPIC, (const char*)message, message_len, 0, 0);
}

golain_err_t _golain_hal_p_log_background_push_task_init(){
    xTaskCreate(_golain_hal_p_log_background_push_task, "p_log_push_task", 4096, NULL, 5, NULL);
    return GOLAIN_OK;
}

void _golain_hal_p_log_background_push_task(void *pvParameters)
{
    uint8_t _golain_logs_buffer[CONFIG_GOLAIN_P_LOGS_BUFFER_SIZE];
    bool _golain_hal_p_log_is_pushing = false;
    while (1)
    {
        if (_golain_hal_p_log_is_pushing == false)
        {
            _golain_hal_p_log_is_pushing = true;
            memset(_golain_logs_buffer, 0, CONFIG_GOLAIN_P_LOGS_BUFFER_SIZE);
            _golain_hal_p_log_read_old_logs(_golain_logs_buffer);
            _golain_hal_p_log_is_pushing = false;
        }
        vTaskDelay(CONFIG_GOLAIN_P_LOGS_FLUSH_INTERVAL*(60*1000) / portTICK_PERIOD_MS);
    }
}

golain_err_t _golain_hal_p_log_check_nvs_errors(esp_err_t err)
{
    switch (err)
    {
    case ESP_OK:
        printf("Done\n");
        return GOLAIN_OK;
    case ESP_ERR_NVS_NOT_FOUND:
        printf("The value is not initialized yet!\n");
        return NVS_READ_FAIL;
    default:
        printf("Error (%s) reading!\n", esp_err_to_name(err));
        return GOLAIN_FAIL;
    }
}

golain_err_t _golain_hal_p_log_write_to_nvs(uint8_t *data, size_t len)
{
    // open nvs_flash in readwrite mode
    esp_err_t err;
    nvs_handle_t p_log_handle;
    err = nvs_open("p_logs", NVS_READWRITE, &p_log_handle);
    if (err != ESP_OK)
    {
        return GOLAIN_FAIL;
    }

    // get last log id
    int32_t last_log_id;
    if (nvs_get_i32(p_log_handle, "last_log_id", &last_log_id) ==
        ESP_ERR_NVS_NOT_FOUND)
    {
        last_log_id = 0;
    }

    if (last_log_id == CONFIG_GOLAIN_MAX_PERSISTENT_LOGS)
        last_log_id = 0;
    else
        last_log_id++;

    // write to nvs flash
    char key[2];
    #if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
        sprintf(key, "%ld", last_log_id);
    #else
        sprintf(key, "%d", last_log_id);
    #endif
    err = nvs_set_blob(p_log_handle, key, data, len);
    if (err != ESP_OK)
    {
        return NVS_UPDATE_FAIL;
    }

    // store current log id
    nvs_set_i32(p_log_handle, "last_log_id", last_log_id);

    // commit changes
    err = nvs_commit(p_log_handle);
    if (err != ESP_OK)
    {
        return NVS_UPDATE_FAIL;
    }
    nvs_close(p_log_handle);
#if CONFIG_PERSISTENT_LOGS_INTERNAL_LOGLEVEL > 2
    #if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
        ESP_LOGI(TAG, "Wrote to NVS: PLogID:%ld", last_log_id);
    #else 
        ESP_LOGI(TAG, "Wrote to NVS: PLogID:%d", last_log_id);
    #endif
#endif
    return GOLAIN_OK;
}

golain_err_t _golain_hal_p_log_write(esp_log_level_t level, const char *func,
                      const char *tag, const char *format, ...)
{
    // create log entry
    PLog plog = PLog_init_default;
    plog.level = (Level)level;

    plog.tag.funcs.encode = &golain_pb_encode_string;
    plog.tag.arg = (void *)tag;

    plog.function.funcs.encode = &golain_pb_encode_string;
    plog.function.arg = (void *)func;

    // get variable arguments
    va_list list;
    va_start(list, format);

    /*
     * buffer here is used to create the string log message as well as
     * store the encoded protobuf message
     */
    uint8_t buffer[256];
    memset(buffer, 0, 256);
    vsprintf((char *)buffer, format, list);
    // vprintf(format, list);
    va_end(list);

    plog.message.funcs.encode = &golain_pb_encode_string;
    plog.message.arg = buffer;

    plog.time_ms = esp_log_timestamp();

    // create output stream
    uint8_t out_buffer[256];
    pb_ostream_t ostream = pb_ostream_from_buffer(out_buffer, sizeof(buffer));
    pb_encode(&ostream, PLog_fields, &plog);

    // write to nvs flash
    esp_err_t err = _golain_hal_p_log_write_to_nvs(out_buffer, ostream.bytes_written);
    if (err != ESP_OK)
    {
        ESP_LOGE("PERSISTENT_LOGS", "Error writing to NVS (%s)",
                 esp_err_to_name(err));
    }
    if (_golain_p_log_send_mqtt_message(out_buffer, ostream.bytes_written) != GOLAIN_OK)
    {
        ESP_LOGE("PERSISTENT_LOGS", "Error sending MQTT message");
    }
    // clear buffer
    memset(out_buffer, 0, 256);

    // return error code
    if (err != ESP_OK)
    {
        ESP_LOGE("PERSISTENT_LOGS", "Error writing to NVS (%s)",
                 esp_err_to_name(err));
        return GOLAIN_FAIL;
    }
    return GOLAIN_OK;
}

golain_err_t _golain_hal_p_log_read_old_logs(uint8_t *buffer)
{
    esp_err_t err;
    nvs_handle_t p_log_handle;
    err = nvs_open("p_logs", NVS_READWRITE, &p_log_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error opening NVS (%s)", esp_err_to_name(err));
        return NVS_READ_FAIL;
    }
    for (uint8_t i = 0; i <= CONFIG_GOLAIN_MAX_PERSISTENT_LOGS; i++)
    {
        char key[4];
        sprintf(key, "%d", i);

#if CONFIG_PERSISTENT_LOGS_INTERNAL_LOGLEVEL > 2
        ESP_LOGI(TAG, "Reading NVS key: %s", key);
#endif
        // get size of stored BLOB
        size_t len;
        err = nvs_get_blob(p_log_handle, key, NULL, &len);

#if CONFIG_PERSISTENT_LOGS_INTERNAL_LOGLEVEL > 2
        ESP_LOGI(TAG, "Size of stored data: %d", len);
#endif

        if (len < 1)
        {
#if CONFIG_PERSISTENT_LOGS_INTERNAL_LOGLEVEL > 2
            ESP_LOGI(TAG, "Nothing stored here.");
#endif
            continue;
        }
        // get stored BLOB
        err = nvs_get_blob(p_log_handle, key, buffer, &len);
        if (err != ESP_OK)
        {
            // if there is an error, remove the value
            ESP_LOGE(TAG, "Error reading NVS (%s)", esp_err_to_name(err));
            nvs_erase_key(p_log_handle, key);
            continue;
        }
#if CONFIG_PERSISTENT_LOGS_INTERNAL_LOGLEVEL > 2
        ESP_LOGI(TAG, "Read from NVS: PLogID:%d : %s", i, (char *)buffer);
#endif
        _golain_p_log_send_mqtt_message(buffer, len);
        if (nvs_erase_key(p_log_handle, key) != ESP_OK)
        {
            ESP_LOGE(TAG, "Error erasing NVS PLog Key %s: (%s)", key, esp_err_to_name(err));
            continue;
        }
    }
    memset(buffer, 0, 256);
    nvs_close(p_log_handle);
    return GOLAIN_OK;
}

golain_err_t _golain_hal_p_log_get_number_of_logs(int32_t *num)
{
    esp_err_t err;
    nvs_handle_t p_log_handle;
    err = nvs_open("p_logs", NVS_READWRITE, &p_log_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error opening NVS (%s)", esp_err_to_name(err));
        return NVS_READ_FAIL;
    }

    return (golain_err_t)nvs_get_i32(p_log_handle, "last_log_id", num);
}
#endif // CONFIG_GOLAIN_PERSISTENT_LOGS

#endif // _GOLAIN_LOGS_H