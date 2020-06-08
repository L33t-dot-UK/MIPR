
/*
 * MIPR Core Code Verison 0.5
 * https://www.l33t.uk/arduino_projects/mipr/
 * Copyright David Bradshaw 2019
 * 
 * For use with SB-001, SB-001A or no sensor board; see below note (light avoidance and obstacle avoidance)
 * 
 */

#include <EEPROM.h>

boolean OdoMod_Installed = false; //Change this to false if you do not have the odometry module installed
String Tel_Packet = "";
char opMode = '0'; //Operating mode 0 == Remote control, 1 == LDR Board seek, 2 == LDR avoid, 3 == object sense, 4 == object follow, 5 == instruction mode

int startTime = 0;
int loopTime = 0;

int telPacketRefresh = 200;
long telPacketTimer = 0;
long telStartTimer = 0;

void setup() 
{
    pinMode(2, OUTPUT);
    Serial.begin(57600); //Change this 9600 if you do not the HC-05 for programming MIPR
    
    if (OdoMod_Installed == true)
    {
        getEEPROM_Values();
    }

    setup_Sensor();  //Comment this out if your not using the VL53XL1 sensor, if you don't the code will hang.
    
    opMode = int(EEPROM.read(5));  
    Serial.println("");
    Serial.print("MIPR Started in mode ");
    Serial.println(opMode);
    delay(1000);

    //To let the user know that MIPR is ready
    speaker_on();
    delay(100);
    speaker_off();
    delay(100);
    speaker_on();
    delay(100);
    speaker_off();
    
}

void loop() 
{
    startTime = millis();
    if (opMode == '0')
    {
        //Do Nothing
        if (OdoMod_Installed == true){build_Tel_Packet("NILT");}
    }
    else if(opMode == '1')
    {
        Forwards(getMotorSpeed(true, true), getMotorSpeed(true, false));
        if (OdoMod_Installed == true){build_Tel_Packet("SB001T");} else {build_Tel_Packet("SB001F");}
    }
    else if(opMode == '2')
    {
        Forwards(getMotorSpeed(true, false), getMotorSpeed(true, true));
        if (OdoMod_Installed == true){build_Tel_Packet("SB001T");} else {build_Tel_Packet("SB001F");}
    }
    else if(opMode == '3')
    {
        //The getDist call is below in the telPacketTimer if clause
        //This is so we don't request an update from the sensor too quickly
         get_Sensor_Values(); //set these values in case we want to use them
         if (OdoMod_Installed == true){build_Tel_Packet("SB001AT");} else {build_Tel_Packet("SB001AF");}
    }
    else if(opMode == '4')
    {
        //Follow object mode
        followMode(200);
        get_Sensor_Values(); //set these values in case we want to use them
        if (OdoMod_Installed == true){build_Tel_Packet("SB001AT");} else {build_Tel_Packet("SB001AF");}
    }
    else if(opMode == '5')
    {
        //Instruction Mode
        //This mode will be implemented in the TelPacket module
        executeInstruction();
        get_Sensor_Values(); //set these values in case we want to use them
    }
    else
    {
        opMode = 0; //If the opMode is currupted
    }

    if (OdoMod_Installed == true)
    {
        SampleWaveForm(false); //calc velocity data from odo module
        calc_Velocity();
    }

    if (telPacketTimer > telPacketRefresh)
    {
       if(opMode == '3') {basicPathFinder(500);}
       Serial.println(Tel_Packet);       
       telStartTimer = millis();
    }
    telPacketTimer = millis() - telStartTimer;
    
    //Listens for BT commands and allows us to change modes
    char command = listenForBTCommands();
    if (command != '.')
    {
        executeBTcommand(command);
    }
    loopTime = millis() - startTime;
}



 
