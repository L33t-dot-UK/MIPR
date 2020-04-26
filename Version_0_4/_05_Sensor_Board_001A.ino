/*
 * Sensor Board 001A = Light Following/Avoiding with added obstacle avoidance using VL53L1X senor
 * https://www.l33t.uk/arduino_projects/mipr/obstacle-avoiding-sensor
 * 
 * This code uses the Polou VL53L1X Library that can be downloaded from https://github.com/pololu/vl53l1x-arduino
 * Copyright David Bradshaw 2019
 * 
 * If you request a measurement quicker than the refresh rate then the sensor will block your code!!!!!
 * 
 */


#include <Wire.h>
#include <VL53L1X.h>

VL53L1X sensor;
#define maxDistance 2000

int dirCounter = 0;
int global_Distance = 0;

void setup_Sensor()
{
    Wire.begin();
    Wire.setClock(400000); // use 400 kHz I2C

    delay(10);
    
    sensor.setTimeout(500);
    
    if (!sensor.init())
    {
         Serial.println("VL53L1X Fails to startup");
    }
    else
    {
        sensor.setDistanceMode(VL53L1X::Long);
        sensor.setMeasurementTimingBudget(50000); //Timing budget is 50mS
        sensor.startContinuous(50); //Get a new value every 50mS
    }
}

int getDist()
{
   int reading = 0;
   reading = sensor.read();

   //if reading is greater than maxdistance then return max distance
   //this is due to unreliable readings after a certain distance
   if (reading > maxDistance)
   {
       reading = maxDistance;
   }

   global_Distance = reading; 
   return reading;    
}

//A very basic path finding algorithm 
void basicPathFinder(int minDistance)
{
    if (getDist() < minDistance +1)
    {
        if (dirCounter < 20)
        {
            dirCounter = dirCounter + 1;
            Right(64,64);
        }
        else
        {
            dirCounter = dirCounter + 1;
            Left(64,64);
            if (dirCounter > 40)
            {
                dirCounter = 0;
            }
        }
    }
    else
    {
        Forwards(96, 96);
    }
}
