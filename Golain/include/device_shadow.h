#pragma once
#include <golain_clients.h>
#include <golain_config.h>

#ifdef GOLAIN_DEVICE_SHADOW_ENABLED
#include <shadow.pb.h>
void Internal_callback(char *topic, byte *payload, unsigned int length);
void (*user_shadow_callback_ptr)() = NULL;

Shadow global_shadow = Shadow_init_zero;

uint8_t receive_buf[Shadow_size];
uint8_t transmit_buf[Shadow_size];
bool golain_shadow_set()

{
    bool status;

    pb_ostream_t stream = pb_ostream_from_buffer(transmit_buf, Shadow_size);

    status = pb_encode(&stream, Shadow_fields, &global_shadow);
    int temp = stream.bytes_written;
    // Serial.printf("Encoded length %d\n",temp);
    if (!status)
    {
        // Serial.printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
        return false;
    }
    else
    {
        // Serial.printf("Encoding successful\n");
        return client.publish(DEVICE_SHADOW_TOPIC_U, transmit_buf, temp);
    }
}

void golain_shadow_get(uint8_t *buffer, size_t message_length)
{

    bool status;
    pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);

    status = pb_decode(&stream, Shadow_fields, &global_shadow);

    // if (!status)
    // {
    //     Serial.printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
    //     return;
    // }
    // else
    // {
    //     Serial.println("Decoded Successfully");
    // }
    // user_shadow_callback();
    if (user_shadow_callback_ptr != NULL && status)
    {
        user_shadow_callback_ptr();
    }

    else
    {
        Serial.println("Decoding failed");
    }
}
