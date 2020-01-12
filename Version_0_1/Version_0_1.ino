/*
 * MIPR Core Code Verison 0.1
 * https://www.l33t.uk/arduino_projects/mipr/
 * Copyright David Bradshaw 2019
 * 
 * For use with no sensor board; give MIPR basic functionality such as motor control and Bluetooth commands
 * 
 */

int enRight = 9;
int enLeft = 5;
int i1A = 4; 
int i2A = 3;
int i3A = 8;
int i4A = 7;

int LEFTSPEED = 254;
int RIGHTSPEED = 254;

void setup() 
{
    Serial.begin(57600); //Use this baud rate if your using the
							//HC-05 to program the Arduino
	
	//Serial.begin(9600); //Use this baud rate if you havent changed the
							//Parameters on the HC-05 this is the default
							//baud.
}

void loop() 
{
    char command = listenForBTCommands();
    if (command != "")
    {
        executeBTcommand(command);
    }
}

/*
 * MOTOR FUNCTIONS
 */

 void Forwards(int leftSpeed, int rightSpeed)
 {
    digitalWrite(i1A,LOW);
    digitalWrite(i2A,HIGH);
    digitalWrite(i3A,HIGH);
    digitalWrite(i4A,LOW);
    analogWrite(enLeft, leftSpeed);
    analogWrite(enRight, rightSpeed);
 }
  void Backwards(int leftSpeed, int rightSpeed)
 {
    digitalWrite(i1A,HIGH);
    digitalWrite(i2A,LOW);
    digitalWrite(i3A,LOW);
    digitalWrite(i4A,HIGH);
    analogWrite(enLeft, leftSpeed);
    analogWrite(enRight, rightSpeed);
 }
 void Right(int leftRotationSpeed, int rightRotationSpeed)
 {
    digitalWrite(i1A,LOW);
    digitalWrite(i2A,HIGH);
    digitalWrite(i3A,LOW);
    digitalWrite(i4A,HIGH);
    analogWrite(enLeft, leftRotationSpeed);
    analogWrite(enRight, rightRotationSpeed);
 } 
 void Left(int leftRotationSpeed, int rightRotationSpeed)
 {
    digitalWrite(i1A,HIGH);
    digitalWrite(i2A,LOW);
    digitalWrite(i3A,HIGH);
    digitalWrite(i4A,LOW);
    analogWrite(enLeft, leftRotationSpeed);
    analogWrite(enRight, rightRotationSpeed);
 }
 void Halt()
 {
    digitalWrite(i1A,LOW);
    digitalWrite(i2A,LOW);
    digitalWrite(i3A,LOW);
    digitalWrite(i4A,LOW);
    analogWrite(enLeft, 254);
    analogWrite(enRight, 254);
 }
  void softStop()
 {
    analogWrite(enLeft, 0);
    analogWrite(enRight, 0);
 }

/*
 * BLUETOOTH CONTROLS
 */

 char listenForBTCommands()
 {
    char BTcommand = "";
    if (Serial.available())
    {
        BTcommand = Serial.read();
    }
    return BTcommand;
 }

 void executeBTcommand(char command)
 {
    if (command == 'F') //Forwards
    {
        softStop();
        Forwards(LEFTSPEED, RIGHTSPEED);
    }
        if (command == 'B') //Backwards
    {
        softStop();
        Backwards(LEFTSPEED, RIGHTSPEED);
    }
        if (command == 'L') //Left turn
    {
        softStop();
        Left(LEFTSPEED, RIGHTSPEED);
    }
        if (command == 'R') //Right turn
    {
        softStop();
        Right(LEFTSPEED, RIGHTSPEED);
    }
        if (command == 'S') //Stop
    {
        softStop();
        delay(1);
        Halt();
    }
 }
