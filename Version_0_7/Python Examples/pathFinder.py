# run "pip install pyserial" in the command line to install the serial library
# run "python -m serial.tools.list_ports" to get a list of your serial ports
# run "pip3 install keyboard" to install the keyboard library

#This example implements the basic path finder algorithm found in V0.4 of 
#MIPR's base code. Instead of writing the code and uploading it to the 
#Arduino we use the SDK and python. All of the timer.sleep() commands gives us our
#system delay which is 52mS. We use this delay because we can get a new reading
#from the VL53L1X every 50mS. You need a delay between sending commands over serial
#If you don't commands will get lost. I use a 1mS delay between sending commands in the main loop.

import serial
import sys
import time
import keyboard

try:
	#SETUP THE SERIAL PORT
	ser = serial.Serial('COM11', 57600, timeout=0, parity=serial.PARITY_EVEN, rtscts=0) #connect to the serial port, you might need to change the comm port
	time.sleep(3) #wait for MIPR to boot up
	msg = ser.read(500) #Read whatever is in the buffer
	time.sleep(0.05) #allow the program time to read the buffer

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

	dirCounter = 0 #Used to decide which direction to turn

	#SETUP VARIOUS VALUES ON MIPR
	ser.write(b'SPD:96') #Set the speed to 200
	time.sleep(0.05)
	ser.write(b'SETSEN') #Start the VL53L1X
	time.sleep(0.05)
	ser.write(b'MAXD:4000') #Set the VL53L1X max range to 4 meters
	time.sleep(0.05)

	print('Basic Path Finding Algorithm Executing type S to stop')

	#GET A DISTANCE MEASUREMENT AND ACT ACCORDINGLY
	while True: #Loop forever
		time.sleep(0.025)
		ser.write(b'DIST') #Get a measurement
		msg = ser.read(500) ##read the message from the serial buffer
		time.sleep(0.025)
		msg = msg.decode('utf-8') ##decode the message
		if keyboard.is_pressed('S') or keyboard.is_pressed('s'):
			ser.write(b'S') #Stop MIPR
			sys.exit()
		if msg != "": #Only print the message if it contains something
			#print(msg, end ='') #uncomment to print measurements; use this for debugging if you have issues
			try:
				intMsg = int(msg)

				if intMsg > 500:
					ser.write(b'SPD:96') #Set the speed to 200
					time.sleep(0.01)
					ser.write(b'F')

				if intMsg < 500:

					if dirCounter < 21:
						dirCounter = dirCounter + 1
						ser.write(b'SPD:64') #Set the speed to 64, we must corner slowly due to the vl53l1x refresh rate
						time.sleep(0.01)
						ser.write(b'R')

					if dirCounter > 20:
						dirCounter = dirCounter + 1
						ser.write(b'SPD:64') #Set the speed to 64, we must corner slowly due to the vl53l1x refresh rate
						time.sleep(0.01)
						ser.write(b'L')

				if dirCounter > 40:
					dirCounter = 0
			except:
				print('can\'t convert')
except:
	print('Can not connect please check the COMM port and ensure that MIPR is switched on') 