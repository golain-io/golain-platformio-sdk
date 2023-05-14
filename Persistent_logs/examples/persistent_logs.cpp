#include <Arduino.h>
#include <persistent_logs.h>
// put function declarations here:
#define MAIN_TAG "MAIN"
int myFunction(int, int);

void setup()
{
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  Serial.begin(9600);
}

void loop()
{
  // put your main code here, to run repeatedly:
  P_LOG_I(MAIN_TAG, "These are Fake error logs");
  for (int i = 0; i < 19; i++)
  {
    P_LOG_E(MAIN_TAG, "This is error number " + String(i));
  }

  P_LOG_I(MAIN_TAG, "These are Fake debug logs");
  for (int i = 0; i < 19; i++)
  {
    P_LOG_W(MAIN_TAG, "This is warning number " + String(i));
  }

  P_LOG_I(MAIN_TAG, "These are Fake Info logs");
  for (int i = 0; i < 19; i++)
  {
    P_LOG_W(MAIN_TAG, "This is info number " + String(i));
  }
 delay(5000);
 read_old_logs();

}

// put function definitions here:
int myFunction(int x, int y)
{
  return x + y;
}