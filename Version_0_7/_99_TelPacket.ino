/*
 * Tel Packet Module Verison 0.2.1
 * 
 * Copyright David Bradshaw 2019
 * 
 */
boolean isFirst = true;
String lineEnding = ";";

void build_Tel_Packet(String sensorBoard)
{
    Tel_Packet = ".";
    if (sensorBoard == "NILT") //No sensor board with Odometry Module
    {
        Tel_Packet =  (String)left_Velocity  + ',' +  (String)leftMotorStatus  + ',' + (String)right_Velocity + ',' + (String)rightMotorStatus  + ',' + (String)battVoltage + ',' + (String)loopTime;
    }
    else if (sensorBoard == "ODO") //SB001 with Odometry Module
    {
        Tel_Packet =  (String)left_Velocity  + ',' +  (String)right_Velocity;
    }
    else if (sensorBoard == "SB001T") //SB001 with Odometry Module
    {
        Tel_Packet = (String)getLeftSensorVal()  + ',' +  (String)getRightSensorVal() + ',' +  (String)left_Velocity  + ',' +  (String)leftMotorStatus  + ',' + (String)right_Velocity + ',' + (String)rightMotorStatus  + ',' + (String)battVoltage + ',' + (String)loopTime;
    }
    else if(sensorBoard == "SB001F")//SB001 without Odometry Module
    {
        Tel_Packet = (String)getLeftSensorVal()  + ',' +  (String)getRightSensorVal() + ',' + (String)loopTime;
    }
    else if(sensorBoard == "SB001AT")//SB001A with Odometry Module
    {
        Tel_Packet = String(global_Distance) + ',' + (String)getLeftSensorVal()  + ',' +  (String)getRightSensorVal() + ',' + (String)left_Velocity  + ',' +  (String)leftMotorStatus  + ',' + (String)right_Velocity + ',' + (String)rightMotorStatus  + ',' + (String)battVoltage + ',' + (String)loopTime;
    }
    else if(sensorBoard == "SB001AF")//SB001A without Odometry Module
    {
        Tel_Packet = String(global_Distance) + ',' + (String)getLeftSensorVal()  + ',' +  (String)getRightSensorVal() +  ',' + (String)loopTime;
    }
    else if(sensorBoard == "SB002T")//SB002 with Odometry Module
    {
        Tel_Packet = (String)leftOVal + ',' + (String)leftMVal + ',' + (String)midVal + ',' + (String)rightMVal + ',' + (String)rightOVal + ',' + (String)Input + ',' + (String)Output + ',' + (String)lrErr + ',' + (String)left_Velocity  + ',' +  (String)leftMotorStatus  + ',' + (String)right_Velocity + ',' + (String)rightMotorStatus  + ',' + (String)battVoltage + ',' + (String)loopTime;
    }
    else if(sensorBoard == "SB002F")//SB002 without Odometry Module
    {
        Tel_Packet = (String)leftOVal + ',' + (String)leftMVal + ',' + (String)midVal + ',' + (String)rightMVal + ',' + (String)rightOVal + ',' + (String)Input + ',' + (String)Output + ',' + (String)lrErr + ',' + (String)loopTime;
    }

    if(OdoMod_Installed == true && battVoltage < 3.58)
    {
        if (isFirst == true)
        {
            strcpy_P(buffer, (char *)pgm_read_word(&(string_table[6])));
            Serial.print(buffer);
            //stop the motors and rest for 10 seconds allowing the load to be taken off the battery
            digitalWrite(13, HIGH); //Turn on the Arduino's LED indicating that the robot is resting for 10 seconds
            Halt();
            delay(10000);
            isFirst = false;
            digitalWrite(13, LOW);
        }
        else
        {
            strcpy_P(buffer, (char *)pgm_read_word(&(string_table[7])));
            Serial.print(buffer);
            Serial.print(battVoltage);
            strcpy_P(buffer, (char *)pgm_read_word(&(string_table[8])));
            Serial.println(buffer);
            Halt();
            speaker_on();
            while(1){} //infinate loop so the robot will not respond
        }
    }    
}


//I have rewritten this function (Op Mode 5) for version 0.7
boolean executeInstruction()
{
    //Used when in instruction mode
    Halt();
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[10])));
        Serial.println(buffer);
        delay(50);
        String tmp = Serial.readString(); //Read data out of the buffer
        delay(50);
        while(Serial.available() == 0)
        {
            //Wait for data input
        } 
        String iString = Serial.readString();

        //STRIP OFF NEWLINE AND CARRIAGE RETURN CHARACTERS IF THEY ARE PRESENT
        if(iString.substring(iString.length() - 2, iString.length()).equals("\r\n")) //Detect NL and CR, we need to remove this
        {
            iString = iString.substring(0,iString.length() - 2);
        }
        else if(iString.substring(iString.length() - 1, iString.length()).equals("\n")) //Detect a NL, we need to remove this
        {
            iString = iString.substring(0,iString.length() - 1);
        }
        else if(iString.substring(iString.length() - 1, iString.length()).equals("\r")) //Detect a CR, we need to remove this
        {
            iString = iString.substring(0,iString.length() - 1);
        }
    
        String cString = parseCommand(iString);
        if (cString.equals("L") || cString.equals("R") || cString.equals("F") || cString.equals("B"))
        {
            //Legal option
            String iTime = parseValue(iString);
            
            //Now execute the command, no sensing will work when the command is being executed
            int i_Time = iTime.toInt(); //cast the String variable to an integer
                
            int elapsedTime = 0;
            int m5startTime = millis();

            strcpy_P(buffer, (char *)pgm_read_word(&(string_table[12])));
            Serial.print(buffer);
            Serial.print(cString);
            Serial.print( " for ");
            Serial.print(i_Time);
            Serial.println(" mS");
            //setup the timer loop
            while(elapsedTime < i_Time)
            {
                elapsedTime = millis() - m5startTime;
                if (cString.equals("F"))
                {   
                    Forwards(dLeftSpeed,dLeftSpeed);
                }
                else if (cString.equals("B"))
                {
                    Backwards(dLeftSpeed,dLeftSpeed);
                }
                else if (cString.equals("R"))
                {
                    Right(dLeftSpeed,dLeftSpeed);
                }
                else if (cString.equals("L"))
                {
                    Left(dLeftSpeed,dLeftSpeed);
                }
            }
            Halt();
            return true;
        }
        else if (iString.equals("O"))
        {
           Serial.println("OOOOOOOOOOOOOOOO Sir");
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
