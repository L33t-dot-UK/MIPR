/*
 * Copyright David Bradshaw 2019
 * 
 */
boolean isFirst = true;

void build_Tel_Packet()
{
    Tel_Packet = String(global_Distance) + ',' + (String)leftLDRval_A  + ',' +  (String)rightLDRval_A + ',' +  (String)left_Velocity  + ',' +  (String)right_Velocity + ',' + (String)battVoltage + ',' + (String)loopTime;

    if(OdoMod_Installed == true && battVoltage < 3.58)
    {
        if (isFirst == true)
        {
            //stop the motors and rest for 10 seconds allowing the load to be taken off the battery
            digitalWrite(13, HIGH); //Turn on the Arduino's LED indicating that the robot is resting for 10 seconds
            Halt();
            delay(10000);
            isFirst = false;
            digitalWrite(13, LOW);
        }
        else
        {
            Serial.print("Battery Voltage too Low ");
            Serial.print(battVoltage);
            Serial.println(" Volts");
            Halt();
            speaker_on();
            while(1){} //infinate loop so the robot will not respond
        }
    }    
}

boolean executeInstruction()
{
    //Used when in instruction mode
    Halt();
        Serial.println("Enter the direction that you want MIPR to move in L == left, R == right, F == forward and B == backwards");
        delay(50);
        String tmp = Serial.readString(); //Read data out of the buffer
        delay(50);
        while(Serial.available() == 0)
        {
            //Wait for data input
        } 
        String iString = Serial.readString();

        iString = iString.substring(0,1);
        if (iString.equals("L") || iString.equals("R") || iString.equals("F") || iString.equals("B"))
        {
            //Legal option
            Serial.println("Enter movement time in seconds, this must be between 1 and 9");
            tmp = Serial.readString(); //Read data out of the buffer
            delay(50);
            while(Serial.available() == 0)
            {
                //Wait for data input
            } 
            String iTime = Serial.readString();
            iTime = iTime.substring(0,1);
            
            if (iTime.equals("1") || iTime.equals("2")  || iTime.equals("3")  || iTime.equals("4")  || iTime.equals("5")  || iTime.equals("6")  || iTime.equals("7")  || iTime.equals("8")  || iTime.equals("9") )
            //Now execute the command, no sensing will work when the command is being executed
            {
                int i_Time = iTime.toInt(); //cast the String variable to an integer
                i_Time = i_Time * 1000;
                
                int elapsedTime = 0;
                int m5startTime = millis();

                 Serial.print("Executing command:: ");
                 Serial.print(iString);
                 Serial.print( " for ");
                 Serial.print(i_Time);
                 Serial.println(" mS");
                //setup the timer loop
                while(elapsedTime < i_Time)
                {
                    elapsedTime = millis() - m5startTime;
                    if (iString.equals("F"))
                    {
                        Forwards(96,96);
                    }
                    else if (iString.equals("B"))
                    {
                        Backwards(96,96);
                    }
                    else if (iString.equals("R"))
                    {
                        Right(32,32);
                    }
                    else if (iString.equals("L"))
                    {
                        Left(32,32);
                    }
                }
                Halt();
                return true;
            }
            else
            {
                return false;
            }   
        }
        else
        {
            //illegal Option brake out of the loop and start again
            return false;
        }
}
