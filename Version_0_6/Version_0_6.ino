      
/*
 * MIPR Core Code Verison 0.6
 * https://www.l33t.uk/arduino_projects/mipr/
 * Copyright David Bradshaw 2019
 * 
 * For use with SB-002 (Line Follower)
 * 
 */

#include <EEPROM.h>

boolean OdoMod_Installed = false; //Change this to false if you do not have the odometry module installed
String Tel_Packet = "";
char opMode = '0'; //Operating mode 0 == Remote control, 1 == LDR Board seek, 2 == LDR avoid, 3 == object sense, 4 == object follow, 5 == instruction mode

int startTime = 0;
int loopTime = 0;

int telPacketRefresh = 20;
long telPacketTimer = 0;
long telStartTimer = 0;
    
void setup() 
{
    pinMode(2, OUTPUT);
    Serial.begin(57600); //Change this to 9600 if you do not use the HC-05 for programming MIPR
    
    if (OdoMod_Installed == true)
    {
        getEEPROM_Values();
    }
    
    opMode = int(EEPROM.read(5));  
    Serial.println("");
    Serial.print("MIPR Started in mode ");
    Serial.println(opMode);
    delay(100);
    
    if(opMode == '3' || opMode == '4')
    {
        Serial.println("Setting up SB001A");
        setup_Sensor();
    }

    if (opMode == '6' || opMode == '7' || opMode == '8')
    {
        Serial.println("Setting up SB002");
        line_FollowerSetup();
    }
  
    //To let the user know that MIPR is ready
    speaker_on();
    delay(100);
    speaker_off();
    delay(100);
    speaker_on();
    delay(100);
    speaker_off();
    delay(150);  
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
        //get_Sensor_Values(); //set these values in case we want to use them
        //if (OdoMod_Installed == true){build_Tel_Packet("SB001AT");} else {build_Tel_Packet("SB001AF");}
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
    else if(opMode == '6') //Line follower mode SIMPLE
    {
        LFHandler(1, false);
        if (OdoMod_Installed == true){build_Tel_Packet("SB002T");} else {build_Tel_Packet("SB002F");}
    }
    else if(opMode == '7') //Line follower mode INTERMEDIATE
    {
        LFHandler(2, false);
        if (OdoMod_Installed == true){build_Tel_Packet("SB002T");} else {build_Tel_Packet("SB002F");}
    }
    else if(opMode == '8') //Line follower mode ADVANCED
    {
        LFHandler(3, false);
        if (OdoMod_Installed == true){build_Tel_Packet("SB002T");} else {build_Tel_Packet("SB002F");}
    }
    else if(opMode == '9')
    {
        
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



 
