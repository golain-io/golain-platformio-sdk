#ifdef GOLAIN_DATA_POINT_ENABLED
#define GOLAIN_DATA_POINT_ENABLED
#pragma once
#include <golain_clients.h>
#include <golain_config.h>
#include <Arduino.h>
#include <string>
#include <pb_encode.h>
#include <ext_hashmap.h>

#ifndef CONFIG_GOLAIN_DATA_BUFFER_MAX_SIZE
#define CONFIG_GOLAIN_DATA_BUFFER_MAX_SIZE 100
#endif

typedef struct _data_point_info
{
    char *dp_name;
    pb_msgdesc_t fields;
    size_t size;

} data_point_info;

void golain_register_data_point(char *data_point_name, pb_msgdesc_t data_point_fields, size_t size)
{
    data_point_info temp = {data_point_name, data_point_fields, size};
    insert(global_map,data_point_name,&temp);
}

void golain_deregister_data_point(char *data_point_name, pb_msgdesc_t data_point_fields, size_t size)
{
    data_point_info temp = {data_point_name, data_point_fields, size};
    deleteNode(global_map,data_point_name);
}

uint8_t device_data_buffer[CONFIG_GOLAIN_DATA_BUFFER_MAX_SIZE];


void golain_post_device_data( char* dp_name,void *device_data_struct)
{
    const data_point_info* temp = (const data_point_info*)get(global_map,dp_name);
    if(temp == NULL){
        Serial.println("Did not find data point");
        return;
    }

    bool status;
    pb_ostream_t stream = pb_ostream_from_buffer(device_data_buffer, temp->size);

    status = pb_encode(&stream, &(temp->fields), device_data_struct);

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

#endif