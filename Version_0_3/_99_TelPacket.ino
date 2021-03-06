/*
 * Tel Packet Module Verison 0.2
 * 
 * Copyright David Bradshaw 2019
 * 
 */

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
