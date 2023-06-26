# MQTT

## Overview
This is an MQTT secured component for Golain backend services. Users can connect to this broker using the generated certificates on Golain website and use it to send messages to their remote devices.This component is based on pubsubclient library. 

## Usage
MQTT can be used to send send and receive messages from and to the device. In order to use this component, user has to go through following steps.  
- Create a new account on Golain website.
- Create a fleet of devices.
- Create a device in that fleet.
- Select the pre-existing shadow or define one by yourself.
- Select datapoints.
- Select and attach a policy to your device.
- Download certificate chain zip for your newly crearted device.
- Extract the zip file and save somewhere, because you will not be able to download it again.
- Create `golain_config` struct.
- Create build flags called `TOPIC_ROOT` and `DEVICE_NAME` in platformio.ini file. Syntax is shown below.
```python
build_flags = -D TOPIC_ROOT='"<Your Root Topic>"'
              -D DEVICE_NAME='"<Your Device Name>"'
```
- Add build flags to enable device shadow and device data points as follows
```python
build_flags = -D GOLAIN_DEVICE_SHADOW_ENABLED  
            # Enables Device shadow feature
              -D GOLAIN_P_LOGS_ENABLED  
            # Enables Persistant logs  
              -D GOLAIN_DEVICE_HEALTH_ENABLED
            # Enables Device Health
              -D GOLAIN_DATA_POINT_ENABLED
            # Enables Device Data points.
```
Each build flag/macro enables a certain feature.  
If you dont need a feature, simply dont include the macro. It will reduce memory footprint of your firmware.

- Define your Shadow.proto file inside src folder inside your project. Make sure you put same proto defination here, which you put in while creating device using our website.

- Mention these two lines in platformio.init file to compile your .proto files. If you want to use device shadow feature, make sure to use name of message as `Shadow`.
```python
custom_nanopb_protos =
    +<src/*.proto> #// add source to your .protofile 
custom_nanopb_options =
    --error-on-unmatched
```
- If you intend to use device shadow functionality, please define a callback named `user_shadow_callback()` and implement the business logic using values in `global_shadow` struct. Note that this callback will be called after every remote shadow update.
- Connect to the internet either using WiFi or Ethernet. 
- Fill in the members of the struct viz. CA Certificate, Device Certificate, Device private key, Callback , Root topic, Client id either directly or using the given APIs.
- Connect to the golain backend using `golain_init()` API. Put your correctly filled struct in this API.
- If you want to update and publish the shadow, use `global_shadow` struct to update it, then use `golain_shadow_set` API to set the shadow locally and encode it in protocol buffer format. The encoded protocol buffer file will be stored in `uint8_t* buffer`, which is a paramter of  `golain_shadow_set` function. It will also give you encoded message length. After calling this function, post the shadow using `post_shadow()` function. Your updated shadow will be reflected on the platform.




### Functions 

- `void mqtt_connect()`  
    This function connects to the Golain MQTT broker using the credentials provided in the form of macros. Note that, it is a blocking api, meaning it would block the CPU untill it connects to the Golain MQTT broker.

- `void callback(char *topic, byte *payload, unsigned int length)`  
    This function lets user interact with the message arrived from the topics, to which they are subscribed. User can write their business logic by comparing the data to a predefined string using `strcmp()` api.  
    Parameters -  
    `topic`: A String (char*), which tells user, the topic from which the message has arrived.  
    `payload`: A byte, which gives user actual data to be received.  
    `length`: An unsigned int, which gives user the total length of incoming data.   
     


- `void postshadow(uint8_t *data, int len)`  
    This function lets user post a shadow in form of a proto file to the golain cloud. The user's data shall be encoded using protobuf standard, which will be an array of uint8_t. User can pass a this array and its length to this function, in order to transmit shadow to the Golain backend.  
    Parameters -  
    `data`: ptr to uint8_t, this is the encoded shadow in protobuf format.  
    `len`: This is the total length of encoded shadow, Please make sure to add correct length of the encoded shadow. In case of incorrect length, Backend will have problem while decoding it.  

- `void postData(char *data, size_t length, char *topic)`  
    This function lets user send their own data to a certain topic using golain MQTT broker.  
    Parameters -  
    `data`: A ptr to char. This is actual data to be sent via MQTT.  
    `length`: size_t. This is length of the message that has to be sent over MQTT.  
    `topic`: A ptr to char. This is the topic to which the data has to be sent.  

- `golain_init(golain_config *clientt)`   
    Description  -  
    The golain_init function initializes the client configuration and establishes a connection to the MQTT broker for the Golain IoT framework. It also subscribes to the device shadow topic if the GOLAIN_DEVICE_SHADOW_ENABLED flag is defined.  

    Syntax -  
    ```c
     golain_init(golain_config *clientt);
    ```
    Parameters -  
    clientt (golain_config*): A pointer to the client configuration structure.  

    Usage -  
    The golain_init function should be called once, typically during the setup phase of an IoT application using the Golain framework.  

    Configuration Parameters -  

    The clientt parameter is a pointer to a golain_config structure that contains the following configuration parameters:

    `ca_cert (const char*)`: The Certificate Authority (CA) certificate for establishing a secure connection to the MQTT broker.  

    `device_cert (const char*)`: The device certificate for authentication with the MQTT broker.  

    `device_pvt_key (const char*)`: The private key corresponding to the device certificate.  

    `client_id (const char*)`: The unique client identifier for the MQTT connection.  

    `root_topic (const char*)`: The root topic to which the device will publish and subscribe.


    Return Value:  

    This function does not return any value.  

    Example - 
    ```c
    // Define and initialize the golain_config structure
    golain_config client_config;
    client_config.ca_cert = "path/to/ca_cert.pem";
    client_config.device_cert = "path/to/device_cert.pem";
    client_config.device_pvt_key = "path/to/device_pvt_key.pem";
    client_config.client_id = "your_device_name";
    client_config.root_topic = "your _root_topic";

    // Initialize the Golain client
    golain_init(&client_config);
    ```

- `golain_shadow_set(uint8_t *buffer, size_t *message_length)`:  
    This function encodes the global shadow struct into protobuf file, which is stored into `uint8_t* buffer` and the encoded length is given in the `*message_length` parameter.
    Update the global shadow struct with your desired values and call this function.  

