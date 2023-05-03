

# Persistent Logs Documentation  
&nbsp;
  
### Macros
  
- CONFIG_MAX_PERSISTENT_LOGS  
    Defines maximum number of logs that can be stored on device.  
    Default value is 50.  
   #include <persistent_logs.h>
  
- P_LOG_I  
  #define P_LOG_I ( TAG, MESSAGE )  
  Writes an Info level log to uart, stores the content of log in a protobuf file and store it in NVS.
  #include <persistent_logs.h>  
  ### Parameters - 
  - Tag - A string, which is used to trace the origin of log.   
  - Message - A string which contains your info log message.  
  &nbsp;
- P_LOG_W  
  #define P_LOG_W ( TAG, MESSAGE ) 
  #include <persistent_logs.h>  
  Writes an Warning level log to uart, stores the content of log in a protobuf file and store it in NVS.
  ### Parameters - 
  - Tag - A string, which is used to trace the origin of log.   
  - Message - A string which contains your warning log message.  
  &nbsp;
- P_LOG_E  
  #define P_LOG_E ( TAG, MESSAGE )  
  #include <persistent_logs.h>  
  Writes an Error level log to uart, stores the content of log in a protobuf file and store it in NVS.
  ### Parameters - 
  - Tag - A string, which is used to trace the origin of log.   
  - Message - A string which contains your Error log message.  
  &nbsp;

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



