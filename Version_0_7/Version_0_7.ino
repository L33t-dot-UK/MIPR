      
/*
 * MIPR Core Code Verison 0.7
 * https://www.l33t.uk/arduino_projects/mipr/sdk/
 * Copyright David Bradshaw 2020
 * 
 */

#include <EEPROM.h>

boolean OdoMod_Installed = false; //Change this to false if you do not have the odometry module installed
String Tel_Packet = ".";
char opMode = '0'; //Operating mode 0 == Remote control, 1 == LDR Board seek, 2 == LDR avoid, 3 == object sense, 4 == object follow, 5 == instruction mode

int startTime = 0;
int loopTime = 0;

int telPacketRefresh = 20;
int telPacketTimer = 0;
long telStartTimer = 0;

String telPacketType = "NIL";
boolean telPacketEn = false;

/*
 * Verison 0.7 memory improvements
 * All strings will be declared here and saved to PROGMEM in order to free up some RAM
 * Strings use alot of space in RAM and by doing this we will save 316 bytes which is around 16% of available RAM for an Arduino Pro Mini
 */

    #include <avr/pgmspace.h>
    
    char buffer[120]; //120 characters maximum size of any string message, if you need to save RAM you can decrease this and truncate the below strings
    
    const char string_0[] PROGMEM = "MIPR Started in mode ";
    const char string_1[] PROGMEM = "Setting up SB002";
    const char string_2[] PROGMEM = "Enter mode and press enter: ";
    const char string_3[] PROGMEM = "Sensors calibrated, baseline value:  ";
    const char string_4[] PROGMEM = "The sensors are saturated. If you are in an area of high IR activity either shield the robot or use different value resistors on SB-002";  //116 characters long
    const char string_5[] PROGMEM = "Put all sensors over a white background and wait for a beep";
    const char string_6[] PROGMEM = "Battery Voltage too Low, resting for 10 seconds";
    const char string_7[] PROGMEM = "Battery Voltage too Low ";
    const char string_8[] PROGMEM = " Volts";
    const char string_9[] PROGMEM = "Restarting MIPR in new mode"; 
    const char string_10[] PROGMEM = "Enter your command followed by the amount of time to execute the command for i.e. F:200 move forwards for 200mS";
    const char string_11[] PROGMEM = "The commands are as follows F, B, L, R follow each command with a colon and time";
    const char string_12[] PROGMEM = "Executing command: ";
    
    const char *const string_table[] PROGMEM = {string_0, string_1, string_2, string_3, string_4, string_5, string_6, string_7, string_8, string_9, string_10, string_11, string_12}; //Save all strings in a table

void setup() 
{
    pinMode(2, OUTPUT);
    Serial.begin(57600); //Change this to 9600 if you do not use the HC-05 for programming MIPR
    
    if (OdoMod_Installed == true)
    {
        getEEPROM_Values();
    }
    
    opMode = int(EEPROM.read(5));  
    strcpy_P(buffer, (char *)pgm_read_word(&(string_table[0])));
    Serial.print(buffer);
    Serial.println(opMode);
    delay(100);
    
    if(opMode == '3' || opMode == '4')
    {
        setup_Sensor();
    }

    if (opMode == '6' || opMode == '7' || opMode == '8')
    {
        strcpy_P(buffer, (char *)pgm_read_word(&(string_table[1])));
        Serial.println(buffer);
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

        get_Sensor_Values(); //incase we want to use these later
        if (OdoMod_Installed == true){build_Tel_Packet("SB001AT");} else {build_Tel_Packet("SB001AF");}
        if (telPacketRefresh < 50)
        {
            telPacketRefresh = 50; //If we get measurements from the VL53L1 sensor quicker than this the code will be blocked
                                    //Minmum refresh rate in mode 3 is 50mS.
        }
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
        //SDK Mode use this mode if you want to control MIPR using a program
        if(telPacketEn) //Only build the telPacket if it is enabled
        {
            build_Tel_Packet(telPacketType);
        }
        else
        {
            Tel_Packet = ".";
        }
        heartBeat();
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
        if(Tel_Packet.equals(".")){} //Dont print if tel packet is the default value
        else if (Tel_Packet.equals("")){} //Dont print if tel packet is empty
        else{Serial.println(Tel_Packet);}
        telStartTimer = millis();
    }
    telPacketTimer = millis() - telStartTimer;
    
    //Listens for BT commands and allows us to change modes
    String command = listenForBTCommands();
    if (command.equals("."))
    {
    }else
    {
        executeBTcommand(command);
    }

    loopTime = millis() - startTime;
}



 
