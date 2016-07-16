#!/usr/bin/python3

#required libraries
import ssl
import paho.mqtt.client as mqtt
import cmd
import time
import json
import configparser

devStatus = None

#called while client tries to establish connection with the server
def on_connect(mqttc, obj, flags, rc):
    if rc==0:
        global thingname
        print ("Connection successful")
        mqttc.subscribe("$aws/things/"+thingname+"/shadow/lamp", qos=0) #The topics start with $aws/things/thingName/shadow."
    elif rc==1:
        print ("Connection refused")

#called when a topic is successfully subscribed to
def on_subscribe(mqttc, obj, mid, granted_qos):
    print("Subscribed")

#called when a message is received by a topic
def on_message(mqttc, obj, msg):
    global devStatus
    jData = json.loads(msg.payload.decode("utf-8"))
    devStatus = jData["color"]

class ControlInterface(cmd.Cmd):
    
    def do_status(self, line):
        global devStatus
        print(devStatus)
    
    def do_send(self, color):
        global thingname
        mqttc.publish(topic="$aws/things/"+thingname+"/shadow/control",payload=color,qos=0)
    
    def do_EOF(self, line):
        return True

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
    mqttc = mqtt.Client(client_id="commander")

    mqttc.on_connect = on_connect
    mqttc.on_subscribe = on_subscribe
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
    #mqttc.loop_forever()
    mqttc.loop_start()
    time.sleep(2)
    mqttc.publish(topic="$aws/things/"+thingname+"/shadow/control",payload="",qos=0)
    ControlInterface().cmdloop()
