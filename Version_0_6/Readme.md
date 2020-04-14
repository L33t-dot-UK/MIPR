STILL IN DEVELOPMENT DO NOT USE

REFERENCE: https://www.l33t.uk/arduino_projects/mipr/line-following-sensor-board/ \
REFERENCE: https://www.youtube.com/watch?v=zg0Kx7rP8lE

# New Features in Version 0.6

- SB-002 sensor aquistion and calibration
- Line following Mode

# Op Modes

To change the Op Mode open serial monitor and connect to MIPR type a capital O for Oscar and then select the desired Op Mode.
Op Modes are as follows;

- 0; Remote controlled
- 1; NOT IMPLEMENTED
- 2; NOT IMPLEMENTED
- 3; NOT IMPLEMENTED
- 4; NOT IMPLEMENTED
- 5; Execute command
- 6; Line Follower Mode; Simple
- 7; Line Follower Mode; Intermediate
- 8; Line Follower Mode; Advanced

Modes 1 to 4 do not work because they require SB-001 or SB-001A to work, for these modes use code version 0.5 or 0.4.

# File Breakdown;

- Version_0_6.ino - Main Arduino IDE file, contains setup and loop funcitons for MIPR
- _01_Motor_Functions.ino - Contains functions to access MIPR's motor driver
- _02_Communications.ino - Allows access to MIPR's bluetooth functions
- _03_Odometry_Module.ino - Carries out basic odometry calculaitons
- _06_Line_Follower.ino
- _99_TelPacket.ino - Builds the telemetry packet for broadcast via the Bluetooth module and monitors battery voltage

