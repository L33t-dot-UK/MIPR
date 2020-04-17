/*
 * Motor Functions Module Verison 0.4
 * https://www.l33t.uk/arduino_projects/mipr/
 * Copyright David Bradshaw 2019
 */
 
int enRight = 5;
int enLeft = 9;
int i1A = 4; 
int i2A = 3;
int i3A = 8;
int i4A = 7;

boolean Left_Motor_Stopped = false;
boolean Right_Motor_Stopped = false;

int LEFTSPEED = 124;
int RIGHTSPEED = 124;
/*
 * MOTOR FUNCTIONS
 */

 void Forwards(int leftSpeed, int rightSpeed)
 {

    if (leftSpeed < 0 || rightSpeed < 0)
    {
        digitalWrite(i1A,HIGH);
        digitalWrite(i2A,LOW);
        digitalWrite(i3A,LOW);
        digitalWrite(i4A,HIGH);
        analogWrite(enLeft, 253);
        analogWrite(enRight, 253);
    }
    else
    {
        digitalWrite(i1A,LOW);
        digitalWrite(i2A,HIGH);
        digitalWrite(i3A,HIGH);
        digitalWrite(i4A,LOW);
        analogWrite(enLeft, leftSpeed);
        analogWrite(enRight, rightSpeed);
    }

   
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
    analogWrite(enLeft, 0);
    analogWrite(enRight, 0);
 }
  void softStop()
 {
    analogWrite(enLeft, 0);
    analogWrite(enRight, 0);
 }
