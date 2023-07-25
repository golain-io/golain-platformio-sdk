#pragma once
#include <golain_clients.h>
#include <golain_config.h>

#ifdef GOLAIN_DATA_POINT_ENABLED
uint8_t device_data_buffer[CONFIG_GOLAIN_DATA_BUFFER_MAX_SIZE];

void golain_post_device_data(void *device_data_struct, const pb_msgdesc_t *message_fields, char *topic, size_t message_size)
{
    bool status;
    pb_ostream_t stream = pb_ostream_from_buffer(device_data_buffer, message_size);

    status = pb_encode(&stream, message_fields, device_data_struct);

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

#endif

