# Golain SDK Example

This example shows how to use Golain SDK to connect to Golain's Platform.


### Steps for configuration

1. Create a new Organization and Project on Golain's Platform.
2. Create a Fleet with a Shadow.
3. Create a Device in the Fleet.
4. Download the PlatformIO headers for the Device at the end of the creation process.
5. Copy the downloaded headers to the `include` folder of this example.
6. Download the proto files for the Shadow and any datapoints you want to use and copy them to the `src` folder of this example.
7. Add the following lines to the `platformio.ini` file:

```
build_flags = -D "MBEDTLS_KEY_EXCHANGE__SOME__PSK_ENABLED"
				-D CORE_DEBUG_LEVEL=3 ; Set to 0 for no debug, 1 for errors, 2 for warnings, 3 for info, 4 for debug, 5 for trace
			 -D ROOT_TOPIC='"/xxxx_xxxx/"' ; Replace with the root topics in the downloaded headers
			 -D DEVICE_NAME='"device-x"' ; Replace with the device name in the downloaded headers - ensure to keep the double quotes.

custom_nanopb_protos = 
	+<src/*.proto>
custom_nanopb_options = 
	--error-on-unmatched
```

### Compatibility

This example is compatible with any device that has atleast 256kb of flash and 128kb of RAM. 
Additionally, either WiFi/Ethernet or GSM Connecttivity is needed.
It has been tested on the following devices:

| Device | Status |
| --- | --- |
| ESP32 Wrover Kit | :heavy_check_mark: |
| ESP32 DevKitC | :heavy_check_mark: |
| ESP32-C3 | :heavy_check_mark: |
| ESP32-S3 | :heavy_check_mark: |
| ESP32-C6 | :heavy_check_mark: |

GSM Connectivity has been tested on the following devices:
| Device | Status |
| --- | --- |
| SIM800L | :heavy_check_mark: |
| SIM7600 | :heavy_check_mark: |
| SIM7600E | :heavy_check_mark: |
| SIM700x | :heavy_check_mark: |