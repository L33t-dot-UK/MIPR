/*
//Author David Bradshaw
//Allows users to communicate with MIPR over Bluetooth
//Visit https://www.l33t.uk/MIPR for more information
*/

int LEFTSPEED = 254;
int RIGHTSPEED = 254;

char listenForBTCommands()
 {
    char BTcommand = "";
    if (Serial.available())
    {
        BTcommand = Serial.read();
        Serial.flush();
    }
    return BTcommand;
 }

 void executeBTcommand(char command)
 {
    if (command == 'F') //Forwards
    {
        softStop();
        Forwards(LEFTSPEED, RIGHTSPEED);
        Serial.print(command);
        Serial.println(" - Command Received");
    }
    if (command == 'B') //Backwards
    {
        softStop();
        Backwards(LEFTSPEED, RIGHTSPEED);
        Serial.print(command);
        Serial.println(" - Command Received");
    }
    if (command == 'L') //Left turn
    {
        softStop();
        Left(LEFTSPEED, RIGHTSPEED);
        Serial.print(command);
        Serial.println(" - Command Received");
    }
    if (command == 'R') //Right turn
    {
        softStop();
        Right(LEFTSPEED, RIGHTSPEED);
        Serial.print(command);
        Serial.println(" - Command Received");
    }
    if (command == 'S') //Stop
    {
        softStop();
        delay(1);
        Halt();
        Serial.print(command);
        Serial.println(" - Command Received");
    }
    if (command == 'C') //Stop
    {
        Serial.print(command);
        Serial.println(" - Command Received");
        Serial.println("RUNNING ODOMETRY CAL THIS WILL TAKE 5 SECONDS");
        _01_run_Wheel_Sensor_Cal(true);
    }
    if (command == 'P') //Stop
    {
        Halt();
        delay(10);
        Serial.print("Enter mode : ");
        delay(50);
        String tmp = Serial.readString(); //Read data out of the buffer
        delay(50);
        while(Serial.available() == 0)
        {
            //Wait for data input
        }
        String iString = Serial.readString();
        Serial.println(iString);
        char mode = iString.charAt(0);
        EEPROM.write(5, mode);
        delay(50);
        asm volatile ("  jmp 0"); //Resets the Arduino
    }
 }
 
