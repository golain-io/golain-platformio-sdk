#pragma once
#include <golain_clients.h>
#include <golain_config.h>
#include <Arduino.h>
#include <string>
#include <pb_encode>


typedef struct _data_point_info
{
    char *dp_name;
    pb_msgdesc_t fields;
    size_t size;
    //   void (*data);

} data_point_info;

void golain_register_data_point(char *data_point_name, pb_msgdesc_t data_point_fields, size_t size)
{
    data_point_info temp = {data_point_name, data_point_fields, size};
    insert(&global_map,data_point_name,&temp);
}

uint8_t device_data_buffer[CONFIG_GOLAIN_DATA_BUFFER_MAX_SIZE];


void golain_post_device_data(void *device_data_struct, char* dp_name)
{
    data_point_info temp = get(&global_map,dp_name);
    if(temp == NULL){
        Serial.println("Did not find data point");
        return;
    }

    bool status;
    pb_ostream_t stream = pb_ostream_from_buffer(device_data_buffer, temp.size);

    status = pb_encode(&stream, temp.field, device_data_struct);

    if (!status)
    {
        Serial.printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
        return;
    }
    else
    {
        Serial.printf("Encoding successful\n");
    }
    String tempe = String(DEVICE_DATA_TOPIC) + String(dp_name);
    client.publish((tempe).c_str(), (const char *)device_data_buffer, stream.bytes_written - 1);
}
