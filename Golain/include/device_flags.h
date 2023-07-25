#pragma once
#include <golain_clients.h>
#include <golain_config.h>

#ifdef GOLAIN_DEVICE_FLAG_ENABLED

void (*user_flag_callback)() = NULL;

void golain_set_flag(int num)
{
    status_flag_bit |= (1 << num);
    return client.publish(DEVICE_FLAG_SET, String(num));
}

void golain_reset_flag(int num)
{
    status_flag_bit &= ~(1 << num);
    return client.publish(DEVICE_FLAG_RESET, String(num));
}

bool golain_check_flag(int num)
{
    return (status_flag_bit << num);
}

void golain_toggle_flag(int num)
{
    if (golain_check_flag(num) == true)
    {
        golain_set_flag(num);
    }
    else
    {
        golain_reset_flag(num);
    }
    return client.publish(DEVICE_FLAG_TOGGLE, String(num));
}
#endif