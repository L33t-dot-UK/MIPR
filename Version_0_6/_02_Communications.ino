/*
 * Communicaitons Module Verison 0.5 - For use with code 0.6 - SB-002 Line following board
 * https://www.l33t.uk/arduino_projects/mipr/
 * Copyright David Bradshaw 2019
 */

 char listenForBTCommands()
 {
    char BTcommand = "";
    if (Serial.available())
    {
        BTcommand = Serial.read();
    }
    return BTcommand;
 }

 void executeBTcommand(char command)
 {
    if (command == 'F') //Forwards
    {
        softStop();
        Forwards(getLeftSpeed(), getRightSpeed());
    }
    if (command == 'B') //Backwards
    {
        softStop();
        Backwards(getLeftSpeed(), getRightSpeed());
    }
    if (command == 'L') //Left turn
    {
        softStop();
        Left(getLeftSpeed(), getRightSpeed());
    }
    if (command == 'R') //Right turn
    {
        softStop();
        Right(getLeftSpeed(), getRightSpeed());
    }
    if (command == 'S') //Stop
    {
        softStop();
        delay(1);
        Halt();
    }
    if (command == 'C')
    {
        //Calibrate sensors for line follower
        sensor_Cal();
    }
    if (command == 'O') //Stop
    {
        Halt();
        delay(10);
        Serial.print("Enter mode and press enter: ");
        delay(50);
        String tmp = Serial.readString(); //Read data out of the buffer
        delay(50);
        while(Serial.available() == 0)
        {
            //Wait for data input
        } 
        String iString = Serial.readString();
        Serial.print(iString);
        Serial.println("Restarting MIPR in new mode");
        char mode = iString.charAt(0);

        EEPROM.write(5, mode);
        delay(50);
        asm volatile ("  jmp 0"); //Resets the Arduino
    }
 }

 void speaker_on()
 {
    //digitalWrite(2, HIGH);
 }

 void speaker_off()
 {
    //digitalWrite(2, LOW);
 }
 
