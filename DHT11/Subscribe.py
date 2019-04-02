import paho.mqtt.client as mqtt
from Get_Data_to_DB import Sensor

# The callback for when the client receives a CONNACK response from the server.
#====================================================
# MQTT Settings 
MQTT_Broker = "192.168.1.3"
MQTT_Port = 1883
Keep_Alive_Interval = 45
MQTT_Topic = "home/sensors/#"

#Connect MQTT broker
def on_connect(client, userdata, flags, rc):
    if rc != 0:
        pass
        print ("Unable to connect to MQTT Broker...")
    else:
        print ("Connected with MQTT Broker: " + str(MQTT_Broker))
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe(MQTT_Topic,0)

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    # This is the Master Call for saving MQTT Data into DB
    # For details of "sensor_Data_Handler" function please refer "sensor_data_to_db.py"
    print ("MQTT Data Received...")
    print ("MQTT Topic: " + msg.topic)  
    print ("Data: " + str(msg.payload))
    try:
        Sensor(msg.payload)
    except:
        print("Error insert database!!")
    print("-------------------")
        

client = mqtt.Client()
client.username_pw_set(username="nhung",password="123456")
client.on_connect = on_connect
client.on_message = on_message

client.connect(MQTT_Broker,MQTT_Port, Keep_Alive_Interval)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()