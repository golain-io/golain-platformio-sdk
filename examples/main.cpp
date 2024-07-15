// Golain SDK Example
//
// This example shows how to use the Golain SDK to connect to the Golain Cloud and post data.
// You can download a sample main.cpp file from the Golain Cloud
// On the device page, look for the "Download sample project" button

// The certs.h file can be downloaded at the end of the device creation process on the Golain Cloud.
// Look for the "Download platformIO headers" button.
#include <certs.h>

#include <Arduino.h>

// #define CONFIG_GOLAIN_USE_GSM // Uncomment this line to use GSM / 4G connection instead of Wi-Fi
// ensure that you have TinyGSM library installed if you are using GSM / 4G connection
// additionally, Golain will deal with the SSL connection, so you don't need to use TinyGSM's SSLClient
// comment out the following lines if you are using TinyGSM:
#include <WiFi.h>
#include <WiFiClientSecure.h>

// For every datapoint, include the corresponding .pb.h file here
#include <coffee.pb.h>

// Include the Golain SDK
#include <golain.h>

Golain golain(DEVICE_NAME, ROOT_TOPIC, GOLAIN_CERT_CA, GOLAIN_DEVICE_CERT, GOLAIN_PRIVATE_KEY);

GolainDataPoint Icoffee;
CoffeeMachineTelemetry coffeeData;


/************************************* Device Shadow Callback ************************************
** This function is called everytime your device shadow is updated from the cloud. 
** You can use this to update your local device state based on the shadow state.
** The current or the new incoming shadow from the cloud is passed as the first argument, 
** and the last shadow state is passed as the second argument.
**
** For Example:
** If you want to turn on an LED when the red variable in the shadow is set to true, you can do:
**
** void onDeviceShadowUpdate(Shadow current, Shadow last) {
**     if (current.red) {
**         Serial.println("Red is on");
**     }
** }
** 
** NOTE: You can only access the shadow variables in this function. If you want to access the shadow
** variables outside this function, you can use the GlobalShadow object.
** Editing the current or the last shadow in this function will not update the shadow in the cloud.
**
** 
** To update the shadow in the cloud, set the new values in the GlobalShadow object, and call
** golain.updateShadow() function to send the new shadow to the cloud. See the loop() function below.
**
/*************************************************************************************************/
void onDeviceShadowUpdate(Shadow current, Shadow last) {
    Serial.println("Device shadow updated");
}

void setup() {
    Serial.begin(115200);

    // Fill in your Wi-Fi credentials here to connect to the internet:
    WiFi.begin("<SSID>", "<password>");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("Connected to WiFi");

    golain.setDeviceShadowCallback(&onDeviceShadowUpdate);

    Result r;

    r = golain.registerDataPoint("coffee", &CoffeeMachineTelemetry_msg, 128);
    
    if (r.err) {
        Serial.printf("Failed to register data point: coffee");
    } else {
        Serial.printf("Registered data point: coffee");
        Icoffee = r.value;
    }

    golain.begin();

    /************************************* Using GSM / 4G Connection ******************************
    ** Golain's SDK supports using TinyGSM to connect to the internet using a GSM / 4G connection.
    ** To use this feature, add this macro to the beginning of your code, before including golain.h:
    **
    ** #define CONFIG_GOLAIN_USE_GSM
    **
    ** Then, after initializing the TinyGSM modem, pass the modem object to the golain.begin() function.
    ** For Example:
    ** TinyGsm modem(SerialAT);
    ** golain.begin(&modem);
    /**********************************************************************************************/
}

void loop() {

    // Call this function to keep the connection alive.
    golain.loop();

    /************************************* Using Device Shadow ************************************
    **
    ** Access your shadow by using the GlobalShadow object
    **  
    ** For Example:
    ** Serial.println(GlobalShadow.blue); 
    **
    **
    ** To update the Shadow, just update the GlobalShadow object and call golain.updateShadow()
    ** 
    ** if (GlobalShadow.red) {
    **     GlobalShadow.blue = 20;
    **     golain.updateShadow();
    ** }
    **
    /**********************************************************************************************/
    
    /************************************* Posting Data *******************************************
    ** 
    ** 
    ** Modify your data-point variables to post updated data to the cloud.
    ** 
    ** For Example:
    ** coffeemachinetelemetryData.someVariable = someValue;
    **
    **
    ** To post the data, call the golain.postData() function.
    ** golain.postData(Icoffee, &coffeeData);
    **
    ** NOTE: You can only post data to the cloud if you have registered the data point.
    ** The data point is registered in the setup() function above.
    **
    ** You can also confirm if the data was posted successfully by checking the return value of the
    ** golain.postData() function.
    **
    ** For Example:
    ** if (golain.postData(Icoffee, &coffeeData).err != GOLAIN_OK){
    **     Serial.printf("Failed to post data for Data Point: COFFEE");
    ** } else {
    **     Serial.printf("Posted data for Data Point: COFFEE");
    ** }
    /********************************************************************************************/

    delay(1000);
}