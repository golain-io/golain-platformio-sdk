# golain-platformio-sdk

## Features - 
- Provides two isolated communication planes between device and Golain Cloud.
- Supports MQTT secured and websockets.
- Enables Remote firmware updates with their digital signatures.
- Fully open-souce and maintained by Golain.
- Open to contributions from developers.  
## Works with  

| Hardware        | PlatformIO Version | Status | Tested |
| ---             | ---      | ---       | ---                |
| ESP32-WROOM32-D | `v3.1.1`   | `dev` | :x:                |
| ESP32-C3        | `v3.1.1` | `dev`     | :heavy_check_mark: |


# Getting Started

## Overview - 
This SDK consists of following components  
- [Persistent Logs](https://registry.platformio.org/libraries/shadowfighterca/Golain%20Persistent%20Logs/installation)
- [MQTT](https://registry.platformio.org/libraries/shadowfighterca/Golain%20MQTT/installation)  
- [OTA](https://registry.platformio.org/libraries/shadowfighterca/Golain%20OTA/installation)
- [Device Health](https://registry.platformio.org/libraries/shadowfighterca/Golain%20Device%20Health/installation)
- BLE Mesh (Coming Soon)

### Prerequisites - 
- VS Code -  
Use following links to Install VS Code on your system.  
   [VS Code for Linux](https://code.visualstudio.com/docs/setup/linux)  
   [VS Code for Windows](https://code.visualstudio.com/docs/setup/windows)  
   [VS Code for MacOS](https://code.visualstudio.com/docs/setup/mac)    

- PlatformIO extension -  
    Go to Extension section of VS code. Enter PlatformIO in the search bar. Click on the install button to install the extension to your VS Code.
    

### Steps to  get started -
- Make sure you have an active internet connection.
- Create a New platformIO project.
- Select Pllatform and Framework of your choice.
- Go to `platformio.ini` file of your newly created project.  
it will look something like this. 

    ```[env:seeed_xiao_esp32c3]  
    platform = espressif32  
    board = seeed_xiao_esp32c3
    framework = arduino 

- Add a section called `lib_deps` to include one of our components to your project. Links for each component are given in the bottom. Refer to this link, to know more about how to add libraries to platformio project.  
After adding the library, your `platformio.ini` file should look like this.  
    ```[env:seeed_xiao_esp32c3]
    platform = espressif32
    board = seeed_xiao_esp32c3
    framework = arduino
    lib_deps =
        shadowfighterca/Golain Persistent Logs@0.0.1

- Here, the line `shadowfighterca/Golain Persistent Logs@0.0.1` refers to Persistent logs component with version 0.0.1. Make sure to check this link to get our latest version of libraries.  
- After adding this line, save the project. PlatformIO will fetch the library you mentioned.
- The location of library will be `/.pio/build/lib_deps/[YOUR_PLATFORM]/[COMPONENT_YOU_MENTIONED]` .
- Many of our libraries need to be configured before used. User needs to enter their credentials such as Device certificates and keys inside the `config.h` file which is located at the path mentioned above. To know more about the configurations, check out documentation of each of these components.
- Include the header file mentioned in the documentation, and start using it as per your business logic.  

### PlatformIO lib_deps links -  
- MQTT - lib_deps= shadowfighterca/Golain MQTT@^0.0.1 
- OTA - lib_deps= shadowfighterca/Golain OTA@^0.0.1
- Persistent logs - lib_deps= shadowfighterca/Golain Persistent Logs@^0.0.2
- Device Health - lib_deps= shadowfighterca/Golain Device Health@^0.0.2