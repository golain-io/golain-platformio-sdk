#ifndef _GOLAIN_TYPES_H
#define _GOLAIN_TYPES_H

typedef enum _golain_err_t
{
    GOLAIN_OK = 1,    /*!< esp_err_t value indicating success (no error) */
    GOLAIN_FAIL = -1, /*!< Generic esp_err_t code indicating failure */
    NVS_NOT_INIT = 0x0001, 
    NVS_UPDATE_FAIL = 0x0002,
    NVS_READ_FAIL = 0x0003,
    NVS_EMPTY = 0x0004,
    PB_UPDATE_FAIL =  0x0005,
    PB_ENCODE_FAIL = 0x0006,


    GOLAIN_ERR_NO_MEM = 0x1001,           /*!< Out of memory */
    GOLAIN_ERR_INVALID_ARG = 0x1002,      /*!< Invalid argument */
    GOLAIN_ERR_INVALID_STATE = 0x1003,    /*!< Invalid state */
    GOLAIN_ERR_INVALID_SIZE = 0x1004,     /*!< Invalid size */

    GOLAIN_ERR_NOT_FOUND = 0x1005,        /*!< Requested resource not found */
    GOLAIN_ERR_NOT_SUPPORTED = 0x1006,    /*!< Operation or feature not supported */
    GOLAIN_ERR_TIMEOUT = 0x1007,          /*!< Operation timed out */
    GOLAIN_ERR_INVALID_RESPONSE = 0x1008, /*!< Received response was invalid */
    GOLAIN_ERR_INVALID_CRC = 0x1009,      /*!< CRC or checksum was invalid */
    GOLAIN_ERR_NOT_FINISHED = 0x100C,     /*!< There are items remained to retrieve */
    
    GOLAIN_ERR_INVALID_VERSION = 0x100A,  /*!< Version was invalid */
    GOLAIN_ERR_INVALID_MAC = 0x100B,      /*!< MAC address was invalid */

    GOLAIN_ERR_WIFI_BASE = 0x2000,      /*!< Starting number of WiFi error codes */
    GOLAIN_ERR_MESH_BASE = 0x3000,      /*!< Starting number of BLE error codes */
    GOLAIN_ERR_FLASH_BASE = 0x4000,     /*!< Starting number of NVS error codes */
    
    GOLAIN_MQTT_CONNECT_FAIL = 0x5001,
    GOLAIN_MQTT_DISCONNECT_FAIL = 0x5002,
    GOLAIN_MQTT_SUBSCRIBE_FAIL = 0x5003,
    GOLAIN_MQTT_PUBLISH_FAIL = 0x5004,
    GOLAIN_MQTT_UNSUBSCRIBE_FAIL = 0x5005,

} golain_err_t;

#endif // _GOLAIN_TYPES_H