import serial
from datetime import datetime
import os

port = "/dev/" + os.getenv('ARDUINO_BT', "rfcomm0") #This will be different for various devices and on windows it will probably be a COM port.

bluetooth=serial.Serial(port, 9600) #Start communications with the bluetooth unit
bluetooth.flushInput() #This gives the bluetooth a little kick
input_data=bluetooth.readline()#This reads the incoming data. In this particular example it will be the "Hello from Blue" line
input_data=input_data.decode()
bluetooth.close() #Otherwise the connection will remain open until a timeout which ties up the /dev/thingamabob
input_data = input_data.split("&")
while "\n" in input_data: input_data.remove("\n")    
if(len(input_data) >= 2 ):
    print("weather,location=inside temperature=" + str(input_data[0]) + ",light=" + str(input_data[1]))
    # int(  datetime.timestamp(datetime.now()))

