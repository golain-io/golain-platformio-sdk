#ifndef NVS_H
#define NVS_H

#include "nvs_flash.h"
#include "nvs.h"
#include <Arduino.h>

 nvs_handle_t p_log_handle;
 
bool custom_nvs_init()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {

        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    if (err == ESP_OK)
    {
        return true;
    }
    else
    {
        return false;
    }
}

#endif