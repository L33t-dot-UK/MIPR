/*
 * Tel Packet Module Verison 0.2
 * 
 * Copyright David Bradshaw 2019
 * 
 */

void build_Tel_Packet()
{
    Tel_Packet = (String)leftLDRval_A  + ',' +  (String)rightLDRval_A + ',' +  (String)left_Velocity  + ',' +  (String)right_Velocity + ',' + (String)battVoltage + ',' + (String)loopTime;

    if(OdoMod_Installed == true && battVoltage < 3.58)
    {
        Serial.print("Battery Voltage too Low ");
        Serial.print(battVoltage);
        Serial.println(" Volts");
        Halt();
        speaker_on();
        while(1){} //infinate loop so the robot will not respond
    }
}
