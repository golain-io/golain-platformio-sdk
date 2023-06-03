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
- Fill in the given 3 certificates in `mqtt_config.h`.
- Put your root topic mentioned in `connections_settings.json` in the `CONFIG_ROOT_TOPIC` macro.
- Setup Internet connection either using WiFi of Ethernet.
- Once Connected to the internet, Connect to the Golain MQTT broker using `mqtt_app_start()` api.
- Use other APIs such as `post_device_shadow()` or `post_device_data_points` as per your business logic.

## Availble Macros  
 
### For setup
- `CONFIG_MQTT_SERVER`  
    This macro lets user put the url of MQTT broker, to which they want to connect. By default this macro will expand to `dev.golain.io` .  
-  `CONFIG_MQTT_PORT`  
    This macro lets user put the port, through which they want to connect to the broker. By default this macro will expand to `8083` .
- `CONFIG_CLIENT_ID`  
    This macro lets user add their unique client id which is mentioned in the `connection_settings.json` file, downloaded at the time of device creation. In order to have a successful connection, one has to put correct client id a in string format.  
- `CONFIG_TOPIC_ROOT`  
This macro lets user add their unique topic root which is mentioned in the `connection_settings.json` file, downloaded at the time of device creation. This parameter helps Golain backend identify the device.
    
### Constants - 
- `static const char AWS_CERT_CA[] `  
    This is an array inside `mqtt_config.h` which holds the CA certificate of the MQTT broker. Open the root certificate, from the certificate zip file, you downloaded while device creation. Make sure to change contents only between `-----BEGIN CERTIFICATE-----` and `-----END CERTIFICATE-----` .This array is stored in the flash memory, thereby saving the RAM available.  

- `static const char AWS_CERT_CRT[] `  
    This is an array inside `mqtt_config.h` which holds the device certificate of the created device. Open the device certificate, from the certificate zip file, you downloaded while device creation. Make sure to change contents only between `-----BEGIN CERTIFICATE-----` and `-----END CERTIFICATE-----` .This array is stored in the flash memory, thereby saving the RAM available.  

- `static const char AWS_CERT_PRIVATE[] `  
    This is an array inside `mqtt_config.h` which holds the private key of the created device. Open the device private key, from the certificate zip file, you downloaded while device creation. Make sure to change contents only between `-----BEGIN CERTIFICATE-----` and `-----END CERTIFICATE-----` .This array is stored in the flash memory, thereby saving the RAM available.  
    

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


     