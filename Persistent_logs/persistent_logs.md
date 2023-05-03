# Persistent Logs

## Overview
This is a set of convinient macros and NVS fucntions that allow you to write logs to a non-volatile store & also print them to console.
These Macros are simply wrappers around the ESP-IDF LOG marcos.
The logs are serialised using nanopb (protocol buffers) and are stored in a byte-seriliased format to memory.
Retriving the logs from memory will deserialise them back to the original structure. The proto file used to serialise them is also included as a reference.

## Usage
These are meant as a conviniet way to apply tracing / reset logging for an embedded application.
For example:
- To tell and persist the last reason for a device reset.
- To diagnose errors by collecting and then batch pushing error logs to a backend (Golain's IoT Platform has support for this built-in)
- Diagnostic code and information that may be used in production

## Availble Macros
 
### For setup
- CONFIG_MAX_PERSISTENT_LOGS  
    Defines maximum number of logs that can be stored on device.  
    Default value is 50.
  
### Usage

#### `P_LOG_I(tag, message)`    
Writes an Info level log to uart, stores the content of log in a proto-encoded format and stores it in NVS.
*Parameters*
- `tag`: A string (char*). User definable, used to tag different subsystems within an application   
- `message`: A string (char*). Contains your info log message.  


#### `P_LOG_W(tag, message)`    
Writes an Warning level log to uart, stores the content of log in a proto-encoded format and stores it in NVS.
*Parameters*
- `tag`: A string (char*). User definable, used to tag different subsystems within an application   
- `message`: A string (char*). Contains your info log message.  


#### `P_LOG_E(tag, message)`    
Writes an Error level log to uart, stores the content of log in a proto-encoded format and stores it in NVS.
*Parameters*
- `tag`: A string (char*). User definable, used to tag different subsystems within an application   
- `message`: A string (char*). Contains your info log message.  



### Functions  

- read_old_logs()  
    #include <persistent_logs.h>  
    This function lets you read the persistent logs that have been stored in NVS so far.  
    Write your business logic i.e. what you want to do with the old logs inside this function as follows  
            

        nvs_get_blob(p_log_handle, key, buffer, &len);
        /*
        do whatever you want with the logs
        */

        memset(buffer, 0, 256); // clear the buffer`
    
    Persistent log will be stored in the third parameter of `nvs_get_blog` i.e. buffer. One can either print the old log on to the console or push it to the server using Golain's MQTT backend.


