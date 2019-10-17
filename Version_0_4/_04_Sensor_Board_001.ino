/*
 * Sensor Board 001 = Light Following/Avoiding Sensor Board
 * https://www.l33t.uk/arduino_projects/mipr/light-seeking-avoiding-sensor/
 * Copyright David Bradshaw 2019
 * 
 */

 int leftLDR = A1;
 int rightLDR = A2;

 int leftLDRval = 0;
 int rightLDRval = 0;

 int leftLDRval_A = 0;
 int rightLDRval_A = 0;
    
 void get_Sensor_Values()
 {
    leftLDRval = analogRead(rightLDR);
    rightLDRval = analogRead(leftLDR);
 }

int getMotorSpeed(boolean lightSeek, boolean isLeft)
{
    get_Sensor_Values();

    leftLDRval_A = leftLDRval;
    rightLDRval_A = rightLDRval;

    //If both LDR values are within 100 of each other then make MIPR go forward at
    //full speed, this will make the robots movements less twitchy
    if (leftLDRval - rightLDRval < 100 && leftLDRval - rightLDRval > 0)
    {
       if (leftLDRval > 600)
        {
             if (lightSeek == true)
            {
                rightLDRval = 253;   
                leftLDRval = 253;
            }
            else
            {
                rightLDRval = -253;   
                leftLDRval = -253;
            }
        }
    }
    else if (rightLDRval - leftLDRval < 100 && rightLDRval - leftLDRval > 0)
    {
        if (rightLDRval > 600)
        {
            if (lightSeek == true)
            {
                rightLDRval = 253;   
                leftLDRval = 253;
            }
            else
            {
                rightLDRval = -253;   
                leftLDRval = -253;
            }
        }
    }

     
    if(leftLDRval > rightLDRval)
    {
        rightLDRval = 0;
    }
    if (rightLDRval > leftLDRval)
    {
        leftLDRval = 0;
    }

    rightLDRval = rightLDRval / 4;
    leftLDRval = leftLDRval / 4;
    
    if(lightSeek == true)
    {
        //set to light seek mode
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
    else if (lightSeek == false)
    {
        //set to light avoid mode
    }
    
}
