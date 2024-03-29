#include "main.h"

//Externs are found in main.h

using namespace pros; //Repeating pros:: is no longer required in declarations

//Motor Ports
#define DRIVE_FRONT_LEFT_PORT      1
#define DRIVE_MIDDLE_LEFT_PORT     2
#define DRIVE_BACK_LEFT_PORT       3
#define DRIVE_FRONT_RIGHT_PORT     4
#define DRIVE_MIDDLE_RIGHT_PORT    5
#define DRIVE_BACK_RIGHT_PORT      6
#define LIFT_PORT                  9
#define INTAKE_PORT                8


//Sensor Ports
#define TRACKING_FIRST_PORT        5  //E
#define TRACKING_SECOND_PORT       6  //F
#define LEFT_ENCODER_PORT          10
#define RIGHT_ENCODER_PORT         12
#define IMU_SENSOR_PORT            11
#define GPS_SENSOR_PORT            13


//Pneumatic Ports
#define FRONT_CLIP_PORT            1  //A
#define BACK_CLIP_PORT             2  //B
#define CLIP_GUARD_PORT            3  //C



//Motor name(port, gearset, bool reversed, encoder units)
Motor driveFL(DRIVE_FRONT_LEFT_PORT, MOTOR_GEARSET_18,
              true, E_MOTOR_ENCODER_DEGREES);
Motor driveML(DRIVE_MIDDLE_LEFT_PORT, MOTOR_GEARSET_18,
              false, E_MOTOR_ENCODER_DEGREES);
Motor driveBL(DRIVE_BACK_LEFT_PORT, MOTOR_GEARSET_18,
              true, E_MOTOR_ENCODER_DEGREES);
Motor driveFR(DRIVE_FRONT_RIGHT_PORT, MOTOR_GEARSET_18,
              false, E_MOTOR_ENCODER_DEGREES);
Motor driveMR(DRIVE_MIDDLE_RIGHT_PORT, MOTOR_GEARSET_18,
              true, E_MOTOR_ENCODER_DEGREES);
Motor driveBR(DRIVE_BACK_RIGHT_PORT, MOTOR_GEARSET_18,
              false, E_MOTOR_ENCODER_DEGREES);
Motor lift(LIFT_PORT, MOTOR_GEARSET_36,
              false, E_MOTOR_ENCODER_DEGREES);
Motor intake(INTAKE_PORT, E_MOTOR_GEARSET_06,
              false, E_MOTOR_ENCODER_DEGREES);

//Sensor declarations
ADIEncoder tracking(TRACKING_FIRST_PORT, TRACKING_SECOND_PORT, false);
Rotation leftEncoder(LEFT_ENCODER_PORT);
Rotation rightEncoder(RIGHT_ENCODER_PORT);
Imu gyro(IMU_SENSOR_PORT);
GPS gps(GPS_SENSOR_PORT, 0, 0, 0);

//Pneumatics declarations
ADIDigitalOut frontClip(FRONT_CLIP_PORT);
ADIDigitalOut backClip(BACK_CLIP_PORT);
ADIDigitalOut clipGuard(CLIP_GUARD_PORT);

//Controller declaration
Controller master(E_CONTROLLER_MASTER);

//Misc functions
void ResetSensors(bool calibrateGyro) {
  tracking.reset();
  leftEncoder.reset();
  rightEncoder.reset();
  if(calibrateGyro) {
    gyro.reset();
    pros::delay(2000);
  }
  else {
    gyro.tare_rotation();
  }

  printf("Sensors reset\n");
}

float rot_offset = 0.0;