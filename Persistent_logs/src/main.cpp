#include <Arduino.h>
#include <Preferences.h>
#include <persistent_logs.h>
#include <nvs.h>

#define MAIN_TAG "main"
int16_t lol= 0;
int i = 45;
size_t len;
String ok = "hello there here is nvs";

char a[25];
void setup()
{
  Serial.begin(9600);
  custom_nvs_init();
}

void loop()
{
  


  P_LOG_I(MAIN_TAG,"hello there");
 
  delay(2000);
}