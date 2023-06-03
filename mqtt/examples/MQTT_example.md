# Golain MQTT

This example lets you connect to golain MQTT broker with the credentials you received while creating a device.  

### Steps for configuration

- First of All go to the `.pio/lib_deps/<your microcontroller>/<Golain_MQTT>` path. This is where the library is located.
- Here, go to `include` directory and open the `mqtt_config.h` file.
- Change only DEVICE_ID, DEVICE_CERT and DEVICE_PVT_KEY parameters. These parameters are given to you when you create your device in a zip file.  


### Code Explained - 

```c
#include <WiFi.h>
#include "mqtt_helper.h"
```
This snippet of code includes the built in WiFi component and Golain's MQTT component.  

```c
const char* ssid = "foo";
const char* password = "bar";
```
Here you set the SSID and Password of your WiFi.

```c
Serial.begin(9600);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  mqtt_connect();
  ```
  Here, We start the Serial monitor at a baud rate of 9600.  
  Then we connect to the WiFi using the given credentials. then we connect to the Golain MQTT backend using `mqtt_connect()` API.

  ```c
  client.loop();
  ```
  This line of code keeps your devices constantly in contact with our MQTT broker.  
  After setting this up, one can write their business logic in `loop()` function.