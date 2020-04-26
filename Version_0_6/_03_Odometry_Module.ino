/*
 * Odometry Module Verison 0.4
 * https://www.l33t.uk/arduino_projects/mipr/
 * Copyright David Bradshaw 2019
 */

int battVpin = A3; //Battery voltage pin
float battV = 0;
float battVoltage = 0;

int leftMotorSensor = A7;
int rightMotorSensor = A6;

float left_Pulsewidth = 0;
long left_Pulse_StartTimer = 0;
boolean left_Timer_Enabled = false;

int right_Pulsewidth = 0;
long right_Pulse_StartTimer = 0;
boolean right_Timer_Enabled = false;

#define wheel_Diameter 4.7
float wheel_Cir = wheel_Diameter * 3.14159;
int wheel_Pits = 8;

int left_Wheel_Pit_Counter = 0;
long left_Wheel_Rot_Timer = 0;
float left_Wheel_RPS_Coarse = 0; //Delayed revs per second readout, updates every revolution
float left_Wheel_RPS_Fine = 0; //Realtime revs per second readout updates every 8th of a rotation

int right_Wheel_Pit_Counter = 0;
long right_Wheel_Rot_Timer = 0;
float right_Wheel_RPS_Coarse = 0; //Delayed revs per second readout, updates every revolution
float right_Wheel_RPS_Fine = 0; //Realtime revs per second readout updates every 8th of a rotation

float left_Velocity = 0;
float right_Velocity = 0;

int maxRight = 0;
int minRight = 1024;
int maxLeft = 0;
int minLeft = 1024;

int meanLeft = 500;
int meanRight = 500;

long left_Refresh_Time = 0;
long right_Refresh_Time = 0;

boolean Left_Motor_Stopped = false;
boolean Right_Motor_Stopped = false;

//Sample the analog waveform and set valus to coarse and fine RPS from this the velocity can
//be calculated.
//We only sample the part of the waveform thats above the mean level, this saves time and
//makes the code simpler. Because of this we guess how long the negative part of the waveform
//will be and in this case we use the figure of 2.3 multiplied by the possitive portion of the 
//waveform.
void SampleWaveForm(boolean reset_Vars)
{
    battV = analogRead(battVpin);
    battVoltage = (battV * (4.65 / 1023.0)); //This value is 4.65 because of the diode on the Arduino's 5v pin and power drain created by the smoothing cct

    if (reset_Vars == true) //Set to true when MOTOR_STATE changes
    {
        left_Timer_Enabled = false;
        right_Timer_Enabled = false;
        left_Wheel_Pit_Counter = 0;
        right_Wheel_Pit_Counter = 0;
        left_Wheel_Rot_Timer = millis();
        right_Wheel_Rot_Timer = millis();
    }

    if((millis() - left_Refresh_Time) > 200) { Left_Motor_Stopped = true;}
    else{Left_Motor_Stopped = false;}
    
    if((millis() - right_Refresh_Time) > 200) { Right_Motor_Stopped = true;}
    else { Right_Motor_Stopped = false;}
    
    if (Left_Motor_Stopped == true)
    {
        left_Wheel_RPS_Coarse = 0;
        left_Wheel_RPS_Fine = 0;
        left_Timer_Enabled = false;
        left_Wheel_Pit_Counter = 0;
        left_Wheel_Rot_Timer = millis();     
    }
    if (Right_Motor_Stopped == true)
    {
        right_Wheel_RPS_Coarse = 0;
        right_Wheel_RPS_Fine = 0;
        right_Timer_Enabled = false;
        right_Wheel_Pit_Counter = 0;
        right_Wheel_Rot_Timer = millis(); 
    }

    int reading = analogRead(leftMotorSensor); //left wheel
    
    if (reading > meanLeft)
    {
         if (left_Timer_Enabled == false)
         {
            left_Pulse_StartTimer = millis();
            left_Timer_Enabled = true;
            left_Wheel_Pit_Counter = left_Wheel_Pit_Counter + 1;
            if (left_Wheel_Pit_Counter == (wheel_Pits - 1))
            {
                //Full wheel rotation
                left_Wheel_Pit_Counter = 0;
                left_Wheel_RPS_Coarse = 1000 / float(millis() - left_Wheel_Rot_Timer);
                left_Wheel_Rot_Timer = millis();
            } 
            left_Refresh_Time = millis();
         }   
    }
    else if (reading < meanLeft)
    {
        if (left_Timer_Enabled == true)
        {
            left_Pulsewidth = millis() - left_Pulse_StartTimer;
            left_Pulsewidth = left_Pulsewidth * 2.3; //Use 2.3 because of the ratio between the holes and black part of the wheel
            left_Wheel_RPS_Fine = 1000 / float(left_Pulsewidth * wheel_Pits);
            left_Timer_Enabled = false;
            calc_Velocity();
        }
    }
    
    reading = analogRead(rightMotorSensor); //right wheel

    if (reading > meanRight)
    {
         if (right_Timer_Enabled == false)
         {
            right_Pulse_StartTimer = millis();
            right_Timer_Enabled = true;
            right_Wheel_Pit_Counter = right_Wheel_Pit_Counter + 1;
            if (right_Wheel_Pit_Counter == (wheel_Pits - 1))
            {
                //Full wheel rotation
                right_Wheel_Pit_Counter = 0;
                right_Wheel_RPS_Coarse = 1000 / float(millis() - right_Wheel_Rot_Timer);
                right_Wheel_Rot_Timer = millis();
            }
            right_Refresh_Time = millis();
         }   
    }
    else if (reading < meanRight)
    {
        if (right_Timer_Enabled == true)
        {
            right_Pulsewidth = millis() - right_Pulse_StartTimer;
            right_Pulsewidth = right_Pulsewidth * 2.3; //Use 2.3 because of the ratio between the holes and black part of the wheel
            right_Wheel_RPS_Fine = 1000 / float(right_Pulsewidth * wheel_Pits);
            right_Timer_Enabled = false;
            calc_Velocity();
        }
    }  
    
}

//Calculates the velocity in CM per Second
float calc_Velocity()
{
    left_Velocity = (left_Wheel_RPS_Coarse * wheel_Cir);
    right_Velocity = (right_Wheel_RPS_Coarse * wheel_Cir);
    
}

void getEEPROM_Values()
{
    if (int(EEPROM.read(0)) == 128)
    {
        maxRight = int(EEPROM.read(1)) * 4;
        minRight = int(EEPROM.read(2)) * 4;
        maxLeft = int(EEPROM.read(3)) * 4;
        minLeft = int(EEPROM.read(4)) * 4;
        meanRight = int(EEPROM.read(6)) * 4;
        meanLeft = int(EEPROM.read(7)) * 4;
    }
    else
    {
        Serial.println("No values found; cALIBRATION must be ran before using the Odometry Module");
        while(1)
        {
            speaker_on();
            delay(500);
            speaker_off();
            delay(500);
        }
    }  
}
