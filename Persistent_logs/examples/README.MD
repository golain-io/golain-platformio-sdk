# Persistent Logs Example

This examples shows how one can log and persist Error, Warning and Info messages in ESP32.

## Code Explained - 

```c
#include <Arduino.h>  
#include <persistent_logs.h>  
```

This snippet of code adds the essential header files for this examples.  
`Arduino.h` header file brings all the basic Arduino functionality, while `persistent_logs.h` brings the persistent logs capability.  

```c++
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
} 
```
This statement enables the Serial monitor at the baud rate of 9600. User can change the baud rate as per his/her application requirement.
### Note - Without this statement Logs will not be shown on the serial monitor, however they will still persist.  

```c++
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
  ```

  This snippet of code lies inside the `loop()` function, meaning this will run forver.

  Here, the `P_LOG_E` API logs the error statement, with given tag.
  Similarly, information and warning statements can be logged using `P_LOG_W` and `P_LOG_I`.  

  ```c
  delay(5000);
  ```
  This statement gives blocking delay of 5 seconds.

  ```c
 read_old_logs();
  ```
This statement reads all the logged logs and puts it to the Serial monitor. User can do anything to these persisted logs by going to the `persistent_logs.h` file.  
To go to this file, press CTRL and click on `persistent_logs.h` inclusion, which we included at the top of this file.  
After going to this file, go to the `read_old_logs()` function.  
It will look something like this.
```c
void read_old_logs()
{
    uint8_t buffer[256];
    size_t len;
    char key[2];
    esp_err_t err;
    int32_t last_log_id = 0;
    memset(buffer, 0, 256);
    err = nvs_open("p_logs", NVS_READWRITE, &p_log_handle);
    if (err != ESP_OK)
    {

        Serial.println("Error opening NVS " + String(esp_err_to_name(err)));
        return;
    }

    nvs_get_i32(p_log_handle, "last_log_id", &last_log_id);

    Serial.println("total " + String(last_log_id) + " logs have been logged");

    while (last_log_id != 0)
    {
        sprintf(key, "%d", last_log_id);

        nvs_get_blob(p_log_handle, key, buffer, &len);
        /*
        do whatever you want with the logs
        */
        Serial.printf("giving log id %d\n", last_log_id);
        // do what you want with logs
        for (int i = 0; i < len; i++)
        {
            Serial.print((char)buffer[i]);
        }
        Serial.println();

        memset(buffer, 0, 256); // clear the buffer
        last_log_id--;
    }
}
```
Inside the, while loop, do whatever you want with the buffer read from NVS. You can push this to Cloud using Golain's MQTT broker.
