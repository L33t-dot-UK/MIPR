/*
 * Tel Packet Module Verison 0.2.1
 * 
 * Copyright David Bradshaw 2019
 * 
 */
boolean isFirst = true;

void build_Tel_Packet(String sensorBoard)
{
    if (sensorBoard == "NILT") //No sensor board with Odometry Module
    {
        Tel_Packet = (String)left_Velocity  + ',' +  (String)right_Velocity + ',' + (String)battVoltage + ',' + (String)loopTime;
    }
    else if (sensorBoard == "SB001T") //SB001 with Odometry Module
    {
        Tel_Packet = (String)getLeftSensorVal()  + ',' +  (String)getRightSensorVal() + ',' +  (String)left_Velocity  + ',' +  (String)right_Velocity + ',' + (String)battVoltage + ',' + (String)loopTime;
    }
    else if(sensorBoard == "SB001F")//SB001 without Odometry Module
    {
        Tel_Packet = (String)getLeftSensorVal()  + ',' +  (String)getRightSensorVal() + ',' + (String)loopTime;
    }
    else if(sensorBoard == "SB001AT")//SB001A with Odometry Module
    {
        Tel_Packet = String(global_Distance) + ',' + (String)getLeftSensorVal()  + ',' +  (String)getRightSensorVal() + ',' +  (String)left_Velocity  + ',' +  (String)right_Velocity + ',' + (String)battVoltage + ',' + (String)loopTime;
    }
    else if(sensorBoard == "SB001AF")//SB001A without Odometry Module
    {
        Tel_Packet = String(global_Distance) + ',' + (String)getLeftSensorVal()  + ',' +  (String)getRightSensorVal() +  ',' + (String)loopTime;
    }
    else if(sensorBoard == "SB002T")//SB002 with Odometry Module
    {
        Tel_Packet = (String)leftOVal + ',' + (String)leftMVal + ',' + (String)midVal + ',' + (String)rightMVal + ',' + (String)rightOVal + ',' + (String)Input + ',' + (String)Output + ',' + (String)lrErr + ',' + (String)left_Velocity  + ',' +  (String)right_Velocity + ',' + (String)battVoltage + ',' + (String)loopTime;
    }
    else if(sensorBoard == "SB002F")//SB002 without Odometry Module
    {
        Tel_Packet = (String)lrErr + "," + (String)loopTime;
    }



    if(OdoMod_Installed == true && battVoltage < 3.58)
    {
        if (isFirst == true)
        {
            Serial.print("Battery Voltage too Low, resting for 10 seconds");
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
            Serial.println("Enter movement time in milli seconds, this must be between 1 and 999");
            tmp = Serial.readString(); //Read data out of the buffer
            delay(50);
            while(Serial.available() == 0)
            {
                //Wait for data input
            } 
            String iTime = Serial.readString();
            iTime = iTime.substring(0,3);
            
            //Now execute the command, no sensing will work when the command is being executed
            int i_Time = iTime.toInt(); //cast the String variable to an integer
                
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
                    Right(54,54);
                }
                else if (iString.equals("L"))
                {
                    Left(54,54);
                }
            }
            Halt();
            return true;
        }
        else if (iString.equals("O"))
        {
           EEPROM.write(5, '0');
           delay(50);
           asm volatile ("  jmp 0"); //Resets the Arduino
           return false;
        }
        else
        {
            return false;
        }   
}
