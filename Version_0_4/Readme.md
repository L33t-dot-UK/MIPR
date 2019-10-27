# New Features in Version 0.4

- Code added for sensor board 001A giving MIPR the ability to detect obstacles

# Op Modes

To change the Op Mode open serial monitor and connect to MIPR type a capital O for Oscar and then select the desired Op Mode.
Op Modes are as follows;

- 0; Remote controlled
- 1; Light Seeker
- 2; Light Avoider
- 3; Obstacle avoider

Once the code is uploaded to MIPR use Telemetry.jar and open the layout named SB001_A
Once loaded this layout will try to connect to your robot and fail. If it fails select the
correct comm port for your system and the correct baud rate then press connect.

# File Breakdown;

SB001_A.txt - Settings file for Telemetry.jar
- Telemetry.jar - Application to view telemetry from MIPR
- Version_0_4.ino - Main Arduino IDE file, contains setup and loop funcitons for MIPR
- _01_Motor_Functions.ino - Contains functions to access MIPR's motor driver
- _02_Communications.ino - Allows access to MIPR's bluetooth functions
- _03_Odometry_Module.ino - Carries out basic odometry calculaitons
- _04_Sensor_Board_001.ino - Provides functions for light seeking sensor board
- _05_Sensor_Board_001A.ino - Provides functions for obstacle avoidance sensor board
- _99_TelPacket.ino - Builds the telemetry packet for broadcast via the Bluetooth module and monitors battery voltage

Please note you must download the Polou Vl53L1X library from github for this code to work.
