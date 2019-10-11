/*
//Author David Bradshaw
//Code to setup Odometry module thresholds and sampling
//Odo thresholds will be saved on EEPROM
//Visit https://www.l33t.uk/MIPR for more information
*/

int addr = 0; //EEPROM ADDRESS

#include <EEPROM.h> //Include this library to access the EEPROM on the Arduino, 

int enRight = 9;
int enLeft = 5;
int i1A = 4; 
int i2A = 3;
int i3A = 8;
int i4A = 7;

int leftMotorSensor = A7;
int rightMotorSensor = A6;

//These values will be set by 01_run_Wheel_Sensor_Cal() and saved in your EEPROM;
int maxRight = 0;
int minRight = 1024;
int maxLeft = 0;
int minLeft = 1024;

int meanLeft = 500;
int meanRight = 500;


int startTime = 0;
int loopTime = 0;

int RESTART_MODE = 0;

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
        Serial.println("Odometry threshold values already in EEPROM");
        Serial.print("meanRight");
        Serial.print(": ");
        Serial.print(meanRight);
        Serial.print("      ");
        Serial.print("meanLeft");
        Serial.print(": ");
        Serial.print(meanLeft);
        Serial.print("      ");
        Serial.print("rightMax");
        Serial.print(": ");
        Serial.print(maxRight);
        Serial.print("      ");
        Serial.print("rightMin");
        Serial.print(": ");
        Serial.print(minRight);
        Serial.print("      ");
        Serial.print("leftMax");
        Serial.print(": ");
        Serial.print(maxLeft);
        Serial.print("      ");
        Serial.print("leftMin");
        Serial.print(": ");
        Serial.println(minLeft);
    }
    else
    {
        Serial.println("No values found; running calibration, this will take around 5 seconds");
        _01_run_Wheel_Sensor_Cal(true);
    }

    RESTART_MODE = int(EEPROM.read(5));  //Dictates what mode to start MIPR in
    Serial.println(RESTART_MODE);
    if (RESTART_MODE == 48) //Default Mode 0
    {
         Serial.println("MIPR STARTED IN DEFAULT MODE");
         //MIPR WILL WAIT FOR COMMANDS IN THIS MODE
    }
    else if (RESTART_MODE == 49) //MODE 1
    {
        //DISPLAYS PULSE TRAIN FOR LEFT AND RIGHT WHEEL TO CHECK THE SETUP AND THRESHOLDS
        Serial.println("MIPR STARTED PULSE DISPLAY MODE");
    }
    else if (RESTART_MODE == 50) //MODE 2
    {
        Serial.println("MIPR STARTED IN MODE 2");
        //NOT IMPLEMENTED
    }

    else
    {
         Serial.println("NO RESTART MODE SELECTED");
    }
}

void loop() 
{
    //We will time how long it takes for the main loop to execute so we know if our odometry calculations are accurate
    startTime = millis();

    //INSERT CODE HERE
    if (RESTART_MODE == 49)
    {
        //Pulse display mode used to test thresholds
        Forwards(128,128);
        _01_Display_Pulse_Train();
    }
    
    char command = listenForBTCommands();
    if (command != "")
    {
        executeBTcommand(command);
    }
    
    loopTime = millis() - startTime; //This must be below 4mS for accurate 
}

void _01_run_Wheel_Sensor_Cal(boolean toPrint)
{
    minRight = 1024;
    minLeft = 1024;
    maxRight = 0;
    maxLeft = 0;
    Forwards(128,128); //Move the motors forward

    int samples = 30000;

    long _meanRight = 0;
    long _meanLeft = 0;

    //take an average reading over 10,000 samples
    for (int ii = 0; ii < samples; ii++)
    {
        int rightReading = analogRead(A6);
        int leftReading = analogRead(A7);
        
        _meanRight = _meanRight + rightReading;
        _meanLeft = _meanLeft + leftReading;

        if(maxRight < rightReading)
        {
            maxRight = rightReading;
        }

        if(maxLeft < leftReading)
        {
            maxLeft = leftReading;
        }

         if(minRight > rightReading)
        {
            minRight = rightReading;
        }

        if(minLeft > leftReading)
        {
            minLeft = leftReading;
        }   
    }

    _meanRight = _meanRight / samples;
    _meanLeft = _meanLeft / samples;

    maxRight = maxRight * 0.95;
    minRight = minRight * 1.55;
    maxLeft = maxLeft * 0.95;
    minLeft = minLeft * 1.55;

    //Save vailues to EEPROM
    addr = 0;
    EEPROM.write(addr, 128); //This indicates that we have calibration figures in the EEPROM
    addr = addr + 1;
    EEPROM.write(addr, (maxRight / 4)); //must divide by 4 to fit value into 1 byte
    addr = addr + 1;
    EEPROM.write(addr, (minRight / 4)); //must divide by 4 to fit value into 1 byte
    addr = addr + 1;
    EEPROM.write(addr, (maxLeft / 4)); //must divide by 4 to fit value into 1 byte
    addr = addr + 1;
    EEPROM.write(addr, (minLeft / 4)); //must divide by 4 to fit value into 1 byte
    addr = 6;
    EEPROM.write(addr, (_meanRight / 4)); //must divide by 4 to fit value into 1 byte
    addr = addr + 1;
    EEPROM.write(addr, (_meanLeft / 4)); //must divide by 4 to fit value into 1 byte

    if (toPrint)
    {
        //Display values for user
        Serial.println("When using the odometry module use the below values as your thresholds");
        Serial.println("meanRight and meanLeft indicates if your TCRT5000's are positioned correctly. This value");
        Serial.println("should be 500 give or take 100. So your reading for this should be between 400 and 600");
        Serial.println("If it is'nt then tilt the sensors either up or down slightly and repeat the test, keep doing this until");
        Serial.println("you get the correct reading.");
        Serial.print("meanRight");
        Serial.print(": ");
        Serial.print(_meanRight);
        Serial.print("      ");
        Serial.print("meanLeft");
        Serial.print(": ");
        Serial.print(_meanLeft);
        Serial.print("      ");
        Serial.print("rightMax");
        Serial.print(": ");
        Serial.print(maxRight);
        Serial.print("      ");
        Serial.print("rightMin");
        Serial.print(": ");
        Serial.print(minRight);
        Serial.print("      ");
        Serial.print("leftMax");
        Serial.print(": ");
        Serial.print(maxLeft);
        Serial.print("      ");
        Serial.print("leftMin");
        Serial.print(": ");
        Serial.println(minLeft);
    }
 
    Halt(); //Stop the motors
}

//This function will print the pulse train for both wheels and
//Show the digital square wave created with the thresholds set above.
//use this to check your setuo to ensure that the TCRT5000's are
//aligned correctly. If they are aligned correctly you should see a
//square wave for each wheel that is a digital representation of the 
//sampled analog waveform created by the rotation of the wheels.
void _01_Display_Pulse_Train()
{
    //Left Wheel
    int reading = analogRead(A7); //left wheel
    Serial.print(reading);
    Serial.print(',');


    int tmpVal = maxLeft;

    if (reading > meanLeft)
    {
         tmpVal = maxLeft;
    }
    else
    {
        if (reading < meanLeft)
        {
            tmpVal = minLeft;
        }
    }

    Serial.print(tmpVal);
    Serial.print(',');
    reading = analogRead(A6); //right wheel
    Serial.print(reading);
    Serial.print(',');

    tmpVal = maxRight;

    if (reading > meanRight)
    {
         tmpVal = maxRight;
    }
    else
    {
        if (reading < meanRight)
        {
            tmpVal = minRight;
        }
    }
    Serial.println(tmpVal);
}
