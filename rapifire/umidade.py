# -*- coding: utf-8 -*-
import paho.mqtt.client as mqtt
import time
import random

mqttc = mqtt.Client("python_pub")
mqttc.username_pw_set("EThoxloDv35yQYLHlKVPIb","xxxx")
mqttc.connect("mqtt.rapifire.com", 1883)
while True:
   r = random.random()+17.31
   mqttc.publish("/EThoxloDv35yQYLHlKVPIb/data", '{"e" : [{"n" : "temp", "v" : '+str(r)+'}]}')
   mqttc.loop(5) #timeout = 5s
   time.sleep(2)
