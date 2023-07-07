#ifdef GOLAIN_DEVICE_FLAG_ENABLED
#ifndef FLAGS_H
#define FLAGS_H
#include <Arduino.h>

int64_t status_flag_bit = 0;

#define DEVICE_FLAG_SET "lol"
#define DEVICE FLAG_RESET "lol1"
#define DEVICE_FLAG_TOGGLE "sgr"
#define DEVICE_FLAG_SUB "sdsd"

void set_flag(int num)
{
  status_flag_bit |= (1 << num);
}

void reset_flag(int num)
{
  status_flag_bit &= ~(1 << num);
}

bool check_flag(int num)
{
  return (status_flag_bit << num);
}

void toggle_flag(int num)
{
  if (check_flag(num) == true)
  {
    set_flag(num);
  }
  else
  {
    reset_flag(num);
  }
}





#endif
#endif