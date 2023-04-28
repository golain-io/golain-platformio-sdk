import os
import time
import struct
import paho.mqtt.client as mqtt

# MQTT settings
mqtt_broker = "broker.hivemq.com"
mqtt_port = 1883
ota_update_topic = "golain/firmware/update"
ota_topic_length = "golain/firmware/length"
mqtt_client_id = "file_sender"

# Chunk size in bytes
chunk_size = 200

# Open the binary file for reading in binary mode
with open("firmware.bin", "rb") as f:
    # Get the file size
    file_size = os.path.getsize("firmware.bin")
    print(file_size)
   

    # Create a MQTT client instance
    client = mqtt.Client(mqtt_client_id)

    # Connect to the MQTT broker
    print(client.connect(mqtt_broker, mqtt_port))
    client.loop()

    # Send the file size as the first message
    # client.publish(mqtt_topic, struct.pack("!Q", file_size))
    print(client.publish(ota_topic_length,str(file_size)))
    # Loop through the file and send chunks
    while True:
    #     # Read a chunk of data from the file
         chunk = f.read(chunk_size)

    #     # If we've reached the end of the file, break out of the loop
         if not chunk:
             break

    #     # Send the chunk over MQTT
         client.publish(ota_update_topic, chunk)

         # Wait a bit before sending the next chunk
         time.sleep(0.1)

    # Disconnect from the MQTT broker
    client.disconnect()
