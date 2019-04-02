import paho.mqtt.client as mqtt
import random,json
from datetime import datetime
from time import sleep
#====================================================
# MQTT Settings 
MQTT_Broker = "192.168.1.3" #ten may chu MQTT
MQTT_Port = 1883          #port mac dinh cua MQTT
Keep_Alive_Interval = 45  #Thoi gian giua cac lan gui goi tin 
MQTT_Topic = "home/sensors"

#====================================================
# Ham ket noi den may chu MQTT
def on_connect(client, userdata, rc):
    if rc != 0:
        pass
        print ("Unable to connect to MQTT Broker...")
    else:
        print ("Connected with MQTT Broker: " + str(MQTT_Broker))

def on_publish(client, userdata, mid):
    pass
        
def on_disconnect(client, userdata, rc):
    if rc !=0:
        pass
        
mqttc = mqtt.Client()
# mqttc.username_pw_set(username="nhung",password="123456") #thay  doi User password cua MQTT
mqttc.on_connect = on_connect 
mqttc.on_disconnect = on_disconnect
mqttc.on_publish = on_publish
mqttc.connect(MQTT_Broker, MQTT_Port, Keep_Alive_Interval)        
#=====================================================================
# Publish du lieu
def publish_To_Topic(topic, message):
    mqttc.publish(topic,message)
    print (("Published: " + str(message) + " " + "on MQTT Topic: " + str(topic)))
    print ("")
#====================================================
# FAKE RANDOM SENSOR 
def publish_Fake_Sensor_Values_to_MQTT():
    Humidity_Fake_Value = int(random.uniform(50, 100))
    Temperature_Fake_Value = int(random.uniform(20, 30))
    Light_Fake_Value = int(random.uniform(0,1))
    Sensor_data = {}
    Sensor_data['SensorID'] = "DHT-11"
    Sensor_data['Date_and_Time'] = (datetime.today()).strftime("%d-%b-%Y %H:%M:%S")
    Sensor_data['Humidity'] = Humidity_Fake_Value
    Sensor_data['Temperature'] = Temperature_Fake_Value
    Sensor_data['Light'] = Light_Fake_Value
    sensor_json_data = json.dumps(Sensor_data)
    print ("Publishing fake Sensor Value: ")
    publish_To_Topic (MQTT_Topic, sensor_json_data)
    sleep(3)

while True:
    publish_Fake_Sensor_Values_to_MQTT()
    sleep(3)
#====================================================