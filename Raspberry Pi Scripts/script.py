#!/usr/bin/env python3

"""
Author: Team Green, EE486, Spring 2021
Description:
Launches on boot of Raspberry Pi through boot.py. Find its local IP address and sets up MQTT broker. 
Waits for data to be published to MQTT broker. On publish manipulated payload and upload to a PHP script.
"""

import paho.mqtt.client as mqtt
import socket
import csv
from datetime import datetime
from datetime import date
import requests

mqtt_topic = "home/measurements"  # Defines MQTT topic
client = mqtt.Client()  # Creates MQTT client
url = 'https://www.ceias.nau.edu/capstone/projects/EE/2021/NAUResearchGreenhouse-F20/upload.php'  # PHP upload URL


def on_connect(client, userdata, flags, rc):
    print("Connected!", str(rc))
    client.subscribe(mqtt_topic)


def on_message(client, userdata, msg):
    now = datetime.now()  # Get current date/time
    time = now.strftime("%m/%d/%Y %H:%M:%S")  # From date/time create a string
    payload = str(msg.payload)[2:-1]  # Strip off unneccesary data from string
    payload = payload.split(',')  # Split string into list on ','
    payload.insert(0, time)  # Instert time at position 0 of list
    module = payload.pop(1)  # Pop the module number off of the list
    print(payload)  # Print payload for debugging
    # Open database file based on module number as append
    with open('/home/pi/Desktop/data/database_%s.csv' % module, 'a+') as file:
        writer = csv.writer(file)  # Open CSV writer onto file
        writer.writerow(payload)  # Write payload into CSV file
    dataFile = open('/home/pi/Desktop/data/database_%s.csv' %
                    module, "rb")  # Open database file as read
    r = requests.post(url, data={'submit': 'Upload'}, files={
                      'fileToUpload': dataFile})  # Post file to PHP upload form
    # Print if the database was uploaded correctly
    print(r.text.find("The uploaded file has been accepted."))
    dataFile.close()  # Close file


def get_ip_address():
    ip_address = ''
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8", 80))
    ip_address = s.getsockname()[0]
    s.close()
    return ip_address


client.on_connect = on_connect
client.on_message = on_message
print(get_ip_address())
mqtt_broker_ip = get_ip_address()
client.connect(mqtt_broker_ip, 1883)
client.loop_forever()
client.disconnect()
