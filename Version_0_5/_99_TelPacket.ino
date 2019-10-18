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
