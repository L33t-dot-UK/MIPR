/*
 * Sensor Board 001 = Light Following/Avoiding Sensor Board
 * Version 1.0
 * https://www.l33t.uk/arduino_projects/mipr/light-seeking-avoiding-sensor/
 * Copyright David Bradshaw 2019
 * 
 */

int leftLDR = A1;
int rightLDR = A2;

int leftLDRval = 0;
int rightLDRval = 0;

void get_Sensor_Values()
{
   leftLDRval = analogRead(rightLDR);
   rightLDRval = analogRead(leftLDR);
}

//lightSeek will put the robot in light seek or avoid mode
//isLeft will return the value from the left sensor or the right sensor, true==left, false==right
int getMotorSpeed(boolean lightSeek, boolean isLeft)
{
    get_Sensor_Values();

    //If both LDR values are within 100 of each other then make MIPR go forwards or backwards at
    //full speed, this will make the robots movements less twitchy
    if (abs(leftLDRval - rightLDRval) < 100)
    {
       if (leftLDRval > 600 || rightLDRval > 600)
        {
            //Go forwards at full speed (almost)!
            if (lightSeek == true)
            {
                rightLDRval = 1023;   
                leftLDRval = 1023;
            }
            else
            {
                //Go backwards at full speed!
                rightLDRval = -1023;   
                leftLDRval = -1023;
            }
        }
    }
    //If the light is coming from the left stop the right motor
    else if (leftLDRval > rightLDRval)
    {
        rightLDRval = 0;
    }
    //If the light is coming from the right stop the left motor
    else if (rightLDRval > leftLDRval)
    {
        leftLDRval = 0;  
    }

    //divide by 4 so the value is between 0 - 255
    rightLDRval = rightLDRval / 4;
    leftLDRval = leftLDRval / 4;

    if(isLeft == true)
    {
        //return left sensor value
        return (leftLDRval);
    }
    else if (isLeft == false)
    {
        //return right sensor value
        return (rightLDRval);
    }    
}

int getLeftSensorVal()
{
    return leftLDRval;
}

int getRightSensorVal()
{
    return rightLDRval;
}
