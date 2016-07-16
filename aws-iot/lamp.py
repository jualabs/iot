#!/usr/bin/python3

#required libraries
import ssl
import paho.mqtt.client as mqtt
import configparser
import json

devStatus = {}
devStatus["color"] = "Red"

#called while client tries to establish connection with the server
def on_connect(mqttc, obj, flags, rc):
    global devStatus
    if rc==0:
        global thingname
        print ("Connection successful")
        mqttc.subscribe("$aws/things/"+thingname+"/shadow/control", qos=0) #The topics start with $aws/things/thingName/shadow."
        print("My color is",devStatus["color"])
    elif rc==1:
        print ("Connection refused")

#called when a message is received by a topic
def on_message(mqttc, obj, msg):
    global devStatus
    global thingname
    newStatus = msg.payload.decode("utf-8")
    if (newStatus in ['Red','Green','Blue']):
        devStatus["color"] = newStatus
    mqttc.publish(topic="$aws/things/"+thingname+"/shadow/lamp",payload=json.dumps(devStatus),qos=0)
    print("My color is",devStatus["color"])

if __name__ == '__main__':
    #Get properties from file
    config = configparser.RawConfigParser()
    config.read('conf/config.prop')
    thingname = config.get("General","thingname")
    rootcert = config.get("General","rootcert")
    certificate = config.get("General","certificate")
    key = config.get("General","key")
    endpoint = config.get("General","endpoint")
    awsport = config.getint("General","port")
    #creating a client with client-id=mqtt-test
    mqttc = mqtt.Client(client_id="lamp")

    mqttc.on_connect = on_connect
    mqttc.on_message = on_message
    #Configure network encryption and authentication options. Enables SSL/TLS support.
    #adding client-side certificates and enabling tlsv1.2 support as required by aws-iot service
    mqttc.tls_set(rootcert,
                certfile=certificate,
                keyfile=key,
                tls_version=ssl.PROTOCOL_TLSv1_2,
                ciphers=None)
    
    #connecting to aws-account-specific-iot-endpoint
    mqttc.connect(endpoint, port=awsport) #AWS IoT service hostname and portno

    #automatically handles reconnecting
    mqttc.loop_forever()
    #mqttc.loop_start()
