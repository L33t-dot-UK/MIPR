# New Features in Version 0.3

- The code is organised in different tabs to make it easier to read.
- Odometry module added that gives battery voltage information and wheel speed info.
- Sensor Board 001 code added goving MIPR the ability to detect light.
- TelPacket code added to make it easier to create telemetry packets.
- Motor funcitons and communicaiton functions have been put in there own tabs.

# Op Modes

To change the Op Mode open serial monitor and connect to MIPR type a capital O for Oscar and then select the desired Op Mode.
Op Modes are as follows;

- 0; Remote controlled
- 1; Light Seeker
- 2; Light Avoider

Once the code is uploaded to MIPR use Telemetry.jar and open the layout named SB001_TEL_A
Once loaded this layout will try to connect to your robot and fail. If it fails select the
correct comm port for your system and the correct baud rate then press connect. Once
connected you should see telemetry data as shown in the You Tube video; https://youtu.be/HkoHo65-CrE

Multilpe .ino files are used to order the code in Arduino IDE each .ino file will have a different funciton.

# File Breakdown;

- SB001_TEL_A.txt - Settings file for Telemetry.jar
- Version_0_3.ino - Main Arduino IDE file, contains setup and loop funcitons for MIPR
- _01_Motor_Functions.ino - Contains functions to access MIPR's motor driver
- _02_Communications.ino - Allows access to MIPR's bluetooth functions
- _03_Odometry_Module.ino - Carries out basic odometry calculaitons
- _04_Sensor_Board_001.ino - Provides functions for light seeking sensor board
- _99_TelPacket.ino - Builds the telemetry packet for broadcast via the Bluetooth module and monitors battery voltage

Please note Telemtry.jar is in the root of this project.
