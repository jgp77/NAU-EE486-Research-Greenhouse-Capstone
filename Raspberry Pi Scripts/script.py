#!/usr/bin/env python3

import paho.mqtt.client as mqtt
import socket
import csv
from datetime import datetime
from datetime import date
import requests

mqtt_topic = "home/measurements"
client = mqtt.Client()
url = 'https://www.ceias.nau.edu/capstone/projects/EE/2021/NAUResearchGreenhouse-F20/upload.php'


def on_connect(client, userdata, flags, rc):
    print("Connected!", str(rc))
    client.subscribe(mqtt_topic)


def on_message(client, userdata, msg):
    now = datetime.now()
    time = now.strftime("%d/%m/%Y %H:%M:%S")
    payload = str(msg.payload)[2:-1]
    payload = payload.split(',')
    payload.insert(0, time)
    module = payload.pop(1)
    print(payload)
    with open('/home/pi/Desktop/data/database_%s.csv' % module, 'a+') as file:
        writer = csv.writer(file)
        writer.writerow(payload)
    myFile = open('/home/pi/Desktop/data/database_%s.csv' % module, "rb")
    r = requests.post(url, data={'submit': 'Upload'}, files={
                      'fileToUpload': myFile})
    print(r.text.find("The uploaded file has been accepted."))
    myFile.close()


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
