# run "pip install pyserial" in the command line to install the serial library
# run "python -m serial.tools.list_ports" to get a list of your serial ports
# run "pip3 install keyboard" to install the keyboard library

#This scrypt will show you how to use the odometry mdoule to track the robots movements
#When this script is running you can push the robot and the direction of travel will be shown

import serial
import sys
import time
import keyboard
import csv
from io import StringIO

from turtle import *
color('red', 'yellow')
begin_fill()

#try:

#SETUP THE SERIAL PORT
ser = serial.Serial('COM11', 57600, timeout=0, parity=serial.PARITY_EVEN, rtscts=0) #connect to the serial port, you might need to change the comm port
time.sleep(3) #wait for MIPR to boot up
msg = ser.read(500) #Read whatever is in the buffer
time.sleep(0.001) #allow the program time to read the buffer

#GET THE OP MODE AND CHANGE TO 9 IF IT IS NOT 9
ser.write(b'GOM') #send the GOM command to get the Op Mode
time.sleep(0.05)
msg = ser.read(500)
msg = msg.decode('utf-8')
intMsg = int(msg)
if intMsg != 9:		#If the Op Mode is not 9 the change to 9
	print('Changing to SDK; Op Mode 9')
	time.sleep(0.05)
	ser.write(b'O') #Change MIPR's mode
	time.sleep(1)
	ser.write(b'9') #Put into Op Mode 9 SDK
	time.sleep(0.05)


#SETUP VARIOUS VALUES ON MIPR
ser.write(b'SPD:64') #Set the speed to 200
time.sleep(0.05)
ser.write(b'SETSEN') #Start the VL53L1X
time.sleep(0.05)
ser.write(b'MAXD:4000') #Set the VL53L1X max range to 4 meters
time.sleep(0.05)

ser.write(b'REF:20') #Set packet refresh to 10mS
time.sleep(0.05)
ser.write(b'ENODO') #Start the Odometry module
time.sleep(0.05)
ser.write(b'TELON') #Turn tel packets on
time.sleep(0.05)
ser.write(b'TEL:SB001AT') #Select which tel packet to use
time.sleep(0.05)

print('Basic Path Finding Algorithm Executing type Q to stop')

numCommas = 0

lostPackets = 0
totalPackets = 0
inputString = ""

lftOldVal = 0
rgtOldVal = 0
lftVal = 0
rgtVal = 0
dirL = ""
dirR = ""

while True: #Loop forever
		time.sleep(0.01)
		ser.write(b'F')
		if keyboard.is_pressed('q') or keyboard.is_pressed('Q'):
				print("Program exited, packets received = " + str(totalPackets) + ", packets lost = " + str(lostPackets))
				time.sleep(0.01)
				ser.write(b'S')
				sys.exit()
		
		rawTelPacket = ""
		toRead = True
		#Loop until the who packet has been read, the newLine \n will tell us that we have received the end of the telPacket
		while toRead:
				bufferSize = ser.in_waiting
				inputString = ser.read(bufferSize).decode('utf-8')
				rawTelPacket = rawTelPacket + inputString
				for strElement in rawTelPacket:
						if strElement == "\n":
								toRead = False #exit loop

		commaNo = rawTelPacket.count(',') #count the number of commas, in SB001AT we should have 8
		totalPackets = totalPackets + 1

		#because we're using tel packets we need to ensure that they are correct
		if commaNo == 8 and rawTelPacket[0] != ',': #If the Telpacket is correct use it
				TelPacket = StringIO(rawTelPacket)
				reader = csv.reader(TelPacket, delimiter=',')
				value = list(csv.reader(StringIO(rawTelPacket)))
				for val in value:
					try:
						lftVal = float(val[3])
						rgtVal = float(val[5])
						dirL = val[4]
						dirR = val[6]
					except:
						lftVal = 0
						rgtVal = 0
						dirL = ""
						dirR = ""
				for row in reader:
					if lftVal != 0 or rgtVal != 0:
						print('\t'.join(row))
						TelPacket = StringIO(rawTelPacket)

				lftSVal = (lftOldVal + (lftVal*0.6))
				rgtSVal = (rgtOldVal + (rgtVal*0.6))

				#Now draw the movement on the screen
				if abs(lftSVal - rgtSVal) < 5: #tolerance, go forward
					#forward(lftSVal)
					forward(0)
				elif lftSVal - rgtSVal < 0: #negative value robot is turning left
						left(abs(lftSVal - rgtSVal))
				elif lftSVal - rgtSVal > 0:
						right(abs(lftSVal - rgtSVal))
		else:
        		lostPackets = lostPackets + 1

	#reader = csv.reader(TelPacket, delimiter=',')
	#except:
		#print("Error")
	#print(numCommas)

	
#except:
	#print('Can not connect please check the COMM port and ensure that MIPR is switched on')
