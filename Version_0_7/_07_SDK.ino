/*
 * SDK Version 1.0
 * Op Mode 9 is SDK mode and will allow you to control MIPR with more commands enabling you to
 * control the robot with languages like Python, Java, C++, etc.
 * --INSERT SDK LINK HERE
 * Copyright David Bradshaw 2020
  */


//Defines the character to be used to seperate commands and values i.e. REFF:20 the command would be REFF and value 20
char parseChar = ':';

void executeSDKcommand(String command)
{
    if(checkString(command))
    {
        //We have a Command Value pair
        if (parseCommand(command).equals("REF")) //Changes the packet refresh rate
        {
            telPacketRefresh = parseValue(command).toInt();
        }
        else if (parseCommand(command).equals("MAXD")) //Changes the maximum measureable distance of the VL53L1 sensor in mm range 1 - 4000
        {
            maxDistance = parseValue(command).toInt();
        }
        else if (parseCommand(command).equals("TEL")) //Sets what telemetry packet you want to recieve. ENSURE THAT YOU HAVE THE CORRECT SENSOR BOARD ATTACHED!
        {
            telPacketType = parseValue(command);
            if (telPacketType.equals("SB001AT") || telPacketType.equals("SB001AF"))
            {
                //Setup SB001A
                setup_Sensor();  //PLEASE NOTE IF THE WRONG SENSOR BOARD IS INSERTED THE ROBOT WILL FREEZE AND BE NON RESPONSIVE!
            }
        }
        else if (parseCommand(command).equals("SPD"))
        {
            setLeftSpeed(parseValue(command).toInt());
            setRightSpeed(parseValue(command).toInt());
        }
        else if (parseCommand(command).equals("SLSPD"))
        {
            setLeftSpeed(parseValue(command).toInt());
        }
        else if (parseCommand(command).equals("SRSPD"))
        {
            setRightSpeed(parseValue(command).toInt());
        }
        else if (parseCommand(command).equals("READPIN"))
        {
            
        }
        else if (parseCommand(command).equals("SPD"))
        {
            
        }
    }
    else //We have an ordinary command
    {
        if (command.equals("TELON")) //Turns the Tel Packets on
        {
            telPacketEn = true;
        }
        else if(command.equals("TELOFF")) //Turns the Tel Packets off
        {
            telPacketEn = false;
        }
        else if (command.equals("SETSEN")) //This must be done before using the DIST command
        {
            setup_Sensor();  //PLEASE NOTE IF THE WRONG SENSOR BOARD IS INSERTED THE ROBOT WILL FREEZE AND BE NON RESPONSIVE!
        }
        else if (command.equals("DIST")) //Returns a distance measurement
        {
            Serial.println(getDist());
        }
        else if (command.equals("ISCALD")) //Returns true if SB002 is calibrated
        {
            Serial.println(isCald);
        }
        else if (command.equals("GET002BIAS")) //Returns bias values for a calibrated SB002
        {
            Serial.print(L2_bias);
            Serial.println(",");
            Serial.print(L1_bias);
            Serial.println(",");
            Serial.print(R1_bias);
            Serial.println(",");
            Serial.print(R2_bias);
            Serial.println(",");
            Serial.println(baseLineValue);
        }
        else if (command.equals("GETCALDVAL")) //Returns SB002 sensor readings after calibration
        {
            //get calibrated Values for all sensors
            leftOVal = getCalSensorVal(L1);
            rightOVal = getCalSensorVal(R1);
            leftMVal = getCalSensorVal(L2);
            rightMVal = getCalSensorVal(R2);
            midVal = getCalSensorVal(M);
            Serial.print(leftOVal);
            Serial.println(",");
            Serial.print(leftMVal);
            Serial.println(",");
            Serial.print(midVal);
            Serial.println(",");
            Serial.print(rightMVal);
            Serial.println(",");
            Serial.println(rightOVal);
        }
        else if (command.equals("GETUNCALDVAL")) //Returns SB002 sensor readings before calibration
        {
            //get calibrated Values for all sensors
            leftOVal = getRawSensorVal(L1);
            rightOVal = getRawSensorVal(R1);
            leftMVal = getRawSensorVal(L2);
            rightMVal = getRawSensorVal(R2);
            midVal = getRawSensorVal(M);
            Serial.print(leftOVal);
            Serial.println(",");
            Serial.print(leftMVal);
            Serial.println(",");
            Serial.print(midVal);
            Serial.println(",");
            Serial.print(rightMVal);
            Serial.println(",");
            Serial.println(rightOVal);
        }
        else if (command.equals("SON")) 
        {
            speaker_on();
        }
        else if (command.equals("SOFF"))
        {
            speaker_off();
        }
        else if (command.equals("GLSPD")) 
        {
            Serial.println(getLeftSpeed());
        }
        else if (command.equals("GRSPD")) 
        {
            Serial.println(getRightSpeed());
        }
        else if (command.equals("LSTU")) 
        {
            Serial.println(getLeftMotorStatus());
        }
        else if (command.equals("RSTU")) 
        {
            Serial.println(getRightMotorStatus());
        }
        else if (command.equals("ENODO")) 
        {
            OdoMod_Installed = true;
        }
        else if (command.equals("DIODO")) 
        {
            OdoMod_Installed = false;
        }
        else if (command.equals("OLSPD")) 
        {
            Serial.println(left_Velocity);
        }
        else if (command.equals("ORSPD")) 
        {
            Serial.println(right_Velocity);
        }
        else if (command.equals("BVOLT")) 
        {
            Serial.println(battVoltage);
        }
        else if (command.equals("LDRL")) 
        {
            get_Sensor_Values();
            Serial.println(leftLDRval);
        }
        else if (command.equals("LDRR")) 
        {
            get_Sensor_Values();
            Serial.println(rightLDRval);
        }
        
    }
    Serial.println(command); //Return the command as a acknowledgment THIS COULD BE MOVED!!!!!!!!
}

//The heartBeat will update sensor values depending on what Tel Packet has been chosen if any
void heartBeat()
{
    if(telPacketType.equals("SB001AT") || telPacketType.equals("SB001AF"))
    {
        //Get a distance measurment and LDR readings
        get_Sensor_Values();
        getDist();  
    }
    else if(telPacketType.equals("SB001T") || telPacketType.equals("SB001F"))
    {
        //Get a distance measurment and LDR readings
        get_Sensor_Values(); 
    }
    else if (telPacketType.equals("SB002T") || telPacketType.equals("SB002F"))
    {
        //get calibrated Values for all sensors
        leftOVal = getCalSensorVal(L1);
        rightOVal = getCalSensorVal(R1);
        leftMVal = getCalSensorVal(L2);
        rightMVal = getCalSensorVal(R2);
        midVal = getCalSensorVal(M);
    }
    //get un-calibrated Values for all sensors
    else if (telPacketType.equals("SB002UCT") || telPacketType.equals("SB002UCF"))
    {
        //get calibrated Values for all sensors
        leftOVal = getRawSensorVal(L1);
        rightOVal = getRawSensorVal(R1);
        leftMVal = getRawSensorVal(L2);
        rightMVal = getRawSensorVal(R2);
        midVal = getRawSensorVal(M);
    }
}

boolean checkString(String command)
{
    for (byte ii = 0; ii < command.length(); ii++)
    {
        if(command.charAt(ii) == parseChar)
        {
            return true;
        }
    }
    return false;
}
/*
 * Returns the command when command value paris are used
 */
String parseCommand(String command)
{
    String retCmd = "";
    for (byte ii = 0; ii < command.length(); ii++)
    {   
        if(command.charAt(ii) == parseChar)
        {
            return retCmd;
        }
        else
        {
            retCmd = retCmd + String(command.charAt(ii));
        }
    }
    return retCmd;
}

/*
 * Returns the value when command value pairs are used
 */
String parseValue(String command)
{
    String retVal = "";
    for (int ii = 0; ii < command.length(); ii++)
    {   
        if(command.charAt(ii) == parseChar)
        {
            retVal = command.substring(ii + 1, command.length());
            return retVal;
        }
    }
    return retVal;
}
