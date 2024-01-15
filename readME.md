# golain-iot-sdk
C++ SDK for Golain IoT Platform based on the Arduino framework.
- Minimal setup to get up and running with Golain.
- Support for WiFi, GSM, 4G, LTE, LoRaWAN, NB-IoT via TinyGSM for communication.
- Post Device Data, update Device Shadow, recieve OTA Updates, monitor device logs, and more.

## Features
| Feature | Status |
| --- | --- |
| Device Shadow | :heavy_check_mark: |
| Device Logs | :heavy_check_mark: |
| Device Data | :heavy_check_mark: |
| User Association | :heavy_check_mark: |
| OTA | :heavy_check_mark: |
| BLE | `alpha` |
| Embedded RPC | :x: |


## Works with / Continous Testing done on -
| Hardware                | Status    | Tested |
| ---                     | ---       | --- |
| ESP32-WROOM32-D         | `stable`  |:heavy_check_mark:  |
| ESP32-C3                | `stable`  | :heavy_check_mark: |
| ESP32-S3                | `stable`  | :heavy_check_mark: |
| ESP32-C6                | `stable`  | :heavy_check_mark: |
| nRF52840-DK + SIM7600E  | `dev`     | :heavy_check_mark: |
| nRF52832-DK + SIM800L   | `dev`     | :heavy_check_mark: |
| nRF9160                 | `dev`     | :heavy_check_mark: |

See Example Projects for more details.
# Getting Started


### Prerequisites - 
1. VS Code - 
    Download and install VS Code from [here](https://code.visualstudio.com/download).
2. PlatformIO extension for VS Code - Install the PlatformIO extension for VS Code from [here](https://platformio.org/install/ide?install=vscode).
3. ProtoCompiler - 
    `protoc` is required for golain sdks to work and should be available in your `PATH`.
    - Windows - Download and install from [here]()
    - Linux - 
        - Debian/Ubuntu -
        ```bash
        sudo apt install protobuf-compiler
        ```
        - Fedora -
        ```bash
        sudo dnf install protobuf-compiler
        ```
        - Arch -
        ```bash
        sudo pacman -S protobuf
        ```
    - MacOS - 
        - via Homebrew
        ```bash
        brew install protobuf
        ```
        - via MacPorts
        ```bash
        sudo port install protobuf3-cpp
        ```

### Setup -
1. Create a new PlatformIO project - 
    - Open VS Code and click on the PlatformIO icon on the left sidebar.
    - Click on `New Project` and select your board.
    - Select a project name and click on `Finish`.
2. Add golain-platformio-sdk as a dependency -
    - Open `platformio.ini` file in your project.
    - Add the following line to the end of the file - 
    ```ini
    lib_deps = golain-platformio-sdk
    ```
    - Save the file.
3. Get device credentials from [Golain Console](web.golain.io) - 
    - Login to Golain Console and navigate to `Devices` section.
    - Click on `Add Device` and follow the instructions.
    - Download the PlatformIO `certs.h` file and place it in your project's `include` folder.