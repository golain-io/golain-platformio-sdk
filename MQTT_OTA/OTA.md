# Over the Air (OTA) update

## Overview
This component lets you update your firmware remotely. You just need to upload your tested .bin firmware file to Golain backend. Our APIs provides seamless firmware updates ove secured MQTT. 

## Usage
OTA (Over-The-Air) updates are increasingly becoming an essential feature of embedded systems, especially in the context of IoT (Internet of Things) devices. OTA updates allow developers to remotely update the firmware of their devices, which is beneficial for a variety of reasons, such as:

- Bug Fixes: OTA updates can fix bugs and vulnerabilities in the firmware without requiring the device to be physically accessed. This can significantly reduce the cost and time associated with bug fixing.

- Security Patches: OTA updates can deliver security patches to address vulnerabilities in the firmware. Since security threats evolve rapidly, the ability to quickly deliver security patches is essential for keeping devices secure.

- Feature Enhancements: OTA updates can also deliver new features and improvements to the firmware, allowing developers to add functionality to their devices post-deployment.

- Compliance: OTA updates can ensure that devices remain compliant with regulations and standards, such as GDPR, by providing firmware updates that address compliance requirements.

- Cost-Effective: OTA updates can reduce the cost of device maintenance and updates, since they can be delivered remotely without the need for a physical technician.

- User Experience: OTA updates can improve the user experience of devices, by enabling developers to fix issues and add new features that enhance the functionality of the device.

## Availble Macros
 
### For setup
- CONFIG_OTA_ENABLE  
    This macro lets user enable or disable the OTA updates. By default this macro will be 1. If user wishes, not to have firmware updates, this macro can be made 0.  

#### Note - User only has to connect to the Golain backend and enable the macro above, in order to avail the OTA service. Any other APIs are not required.  
<br>
 Caution - While uploading the firmware via Golain APIs, ensure that it contains this OTA component, otherwise you will not be able to update your device remotely again.

