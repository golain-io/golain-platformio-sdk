#include <Arduino.h>
#include "persistent_logs.h"
#define MAIN_TAG "main"

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop()
{
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 25; i++)
  {
    P_LOG_E(MAIN_TAG, "%d\n", i);
  }
}