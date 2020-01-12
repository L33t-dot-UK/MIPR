# New Features in Version 0.5

- Follow Mode added
- Execute command mode added

Follow mode will make the robot maintain a distance from an object. The default value in the code is set to 200mm, this can be changed.
The execute command mode will let you type in a direction command and a time value. The robot will the travel the chosen distance for
the given time. The time values are in milliseconds with a maximum value of 999mS. This is a little clunky but it demonstrates what can be
done using commands. A future update of this mode will allow the user to specify distance rather than time and string commands together.

# Op Modes

To change the Op Mode open serial monitor and connect to MIPR type a capital O for Oscar and then select the desired Op Mode.
Op Modes are as follows;

- 0; Remote controlled
- 1; Light Seeker
- 2; Light Avoider
- 3; Obstacle avoider
- 4; Follow Mode
- 5; Execute command

# File Breakdown;

- Version_0_5.ino - Main Arduino IDE file, contains setup and loop funcitons for MIPR
- _01_Motor_Functions.ino - Contains functions to access MIPR's motor driver
- _02_Communications.ino - Allows access to MIPR's bluetooth functions
- _03_Odometry_Module.ino - Carries out basic odometry calculaitons
- _04_Sensor_Board_001.ino - Provides functions for light seeking sensor board
- _05_Sensor_Board_001A.ino - Provides functions for obstacle avoidance sensor board
- _99_TelPacket.ino - Builds the telemetry packet for broadcast via the Bluetooth module and monitors battery voltage

Please note you must download the Polou Vl53L1X library from github for this code to work.
