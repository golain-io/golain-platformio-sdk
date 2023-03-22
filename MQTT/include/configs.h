#pragma once
#define CONFIG_BROKER_URI "mqtts://dev.golain.io"
#define CONFIG_MQTT_PORT 8083
#define CONFIG_MESSAGE_NAMES "shadow,end"
#define CONFIG_NUMBER_OF_MESSAGES 2
#define CONFIG_TOPIC_ROOT "<Insert root topic>"
#define CONFIG_DEVICE_NAME "<Insert device name>"
#define CONFIG_ESP_WIFI_SSID "FLBS"
#define CONFIG_ESP_WIFI_PASSWORD "hellowworld"
#define CONFIG_ESP_MAXIMUM_RETRY 5 
//     config BROKER_URI
//         string "Broker URL"
//         default "mqtts://dev.golain.io"
//         help
//             URL of an mqtt broker which this example connects to.

//     config MQTT_PORT
//         int "MQTT Port"
//         default 8083
//         help 
//             Port number for the mqtt broker

//     # config MESSAGE_NAMES
//     #     string "Protobuff message names"
//     #     default "shadow,end"
//     #     help
//     #         A comma separated list of you protobuf message names

//     # config NUMBER_OF_MESSAGES
//     #     int "Number of proto messages"
//     #     default 2
//     #     help
//     #         Number of protobuf messages

//     config TOPIC_ROOT   
//         string "Path to the root topic for your project"
//         default "<Insert root topic>"
        
//     config DEVICE_NAME
//         string "Name of your device"
//         default "<Insert device name>"
    
//     config ESP_WIFI_SSID
//                 string "WiFi SSID"
//                 default "myssid"
//                 help
//                     SSID (network name) for the example to connect to.
        
//     config ESP_WIFI_PASSWORD
//                 string "WiFi Password"
//                 default "mypassword"
//                 help
//                     WiFi password (WPA or WPA2) for the example to use.
        
//     config ESP_MAXIMUM_RETRY
//                 int "Maximum retry"
//                 default 5
//                 help
//                     Set the Maximum retry to avoid station reconnecting to the AP unlimited when the AP is really inexistent.
        
//             choice ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD
//                 prompt "WiFi Scan auth mode threshold"
//                 default ESP_WIFI_AUTH_WPA2_PSK
//                 help
//                     The weakest authmode to accept in the scan mode.
//                     This value defaults to ESP_WIFI_AUTH_WPA2_PSK incase password is present and ESP_WIFI_AUTH_OPEN is used.
//                     Please select ESP_WIFI_AUTH_WEP/ESP_WIFI_AUTH_WPA_PSK incase AP is operating in WEP/WPA mode.
        
//                 config ESP_WIFI_AUTH_OPEN
//                     bool "OPEN"
//                 config ESP_WIFI_AUTH_WEP
//                     bool "WEP"
//                 config ESP_WIFI_AUTH_WPA_PSK
//                     bool "WPA PSK"
//                 config ESP_WIFI_AUTH_WPA2_PSK
//                     bool "WPA2 PSK"
//                 config ESP_WIFI_AUTH_WPA_WPA2_PSK
//                     bool "WPA/WPA2 PSK"
//                 config ESP_WIFI_AUTH_WPA3_PSK
//                     bool "WPA3 PSK"
//                 config ESP_WIFI_AUTH_WPA2_WPA3_PSK
//                     bool "WPA2/WPA3 PSK"
//                 config ESP_WIFI_AUTH_WAPI_PSK
//                     bool "WAPI PSK"
//             endchoice

// endmenu