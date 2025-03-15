//#include "main.h"

#include "pros/adi.hpp"
#include "pros/imu.hpp"
#include "pros/motor_group.hpp"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include "pros/rotation.hpp"

extern bool isBlue;
extern bool isRed;

extern int intakeTimer;
extern bool intakeSlowed;

// motors declaration
extern pros::Motor driveLeftBack;
extern pros::Motor driveLeftMid;
extern pros::Motor driveLeftFront;
extern pros::Motor driveRightBack;
extern pros::Motor driveRightMid;
extern pros::Motor driveRightFront;
extern pros::Motor intake;
extern pros::Motor intakeUpper;
extern pros::Motor arm;

extern pros::MotorGroup driveLeft;
extern pros::MotorGroup driveRight;

extern pros::adi::DigitalOut grabber;
extern pros::adi::DigitalOut sweeper;

extern pros::Imu inertial;

extern pros::Optical optical;

extern pros::Rotation rotation;
extern pros::Rotation xPod;
extern pros::Rotation yPod;

// controller declaration
extern pros::Controller controller;


