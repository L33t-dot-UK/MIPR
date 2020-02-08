/*
 * BEFORE RUNNING THIS PROGRAM ENSURE THAT YOU HAVE CALIBRATED THE ODOMETRY MODULE CORRECTLY
 * USING THE PROCEDURE FROM BUILDING THE BATTERY BOARD TUTORIAL FOUND AT L33T.UK/MIPR
 */

#include <EEPROM.h>

int enRight = 9;
int enLeft = 5;
int i1A = 4; 
int i2A = 3;
int i3A = 8;
int i4A = 7;

int leftMotorSensor = A7;
int rightMotorSensor = A6;

int maxRight = 0;
int minRight = 1024;
int maxLeft = 0;
int minLeft = 1024;

int meanLeft = 500;
int meanRight = 500;


int startTime = 0;
int loopTime = 0;

void setup() 
{
    Serial.begin(57600);

    //Lets see the maximum and minimum values for the left and right
    //Sensor. This will be different for each robot depending on the
    //angle of the TCRT5000 and it's distance from the wheel

    if (int(EEPROM.read(0)) == 128)
    {
        maxRight = int(EEPROM.read(1)) * 4;
        minRight = int(EEPROM.read(2)) * 4;
        maxLeft = int(EEPROM.read(3)) * 4;
        minLeft = int(EEPROM.read(4)) * 4;
        meanRight = int(EEPROM.read(6)) * 4;
        meanLeft = int(EEPROM.read(7)) * 4;
    }
    else
    {
        Serial.println("No values found; CALIBRATION must be ran before using the Odometry Module");
    }  
}

void loop() 
{
    //We will time how long it takes for the main loop to execute so we know if our odometry calculations are accurate
    startTime = millis();

    Forwards(128,128);
    SampleWaveForm(false);

    loopTime = millis() - startTime;
}
