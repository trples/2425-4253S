#include "main.h"
#include "pros/abstract_motor.hpp"
#include "pros/motors.hpp"
#include "subsystemHeaders/global.hpp"

bool isBlue = false;
bool isRed = false;

int intakeTimer = 0;
bool intakeSlowed = false;

// motor ports + radio port 11
pros::Motor driveLeftBack(-15, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor driveLeftBot(-14, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor driveLeftTop(16, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);

pros::Motor driveRightBack(2, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor driveRightBot(-20, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor driveRightTop(3, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);

//pros::Motor intakeLower(-11, pros::MotorGearset::green, pros::MotorEncoderUnits::degrees);
//pros::Motor intakeUpper(4,pros::MotorGearset::green,pros::MotorEncoderUnits::degrees);

//pros::Motor arm(-4, pros::MotorGearset::red, pros::MotorEncoderUnits::degrees);


pros::MotorGroup driveLeft({-14,-15,16}); 
pros::MotorGroup driveRight({3,-20,2}); 
pros::MotorGroup intake({4,-11});

pros::adi::DigitalOut grabber('A');
//pros::adi::DigitalOut sweeper('B');
pros::adi::DigitalOut arm('B');

pros::Imu inertial(21);

pros::Optical optical(18);

pros::Rotation rotation(9);
pros::Rotation xPod(16); // left/right
pros::Rotation yPod(8); // front/back

// controller is declared in header class
pros::Controller controller(pros::E_CONTROLLER_MASTER); 




