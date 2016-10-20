# -*- coding: utf-8 -*-
import paho.mqtt.client as mqtt
import requests

#called when a message is received by a topic
def on_message(mqttc, obj, msg):
    temp = msg.payload.decode("utf-8")
    print(temp)
    try:
	requests.get("https://api.thingspeak.com/update?api_key=VXTXNK09FJEQIE6F&field1="+str(temp), timeout=2)
	print("Sent")
    except:
	print("Not sent")

mqttc = mqtt.Client("mqtt2ts")
mqttc.username_pw_set("XXXX","XXX")
mqttc.on_message = on_message
mqttc.connect("m13.cloudmqtt.com", 12045)
mqttc.subscribe("temp", qos=0)
mqttc.loop_forever()
