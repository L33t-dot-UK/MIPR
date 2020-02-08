/*
 * Line Follower Module Verison 0.1
 * https://www.l33t.uk/arduino_projects/mipr/
 * Copyright David Bradshaw 2019
 * 
 * This is designed for tracks that use black lines on white background
 * 
 */


int L1 = A5; //Outer Left
int L2 = A4; //Inner Left
int M = A2; //Middle
int R2 = A1; //Inner Middle
int R1 = A0; //Outer Middle

float L1_bias = 1;
float L2_bias = 1;
float R1_bias = 1;
float R2_bias = 1;

int leftVal = 0;
int rightVal = 0;
int midVal = 0;

int baseLineValue = 0;

void line_FollowerSetup()
{
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
    pinMode(A6, INPUT);
    pinMode(A7, INPUT);
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
    Serial.println("Put all sensors over a white background and wait for a beep");
    delay(2000);

    int bechmarkM = getRawSensorVal(M); //Benchmark value all other sensors will align themselves with this value
    
    //Bias values that will be used to align the senor values
    L1_bias = (float)bechmarkM / (float)getRawSensorVal(L1);
    L2_bias = (float)bechmarkM / (float)getRawSensorVal(L2);
    R1_bias = (float)bechmarkM / (float)getRawSensorVal(R1);
    R2_bias = (float)bechmarkM / (float)getRawSensorVal(R2);

    baseLineValue = (getCalSensorVal(L1) + getCalSensorVal(L2) + getCalSensorVal(R1) + getCalSensorVal(R2)) / 4; //This value is what all sensors will aim for
    /*
    speaker_on();
    delay(450);
    speaker_off();
    */
}

//Simple line follower using just 2 sensors L2 and R2. You need a fairly thick line for this to work properly.
void simple_LF()
{
    leftVal = getCalSensorVal(L2);
    rightVal = getCalSensorVal(R2);

    int leftMax = leftVal + (leftVal * 0.2);
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
    else if (leftVal < 500 && rightVal < 500)
    {
        softStop();
    } 
}

//Simple line follower using 3 sensors M, L2 and R2.
//Works better than the above algorithm, mpre accurate and faster however still poor performing, we
//can do much better with an algorithm that adjusts its own speed depending on the values sensed by the sensor.
void simple_LF_M()
{
    leftVal = getCalSensorVal(L2);
    rightVal = getCalSensorVal(R2);
    midVal =  getCalSensorVal(M);
    
    int leftMax = leftVal + (leftVal * 0.2);
    int leftMin = leftVal - (leftVal * 0.2);

    if (midVal < leftVal && midVal < rightVal)
    {
        //Centre sensor is over the line, go forward
        Forwards(150,150);
    }
    else if (leftVal < midVal && leftVal < rightVal)
    {
        Left(40,40);
    }
    else if (rightVal < midVal && rightVal < leftVal)
    {
       Right(40,40); 
    }
    else if (rightVal < midVal)
    {
        Right(40,40); 
    }
    else if (leftVal < midVal)
    {
        Left(40,40);
    }
    else
    {
        Halt();
    }
}

//Complex line follower using a PID and all 5 sensors 
void complex_LF()
{
    //Get sensor Values
    int L1_Val = getCalSensorVal(L1);
    int L2_Val = getCalSensorVal(L2);
    int R1_Val = getCalSensorVal(R1);
    int R2_Val = getCalSensorVal(R2);

    int meanLeft = (L1_Val + L2_Val) / 2;
    int meanRight = (R1_Val + R2_Val) / 2;

    int leftError = baseLineValue - meanLeft;
    int rightError = baseLineValue - meanRight;

    /*
     * IF THE ERROR IS A POSSITIVE NUMBER OR 0 THEN THE SIDE IS OVER THE WHITE PART OF THE PAPER I.E. NO LINE
     * IF THE ERROR IS NEGATIVE THEN A LINE CAN BE DETECTED AND THE ROBOT SHOULD MOVE IN THE OPPOSITE DIRECTION
     * BY A CERTAIN AMOUNT.
     */

     
}

void test_proc()
{ 
    
  delay(20);
  
  Serial.print(getRawSensorVal(L1));
 Serial.print(",");
 Serial.print(getRawSensorVal(L2));
  Serial.print(",");
  Serial.print(getRawSensorVal(M));
   Serial.print(",");
   Serial.print(getRawSensorVal(R2));
    Serial.print(",");
   Serial.println(getRawSensorVal(R1));
/*
     delay(20);
  Serial.print(L1_bias);
 Serial.print(",");
 Serial.print(L2_bias);
  Serial.print(",");
  Serial.print(R1_bias);
   Serial.print(",");
   Serial.println(R2_bias);
*/
}
