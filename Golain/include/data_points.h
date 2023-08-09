#pragma once
#include <golain_clients.h>
#include <golain_config.h>
#include <Arduino.h>
#include <string>

typedef struct _data_point_info{
  String dp_name;  
  String fields;
  size_t size;
  void (*data);

}data_point_info;




uint8_t device_data_buffer[CONFIG_GOLAIN_DATA_BUFFER_MAX_SIZE];

void golain_post_device_data(void *device_data_struct, String dp_name)
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
