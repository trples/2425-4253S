#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "main.h"
#include "pros/abstract_motor.hpp"
#include "pros/motors.hpp"
#include <cstddef>
#include "subsystemHeaders/global.hpp"

bool isBlue = false;
bool isRed = false;

int intakeTimer = 0;
bool intakeSlowed = false;

// motor ports + radio port 21
pros::Motor driveLeftBack(-11, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor driveLeftBot(-3, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor driveLeftTop(17, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);

pros::Motor driveRightBack(9, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor driveRightBot(8, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor driveRightTop(-4, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);

//pros::Motor intakeLower(-11, pros::MotorGearset::green, pros::MotorEncoderUnits::degrees);
//pros::Motor intakeUpper(4,pros::MotorGearset::green,pros::MotorEncoderUnits::degrees);

//pros::Motor arm(-4, pros::MotorGearset::red, pros::MotorEncoderUnits::degrees);


pros::MotorGroup driveLeft({-11,-3,17}, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees); 
pros::MotorGroup driveRight({9,8,-4}, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees); 
pros::MotorGroup intake({20 /*upper intake*/,-2 /*lower intake reversed*/});

pros::adi::DigitalOut grabber('A');
//pros::adi::DigitalOut sweeper('B');
pros::adi::DigitalOut arm('B');

pros::Imu inertial(21);

pros::Optical optical(18);

pros::Rotation rotation(9);
pros::Rotation xPod(16); // horiz tracking

lemlib::TrackingWheel horizontalEncoder(&xPod, lemlib::Omniwheel::NEW_2,-4);

lemlib::OdomSensors sensors(nullptr,nullptr,&horizontalEncoder,nullptr, &inertial);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              10, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in degrees
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in degrees
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

lemlib::Drivetrain drivetrain(&driveLeft, // left motor group
    &driveRight, // right motor group
    10, // track width
    lemlib::Omniwheel::NEW_275, // using new 4" omnis
    480, // drivetrain rpm is 360
    2 // horizontal drift is 2 (for now)
);

lemlib::Chassis chassis(drivetrain, // drivetrain settings
    lateral_controller, // lateral PID settings
    angular_controller, // angular PID settings
    sensors // odometry sensors
);

// controller is declared in header class
pros::Controller controller(pros::E_CONTROLLER_MASTER); 