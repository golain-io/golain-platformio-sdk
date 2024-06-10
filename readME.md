# Golain PlatformIO SDK

## Introduction

The Golain PlatformIO SDK is a C++ Software Development Kit designed for seamless integration with the Golain IoT 
Platform. This SDK leverages the PlatformIO framework to facilitate quick and efficient setup, enabling developers to 
connect their IoT devices using various communication methods. The SDK supports Wi-Fi, GSM, 4G, LTE, LoRaWAN, and NB-IoT
via TinyGSM, providing a robust solution for diverse IoT applications.

## Features

| Feature          | Status   |
|------------------|----------|
| Device Shadow    | &#x2714; |
| Device Logs      | &#x2714; |
| Device Data      | &#x2714; |
| User Association | &#x2714; |
| OTA              | &#x2714; |
| BLE              | `alpha`  |
| Embedded RPC     | &#x2718; |

## Supported Hardware and Testing Status

| Hardware               | Status   | Tested    |
|------------------------|----------|-----------|
| ESP32-WROOM32-D        | `stable` | &#x2714;  |
| ESP32-C3               | `stable` | &#x2714;  |
| ESP32-S3               | `stable` | &#x2714;  |
| ESP32-C6               | `stable` | &#x2714;  |
| nRF52840-DK + SIM7600E | `dev`    | &#x2714;  |
| nRF52832-DK + SIM800L  | `dev`    | &#x2714 ; |
| nRF9160                | `dev`    | &#x2714;  |

Refer to Example Projects for more details.

## Getting Started

### Prerequisites

1. **Visual Studio Code**
    - Download and install Visual Studio Code from [here](https://code.visualstudio.com/download).

2. **PlatformIO Extension for VS Code**
    - Install the PlatformIO extension for Visual Studio Code from [here](https://platformio.org/install/ide?install=vscode).

3. **ProtoCompiler**
    - `protoc` is required for Golain SDKs and should be available in your `PATH`.
        - **Windows** - Download and install from [here](https://github.com/protocolbuffers/protobuf/releases).
        - **Linux**:
            - **Debian/Ubuntu**:
              ```bash
              sudo apt install protobuf-compiler
              ```
            - **Fedora**:
              ```bash
              sudo dnf install protobuf-compiler
              ```
            - **Arch**:
              ```bash
              sudo pacman -S protobuf
              ```
        - **MacOS**:
            - via Homebrew:
              ```bash
              brew install protobuf
              ```
            - via MacPorts:
              ```bash
              sudo port install protobuf3-cpp
              ```

### Setup

4. **Update metadata**
    - Update the `ROOT_TOPIC` in this format: `'"/xxxx/"'` (replace xxxx with your topic)
    - Update the `DEVICE_NAME` in this format: `'"xxxx"'`  (replace xxxx with your device name)

5. **Get device credentials from [Golain Console](https://web.golain.io)**
    - Login to Golain Console and navigate to the `Devices` section.
    - Click on `Add Device` and follow the instructions.
    - Download the PlatformIO `certs.h` file and place it in your project's `include` folder.
    - Also, download the `project template` and place its contents in the `src` folder.

    ![template](assets/template.png)

6. **Update `main.cpp`**
    - Replace the content of `main.cpp` with the content from the `main.ino` file provided during the header download.

7. **Install dependencies**
    - From the quick access of PIO Home, go to libraries and install the following dependencies: `golain-platformio-sdk`.

8. **Verify your ESP32 board connection**
    - Ensure your ESP32 board appears on the `Serial Port` from the `device` section in PIO Home.

9. **Build and flash the program**
    - Once all dependencies and files are placed and modified, build the program. Your build will be flashed onto the board.

10. **Verify device status**
    - If everything is done correctly, you should be able to see your device online on the [Golain Console](https://web.golain.io).


## Contributing
We welcome contributions to enhance the Golain PlatformIO SDK. Please submit pull requests to the GitHub repository.

## Support
For support and troubleshooting, please visit the Golain [support page](https://scheduler-web-development.azo.dev/consumers/event/166/golain-solutions) or contact us via [email](ishan@golain.io).

## License
This project is licensed under the MIT License. See the [LICENSE file](https://github.com/golain-io/golain-platformio-sdk/blob/main/LICENSE) for details.

