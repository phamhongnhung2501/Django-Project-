#!/usr/bin/python

import sqlite3
import json

# SQLite DB Name
DB_Name =  "../db.sqlite3"

def Sensor(jsonData):
	#Parse Data

	json_Dict = json.loads(jsonData)

	SensorID = json_Dict['SensorID']
	Data_and_Time = json_Dict['Time']
	Temperature = json_Dict['Temperature']
	Humidity = json_Dict['Humidity']
	Light = json_Dict['Light']
	print(Light)
	#Push into DB Table
	print()
	conn = sqlite3.connect(DB_Name)
	conn.execute("INSERT INTO sensor_sensor (SensorID,Date_and_Time, Temperature,Humidity,Light) \
	      VALUES (?,?,?,?,?)",[SensorID,Data_and_Time, Temperature,Humidity,Light])
	conn.commit()
	print ("Sensor created new value")
	conn.close()


