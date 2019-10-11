/*
//Author David Bradshaw
//Used to calculate wheel velocity
//This is a rough version of this functionality, it will be imroved upon in later versions
//Version 0.01
*/

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

//Sample the analog waveform and set valus to coarse and fine RPS from this the velocity can
//be calculated.
//We only sample the part of the waveform thats above the mean level, this saves time and
//makes the code simpler. Because of this we guess how long the negative part of the waveform
//will be and in this case we use the figure of 2.3 multiplied by the possitive portion of the 
//waveform.
void SampleWaveForm(boolean reset_Vars)
{
    int reading = analogRead(leftMotorSensor); //left wheel

    if (reset_Vars == true) //Set to true when MOTOR_STATE changes
    {
        left_Timer_Enabled = false;
        right_Timer_Enabled = false;
        left_Wheel_Pit_Counter = 0;
        right_Wheel_Pit_Counter = 0;
        left_Wheel_Rot_Timer = millis();
        right_Wheel_Rot_Timer = millis();
    }

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

    //FOR DEBUGING
    Serial.print(47); //Upper Limit
    Serial.print(',');
    Serial.print(42); //Lower Limit
    Serial.print(',');
    Serial.print(left_Velocity);
    Serial.print(',');
    Serial.println(right_Velocity);
}

//Calculates the velocity in CM per Second
float calc_Velocity()
{
    left_Velocity = (left_Wheel_RPS_Coarse * wheel_Cir);
    right_Velocity = (right_Wheel_RPS_Coarse * wheel_Cir);
    
}
