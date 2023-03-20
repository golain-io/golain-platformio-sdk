#include <Arduino.h>
#include "constants.h"
#include "deviceHealth.h"
#include "constants.h"
void setup() {
  // put your setup code here, to run once:
  esp_reset_reason();
}


esp_err_t nvs_init(){
    

   esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    return err;
}

void loop() {
    uint8_t buffer[protobufSize];

    nvs_init();
    esp_err_t err ;
    
   
    size_t message_length;

    /* Encode our message */
    if (!encode_message(buffer, sizeof(buffer), &message_length))
    {
        return;
    }

    err  = deviceHealthStore(buffer);
    if (err == ESP_OK){
        ESP_LOGI("main","device health Stored successfully");
    }
    else{
    ESP_LOGI("main","health store %s",esp_err_to_name(err));
    }

    /* But because we are lazy, we will just decode it immediately. */
    decode_message(buffer, message_length);

}