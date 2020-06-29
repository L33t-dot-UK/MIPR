/*
 * Motor Functions Module Version 1.0
 * https://www.l33t.uk/arduino_projects/mipr/
 * Copyright David Bradshaw 2020
 */

//Pin allocations for the motor driver
int enRight = 9;
int enLeft = 5;
int i1A = 4; 
int i2A = 3;
int i3A = 8;
int i4A = 7;

//Default speed of the motors
int dLeftSpeed = 124;
int dRightSpeed = 124;

//Allows us to get the motors status
char leftMotorStatus = 'S';
char rightMotorStatus = 'S';

void Forwards(int leftSpeed, int rightSpeed)
{
    //Allows us to use negative speeds to make the robot go backwards
    if (leftSpeed < 0)
    {
        digitalWrite(i1A,HIGH);
        digitalWrite(i2A,LOW);
        
        digitalWrite(i3A,HIGH);
        digitalWrite(i4A,LOW);
        analogWrite(enLeft, abs(leftSpeed));
        analogWrite(enRight, abs(rightSpeed));
        leftMotorStatus = 'B';
        rightMotorStatus = 'F';
    }
    else if (rightSpeed < 0)
    {
        digitalWrite(i1A,LOW);
        digitalWrite(i2A,HIGH);
        
        digitalWrite(i3A,LOW);
        digitalWrite(i4A,HIGH);
        analogWrite(enLeft, abs(leftSpeed));
        analogWrite(enRight, abs(rightSpeed));
        leftMotorStatus = 'F';
        rightMotorStatus = 'B';
    }
    else
    {
        digitalWrite(i1A,LOW);
        digitalWrite(i2A,HIGH);
        digitalWrite(i3A,HIGH);
        digitalWrite(i4A,LOW);
        analogWrite(enLeft, leftSpeed);
        analogWrite(enRight, rightSpeed);
        leftMotorStatus = 'F';
        rightMotorStatus = 'F';
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
    leftMotorStatus = 'B';
    rightMotorStatus = 'B';
}
void Left(int leftRotationSpeed, int rightRotationSpeed)
{
    digitalWrite(i1A,HIGH);
    digitalWrite(i2A,LOW);
    digitalWrite(i3A,HIGH);
    digitalWrite(i4A,LOW);
    analogWrite(enLeft, leftRotationSpeed);
    analogWrite(enRight, rightRotationSpeed);
    leftMotorStatus = 'B';
    rightMotorStatus = 'F';
}
void Right(int leftRotationSpeed, int rightRotationSpeed)
{
    digitalWrite(i1A,LOW);
    digitalWrite(i2A,HIGH);
    digitalWrite(i3A,LOW);
    digitalWrite(i4A,HIGH);
    analogWrite(enLeft, leftRotationSpeed);
    analogWrite(enRight, rightRotationSpeed);
    leftMotorStatus = 'F';
    rightMotorStatus = 'B';
} 
void Halt()
{
    digitalWrite(i1A,LOW);
    digitalWrite(i2A,LOW);
    digitalWrite(i3A,LOW);
    digitalWrite(i4A,LOW);
    analogWrite(enLeft, 0);
    analogWrite(enRight, 0);
    leftMotorStatus = 'S';
    rightMotorStatus = 'S';
}

void softStop()
{
    analogWrite(enLeft, 0);
    analogWrite(enRight, 0);
    leftMotorStatus = 'S';
    rightMotorStatus = 'S';
}

//Helper funcitons to get information about the motors or to set certain parameters
void setLeftSpeed(int leftSpeed)
{
    dLeftSpeed = leftSpeed;
}
void setRightSpeed(int rightSpeed)
{
    dRightSpeed = rightSpeed;
}
int getLeftSpeed()
{
    return dLeftSpeed;
}
int getRightSpeed()
{
    return dRightSpeed;
}
char getLeftMotorStatus()
{
    return leftMotorStatus;
}
char getRightMotorStatus()
{
    return rightMotorStatus;
}
