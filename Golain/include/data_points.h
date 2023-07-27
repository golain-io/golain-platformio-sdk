#pragma once
#include <golain_clients.h>
#include <golain_config.h>
#include <iostream>
#include <unordered_map>
#include <Arduino.h>
#include <string>

typedef struct _dp_info{
    size_t max_size;
    pb_msgdesc_t message_fields;

} data_point_info;
std::unordered_map<std::string, data_point_info> dpmap;

void golain_register_data_point(String dp_name, const pb_msgdesc_t *message_fields, size_t max_message_size)
{
    dp_map[dp_name] = {max_message_size, &message_fields};
}

uint8_t device_data_buffer[CONFIG_GOLAIN_DATA_BUFFER_MAX_SIZE];

void golain_post_device_data(void *device_data_struct, std:string dp_name)
{
    bool status;
    pb_ostream_t stream = pb_ostream_from_buffer(device_data_buffer, dpmap[dp_name].max_size);

    status = pb_encode(&stream, dpmap[dp_name].message_fields, device_data_struct);

    if (!status)
    {
        Serial.printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
        return;
    }
    else
    {
        Serial.printf("Encoding successful\n");
    }
    String temp = String(DEVICE_DATA_TOPIC) + String(topic);
    client.publish((temp).c_str(), (const char *)device_data_buffer, stream.bytes_written - 1);
}
