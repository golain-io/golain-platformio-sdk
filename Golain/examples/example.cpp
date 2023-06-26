#include <Arduino.h>
#include <golain.h>
#include <certs.h>
#include <WiFi.h>
#include<data_point.pb.h>

uint8_t receive_buf[128];
uint8_t local_buffer[128];
size_t local_len;
golain_config global_config;
int myFunction(int, int);

uint8_t shadow_buff[Shadow_size];

void shadow_callback(char *topic, byte *payload, unsigned int length)
{
  memset(receive_buf,0,127);
    Serial.print("Received message [");
    Serial.print(topic);
    Serial.print("]: ");
    Serial.printf(" Received length is %d",length);
    for (int i = 0; i < length; i++)
    {
        receive_buf[i] = payload[i];
    }

    golain_shadow_get(receive_buf,length);
    Serial.printf("Red %d Green %d Blue %d state %d\n",global_shadow.red,global_shadow.green,global_shadow.blue,global_shadow.on);
}


// static const int RXPin = 4, TXPin = 0;
// static const uint32_t GPSBaud = 9600;
// void displayInfo();
// // The TinyGPSPlus object
// TinyGPSPlus gps;

// SoftwareSerial ss(RXPin, TXPin);

size_t encode_size = 0;

HSL2 data_point_struct =  HSL2_init_zero;

void setup()
{
  // ss.begin(GPSBaud);

  WiFi.begin("lol", "Casio@123");
  while (WiFi.status() != WL_CONNECTED)
  {
  }
  // put your setup code here, to run once:
  Serial.begin(9600);
  golain_set_client_id(&global_config, DEVICE_NAME);
  golain_set_ca_cert(&global_config, GOLAIN_CERT_CA);
  golain_set_device_cert(&global_config, GOLAIN_DEVICE_CERT);
  golain_set_device_pvt_key(&global_config, GOLAIN_PRIVATE_KEY);
  golain_set_callback(&global_config, shadow_callback);
  golain_set_root_topic(&global_config, TOPIC_ROOT);
  golain_init(&global_config);
  // client.subscribe(DEVICE_SHADOW_TOPIC_U,1);
  global_shadow.blue = 98;
  global_shadow.red = 89;
  global_shadow.green = 85;
  global_shadow.on = true;
}

double lng, lat;
String k = "";
void loop()
{
  client.loop();
  data_point_struct.Hue = rand() % 51 + 50;
  data_point_struct.Lightness = rand() % 51 + 50;
    data_point_struct.Saturation = rand() % 51 + 50;
    Serial.printf(" H = %d S = %d L = %d\n",data_point_struct.Hue,data_point_struct.Saturation,data_point_struct.Lightness);
golain_post_device_data(&data_point_struct,HSL2_fields,"HSL2",HSL2_size); 
  
  delay(2000);

  // global_shadow.blue = global_shadow.blue +1;
  // global_shadow.red = global_shadow.red + 1;
  // global_shadow.green = global_shadow.green + 1;
  // global_shadow.on = true;
  // golain_shadow_set(shadow_buff, &encode_size);
  // Serial.printf("Encoded size was %d", encode_size);
  // golain_shadow_get(shadow_buff, encode_size);
  // Serial.println(global_shadow.red);
  // Serial.println(global_shadow.blue);
  // Serial.println(global_shadow.green);
  // postshadow(shadow_buff, encode_size);
  // delay(2000);
  // lng = gps..lng();
  // lat = gps.location.lat();
  // Serial.printf("latitude = %lf\n",lat);
  // Serial.printf("longitude = %lf\n",lng);
  // k+= String(lng)+ ","+String(lat);

  // client.publish(DEVICE_LOCATION_TOPIC,k.c_str());
}

// put function definitions here:
