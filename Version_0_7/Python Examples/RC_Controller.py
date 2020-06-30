# run "pip install pyserial" in the command line to install the serial library
# run "python -m serial.tools.list_ports" to get a list of your serial ports
# run "pip3 install keyboard" to install the keyboard library

#Allows you to control MIPR using your keyboard in same way as the Android application]#
#found here https://play.google.com/store/apps/details?id=braulio.calle.bluetoothRCcontroller

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

	print('W = FORWARDS, S = BACKWARDS, A = LEFT, D = RIGHT, O = STOP, 1 -9 WILL SET YOUR SPEED')
	while True: #Loop forever
		time.sleep(0.02)
		if keyboard.is_pressed('S') or keyboard.is_pressed('s'):
			ser.write(b'S')
			time.sleep(0.01)
			ser.write(b'B')
		if keyboard.is_pressed('W') or keyboard.is_pressed('w'):
			ser.write(b'S')
			time.sleep(0.01)
			ser.write(b'F')
		if keyboard.is_pressed('A') or keyboard.is_pressed('a'):
			ser.write(b'S')
			time.sleep(0.01)
			ser.write(b'L')
		if keyboard.is_pressed('D') or keyboard.is_pressed('d'):
			ser.write(b'S')
			time.sleep(0.01)
			ser.write(b'R')
		if keyboard.is_pressed('O') or keyboard.is_pressed('o'):
			ser.write(b'S')
			time.sleep(0.01)
			ser.write(b'S')
		if keyboard.is_pressed('1'):
			ser.write(b'1')
		if keyboard.is_pressed('2'):
			ser.write(b'2')
		if keyboard.is_pressed('3'):
			ser.write(b'3')
		if keyboard.is_pressed('4'):
			ser.write(b'4')
		if keyboard.is_pressed('5'):
			ser.write(b'5')
		if keyboard.is_pressed('6'):
			ser.write(b'6')
		if keyboard.is_pressed('7'):
			ser.write(b'7')
		if keyboard.is_pressed('8'):
			ser.write(b'8')
		if keyboard.is_pressed('9'):
			ser.write(b'9')
	
except:
	print('Can not connect please check the COMM port and ensure that MIPR is switched on') 