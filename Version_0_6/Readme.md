# New Features in Version 0.6

- 


# Op Modes

To change the Op Mode open serial monitor and connect to MIPR type a capital O for Oscar and then select the desired Op Mode.
Op Modes are as follows;

- 0; Remote controlled
- 1; --
- 2; --
- 3; --
- 4; --
- 5; Execute command
- 6; Line Follower Mode

Modes 1 to 4 do not work because they require SB-001 or SB-001A to work.

# File Breakdown;

- Version_0_6.ino - Main Arduino IDE file, contains setup and loop funcitons for MIPR
- _01_Motor_Functions.ino - Contains functions to access MIPR's motor driver
- _02_Communications.ino - Allows access to MIPR's bluetooth functions
- _03_Odometry_Module.ino - Carries out basic odometry calculaitons
- _99_TelPacket.ino - Builds the telemetry packet for broadcast via the Bluetooth module and monitors battery voltage

