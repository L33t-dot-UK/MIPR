/*
 * Communicaitons Module Verison 0.6 - For use with code 0.6 - SB-002 Line following board
 * https://www.l33t.uk/arduino_projects/mipr/
 * Copyright David Bradshaw 2019
 * 
 * Now supports multi character commands and detects newline and carriage return
 * characters at the end of commands
 */

 String listenForBTCommands()
 {
    String BTcommand = ".";
    Serial.setTimeout(5);
    Serial.flush();
    
    if (Serial.available())
    {
        BTcommand = Serial.readString();
    }

    //STRIP OFF NEWLINE AND CARRIAGE RETURN CHARACTERS IF THEY ARE PRESENT
    if(BTcommand.substring(BTcommand.length() - 2, BTcommand.length()).equals("\r\n")) //Detect NL and CR, we need to remove this
    {
        BTcommand = BTcommand.substring(0,BTcommand.length() - 2);
    }
    else if(BTcommand.substring(BTcommand.length() - 1, BTcommand.length()).equals("\n")) //Detect a NL, we need to remove this
    {
        BTcommand = BTcommand.substring(0,BTcommand.length() - 1);
    }
    else if(BTcommand.substring(BTcommand.length() - 1, BTcommand.length()).equals("\r")) //Detect a CR, we need to remove this
    {
        BTcommand = BTcommand.substring(0,BTcommand.length() - 1);
    }
    return BTcommand;
 }

 void executeBTcommand(String command)
 {  
    if (command.equals("F")) //Forwards
    {
        softStop();
        Forwards(getLeftSpeed(), getRightSpeed());
    }
    
    else if (command.equals("B")) //Backwards
    {
        softStop();
        Backwards(getLeftSpeed(), getRightSpeed());
    }
    else if (command.equals("L")) //Left turn
    {
        softStop();
        Left(getLeftSpeed(), getRightSpeed());
    }
    
    else if (command.equals("R")) //Right turn
    {
        softStop();
        Right(getLeftSpeed(), getRightSpeed());
    }
    
    else if (command.equals("S")) //Stop
    {
        softStop();
        delay(1);
        Halt();
    }
    else if (command.equals("C"))
    {
        //Calibrate sensors for line follower
        sensor_Cal();
    }
    
    else if(command.equals("1"))
    {
        setLeftSpeed(40);
        setRightSpeed(40);
    }
    
    else if(command.equals("2"))
    {
        setLeftSpeed(63);
        setRightSpeed(63);
    }
    
    else if(command.equals("3"))
    {
        setLeftSpeed(86);
        setRightSpeed(86);
    }
    
    else if(command.equals("4"))
    {
        setLeftSpeed(108);
        setRightSpeed(108);
    }
    
    else if(command.equals("5"))
    {
        setLeftSpeed(130);
        setRightSpeed(130);
    }
    
    else if(command.equals("6"))
    {
        setLeftSpeed(153);
        setRightSpeed(153);
    }  
      
    else if(command.equals("7"))
    {
        setLeftSpeed(176);
        setRightSpeed(176);
    }
    
    else if(command.equals("8"))
    {
        setLeftSpeed(200);
        setRightSpeed(200);
    }
    
    else if(command.equals("9"))
    {
        setLeftSpeed(255);
        setRightSpeed(255);
    }
    else if(command.equals("v"))
    {
        speaker_off();
    }
    else if(command.equals("V"))
    {
        speaker_on();
    }
    
    else if (command.equals("O")) 
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

    //SDK commands, will only execute if the above commands has not been used
    else if (opMode == '9')
    {
        executeSDKcommand(command);
    }
 }

 void speaker_on()
 {
    digitalWrite(2, HIGH);
 }

 void speaker_off()
 {
    digitalWrite(2, LOW);
 }
 
