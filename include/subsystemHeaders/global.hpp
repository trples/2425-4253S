//#include "main.h"

#include "lemlib/chassis/chassis.hpp"
#include "pros/adi.hpp"
#include "pros/colors.hpp"
#include "pros/imu.hpp"
#include "pros/motor_group.hpp"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include "pros/rotation.hpp"
#include <cstddef>

extern bool isBlue;

extern bool armGoalPosition;

extern int intakeTimer;
extern bool intakeSlowed;

// motors declaration
extern pros::Motor driveLeftBack;
extern pros::Motor driveLeftBot;
extern pros::Motor driveLeftTop;
extern pros::Motor driveRightBack;
extern pros::Motor driveRightBot;
extern pros::Motor driveRightTop;
extern pros::Motor intakeLower;
extern pros::Motor intakeUpper;
//extern pros::Motor arm;

extern pros::MotorGroup driveLeft;
extern pros::MotorGroup driveRight;
extern pros::MotorGroup intake;
extern pros::Motor arm;

extern pros::adi::DigitalOut grabber;
extern pros::adi::DigitalOut doinker;
extern pros::adi::DigitalOut arm_1;
extern pros::adi::DigitalOut arm_2;


extern pros::Imu inertial;

extern pros::Optical optical;

extern pros::Rotation arm_rotation;
extern pros::Rotation xPod;

extern lemlib::Chassis chassis;

extern pros::Task colorSortTask;

// controller declaration
extern pros::Controller controller;


