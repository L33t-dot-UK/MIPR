/*
 * Line Follower Module Verison 0.1 SB-002
 * https://www.l33t.uk/arduino_projects/mipr/
 * Copyright David Bradshaw 2019
 * 
 * This is designed for tracks that use black lines on white background
 * 
 * The below code uses absolute values from the sensor board and for the code
 * to work you will need to use the resistor values from the tutorial for either
 * the Low Power or High Power board. If you use different valued resistors
 * you will need to convert the values; for the below code to work the baseLine
 * values will be between 250 and 450, if your baseLine values are 800 for instance 
 * you will have to divide all sensor values by at least half for them to work with this code
 * 
 *      Resistor values should be; Low Power: R1 220 Ohms, R2 - R6 4700 Ohms
 *                                 High Power: R1 20 Ohms, R2 - R6 470 Ohms (7805CV must be replaced with a BEC due to current draw)
 */

#include <PID_v1.h>
 
int L1 = A5; //Outer Left
int L2 = A4; //Inner Left
int M = A2; //Middle
int R2 = A1; //Inner Middle
int R1 = A0; //Outer Middle

float L1_bias = 1;
float L2_bias = 1;
float R1_bias = 1;
float R2_bias = 1;

int leftOVal;
int rightOVal;
int leftMVal;
int rightMVal;
int midVal;
    
int baseLineValue = 0;

boolean isCald = false;
boolean firstRun = true;

int lrErr = 0;

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

double baseSpeed = 55; //base speed
int minLimit = -200;
int maxLimit = 200;

//Ki should always be above Kp
double consKp=3, consKi=5, consKd=0.1;

PID leftPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);

int refreshRate = 0;
long refreshStartTime = millis();

void line_FollowerSetup()
{
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
    pinMode(A6, INPUT);
    pinMode(A7, INPUT);

    Setpoint = 0;
    //turn the PID on
    leftPID.SetOutputLimits(minLimit, maxLimit);
    leftPID.SetSampleTime(10);
    leftPID.SetMode(AUTOMATIC);
}

//Simple line follower using just 2 sensors L2 and R2. You need a fairly thick line for this to work properly.
//The robot will use absolute values for the speed of each motor not reacting to the environment just using simnple
//if then else statements to navigate the track
void simple_LF(int leftVal, int rightVal)
{ 
    int leftMax = leftVal + (leftVal * 0.2); //Tolerance of 20% to make the robot less jerky
    int leftMin = leftVal - (leftVal * 0.2);

    if (rightVal > leftMin && rightVal < leftMax) //If the left and right values are within 20%, go forward
    {
        Forwards(100, 100);
    }
    else if (leftVal > rightVal)
    {
        Right(50,50);
    }
    else if (rightVal > leftVal)
    {
        Left(50,50);
    }
}

//A simple line follower using all sensors by averaging out the values for left and right sensors
//The speed is adjusted depending on the difference between left and right values
void simple_LF_M(int lftAv, int rgtAv, int lrErr)
{  
    baseSpeed = 96; //base speed
    int minSpeed = 0; //Minimum motor speed
    int maxSpeed = 255; //Maximum motor speed when using low power sensor board
    //int maxSpeed = 150; //Maximum motor speed when using high power sensor board
    
    int lftSpeed;
    int rgtSpeed;
    
    //Our goal is to get lrErr to 0
    //if it is negative then the robot needs to turn left (left side over the line too much)
    //if it is possitive then the robot needs to turn right (right side over the line too much)

    //lrErr = lrErr * 0.8;      //Only use for high power sensor board
    
    if(lrErr > 0) // turn right if the error is possitive
    {
        lftSpeed = baseSpeed - lrErr;
        rgtSpeed = baseSpeed + lrErr;
    }
    else // turn left if the error is negative or 0
    {
        lftSpeed = baseSpeed + abs(lrErr);
        rgtSpeed = baseSpeed - abs(lrErr);
    }

    //Make sure we are above the minimum speed and below the maximum speed
    if (lftSpeed < minSpeed){lftSpeed = minSpeed;}
    if (rgtSpeed < minSpeed){rgtSpeed = minSpeed;}
    if (lftSpeed > maxSpeed){lftSpeed = maxSpeed;}
    if (rgtSpeed > maxSpeed){rgtSpeed = maxSpeed;}

    float nFactor = 1.8; //If this value is set too low the robot will be very jerky
    //if the speed is nFactor% higher than the base speed then turn left or right, this will produce a sharper 
    //turn than the Forwards function as it will turn one motor forwards and the other backwards to produce the 
    //sharp turn.
    if(lftSpeed > float(float(baseSpeed) * nFactor))
    {
        Left(lftSpeed, rgtSpeed);
    }
     else if(rgtSpeed > float(float(baseSpeed) * nFactor))
    {
        Right(lftSpeed, rgtSpeed);
    }
        
    else
    {
        Forwards(lftSpeed, rgtSpeed);
    }
}


//Complex line follower using a PID and all 5 sensors 
void complex_LF(int leftOVal, int leftMVal, int midVal, int rightMVal, int rightOVal, int lrErr)
{
    //  P - Proportional an amount to multiply the error by 
    //  I - Integral the sum of previous errors
    //  D - Derivative the speed at which we react to errors

    /*
     * For this PID the setpoint will always be 0 i.e. no error between the left and right side
     */

    float scaleFactor = 1 / float(maxLimit);
    float ETV; //Error Tracker Value ratio
    
    int leftAv = leftOVal + leftMVal / 2;
    int rightAv = rightMVal + rightOVal / 2;
    int lrAv = leftAv-rightAv;

    Input = lrAv; 

    int lftSpeed;
    int rgtSpeed;
    int errThreshold = 60;
    int upperThreshold = 120;

    leftPID.Compute(); 
    
    if (abs(Output) > upperThreshold)
    {
        if (baseSpeed > 55)
        {
            baseSpeed = baseSpeed - 20;
        }
    }
    if (abs(Output) < errThreshold)
    {
        if (baseSpeed < 200)
        {
            baseSpeed = baseSpeed + 10;
        }
    }
    
    ETV = 1 - (scaleFactor * float(abs(Output)));

    if (Output < 0) // turn right if the error is possitive
    {
        lftSpeed = (baseSpeed*ETV) + (ETV * abs(Output));
        rgtSpeed = baseSpeed + abs(Output);
    }
    else // turn left if the error is negative or 0
    {
        lftSpeed = baseSpeed + abs(Output);
        rgtSpeed = (baseSpeed*ETV) + (ETV * abs(Output));
    }

    int maxSpeed = 255; //Maximum motor speed when using low power sensor board

    if (lftSpeed > maxSpeed){lftSpeed = maxSpeed;}
    if (rgtSpeed > maxSpeed){rgtSpeed = maxSpeed;}
   
    Forwards(abs(lftSpeed), abs(rgtSpeed));
}

void LFHandler(int mode, boolean fullTele)
{
     //Check to see if this is the first run, if so calibrate the sensor board
     if (firstRun == true)
     {
         sensor_Cal();
         firstRun = false;
     }

    //get the sensor values
    int RefreshTimer = millis() - refreshStartTime;
    if (RefreshTimer > refreshRate)
    {
        leftOVal = getCalSensorVal(L1);
        rightOVal = getCalSensorVal(R1);
        leftMVal = getCalSensorVal(L2);
        rightMVal = getCalSensorVal(R2);
        midVal =  getCalSensorVal(M);
        RefreshTimer = 0;
        refreshStartTime = millis();
    }


    //average the value for the outer sensors
    int lftAv = (leftOVal + leftMVal) / 2;
    int rgtAv = (rightOVal + rightMVal) / 2;

    int outtertAv = (leftOVal + rightOVal) / 2;
    int innerAv =   (leftMVal + rightMVal) / 2;

    lrErr = lftAv - rgtAv; //Error for the intter left and inner right sensors
    
    int absAv = (leftOVal + rightOVal + leftMVal + rightMVal + midVal) / 5;

    float lftSpeed = 0;
    float rgtSpeed = 0;

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------
    //THE BELOW CODE IS ALL ABOUT DETECTING A LINE. IF THE LINE IS THERE THEN THE ALGORITHM WILL BE EXECUTED OTHER WISE THE ROBOT WILL STOP AND TURN ON THE SPEAKER
    //---------------------------------------------------------------------------------------------------------------------------------------------------------------
    
    boolean detectLine = false; //can we see a line
    
    //Compare readings from all sensors to see if a line is present we should have a decrease of at least 10% if we can see a line
    float lineThresh = 0.90; //This value might need tuning depending on the surface
    
    if (lftAv < 50 && rgtAv < 50) //No IR detected so we are either in a black surface, upside down or theres a thick cross road
    {
        //no floor can't be detected
        detectLine = false;
        speaker_on();
    }
    else if(lftAv >  rgtAv)  //Line could be on the right if the robot
    {
        if (float(float(rgtAv) / float(lftAv)) > lineThresh )
        {
            //No line detected check middle sensor
            if (checkMid(midVal, lftAv, rgtAv) == true) //No line detected in the right side so check the middle sensor
            {
                //line detected
                detectLine = true;
            }
            else
            {
                detectLine = false;
            }
        }
        else //RIGHT SENSORS OVER THE LINE
        {
            //line detected
            detectLine = true;
        }
    }
    else if(lftAv <  rgtAv) //Line coule be on the left of the robot
    {
        if (float(float(lftAv) / float(rgtAv)) > lineThresh )
        {
            //No line detected check middle sensor
            if (checkMid(midVal, lftAv, rgtAv) == true) //No line detected in the left side so check the middle sensor
            {
                //line detected
                detectLine = true;
            }
            else
            {
                detectLine = false;
            }
        }
        else  //LEFT SENSORS OVER THE LINE
        {
            //line detected
            detectLine = true;
        }
    }

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------
    //------------------------------------------- END OF LINE DETECTION STUFF ---------------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------------------------------------------------------------------------------------

    if (detectLine == false) //No line detected reverse the robot and sound the speaker
    {
        softStop();
        Backwards(42,42);
        speaker_on();
    }
    else //We have a line so execute the correct algorithm
    {
        if(mode == 1) //Simple LF
        {
            speaker_off();
            simple_LF(leftMVal, rightMVal);
        }
        else if (mode == 2) //Intemediate LF
        {
            speaker_off();
            simple_LF_M(lftAv, rgtAv, lrErr);
        }
        else if (mode == 3) //PID
        {
            speaker_off();
             complex_LF(leftOVal, leftMVal, midVal, rightMVal, rightOVal, lrErr);

        }
    }
}

boolean checkMid(int midVal, int lftAv, int rgtAv)
{
    float lineThresh = 0.95; //This value might need tuning
    //Average left and right values here for simplicity
    if (float(float(midVal) / float(((lftAv + rgtAv) / 2))) > lineThresh)
    {
        //No line detected
        return false;
    }
    else  //MID SENSOR OVER THE LINE
    {
        //line detected
        return true;
    }
}

//Returns raw value from a sensor
int getRawSensorVal(int sensor)
{
    return analogRead(sensor);
}

//Returns calibrated value from a sensor
int getCalSensorVal(int sensor)
{
    float sensorVal;

    if(sensor == M){sensorVal = (float)analogRead(sensor);}
    else if(sensor == L1){sensorVal = L1_bias * (float)analogRead(sensor);}
    else if(sensor == L2){sensorVal = L2_bias * (float)analogRead(sensor);}
    else if(sensor == R1){sensorVal = R1_bias * (float)analogRead(sensor);}
    else if(sensor == R2){sensorVal = R2_bias * (float)analogRead(sensor);}

    return (int)sensorVal;
}

//Calibration routine to calibrate the sensors accounting for impedance mismatches in the circuit
//and for IR reflectance properties of different materials.
void sensor_Cal()
{
    //One beep indicates that Cal is about to start
    speaker_on();
    delay(450);
    speaker_off();
    
    Serial.println("Put all sensors over a white background and wait for a beep");
    delay(500);

    int bechmarkM = 0; //Benchmark value all other sensors will align themselves with this value

    int itterations = 250;
    long averageVal = 0;

    //get middle average value
    for (int ii = 0; ii < itterations; ii++)
    {
        delay(1);
        averageVal = averageVal + (long)getRawSensorVal(M);
    }
    bechmarkM = averageVal / itterations;

     long averageValL1 = 0; long averageValL2 = 0; long averageValR1 = 0; long averageValR2 = 0;
     
    //Bias values that will be used to align the senor values
    for (int ii = 0; ii < itterations; ii++)
    {
        delay(1);
        averageValL1 = averageValL1 + (long)getRawSensorVal(L1);
        averageValL2 = averageValL2 + (long)getRawSensorVal(L2);
        averageValR1 = averageValR1 + (long)getRawSensorVal(R1);
        averageValR2 = averageValR2 + (long)getRawSensorVal(R2);
    }

    averageValL1 = averageValL1 / itterations;
    averageValL2 = averageValL2 / itterations; 
    averageValR1 = averageValR1 / itterations;
    averageValR2 = averageValR2 / itterations;

    //Calculate the bias as a percentage
    L1_bias = (float)bechmarkM / (float)averageValL1;
    L2_bias = (float)bechmarkM / (float)averageValL2;
    R1_bias = (float)bechmarkM / (float)averageValR1;
    R2_bias = (float)bechmarkM / (float)averageValR2;

    baseLineValue = (getCalSensorVal(L1) + getCalSensorVal(L2) + getCalSensorVal(R1) + getCalSensorVal(R2)) / 4; //Baseline value of left and right sensors

    if (baseLineValue == 1023)
    {
        Serial.println("The sensors are saturated. If you are in an area of high IR activity either shield the robot or use different value resistors on SB-002");
        speaker_on();
        delay(5000);
        speaker_off();
        executeBTcommand('O');
    }

    // 2 beeps to indicate that cal has finished
    speaker_on();
    delay(450);
    speaker_off();
    delay(200);
    speaker_on();
    delay(450);
    speaker_off();
    Serial.print("Sensors calibrated, baseline value:  ");
    Serial.println(baseLineValue);

    isCald = true;
    // Delay to allow user to put robot over the line
    delay(500);
}
