#include "main.h"
#include "subsystemHeaders/global.hpp"

bool isBlue = false;
bool isRed = false;

// motor ports + radio port 11
pros::Motor driveLeftBack(-3, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor driveLeftMid(-12, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor driveLeftFront(-14, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);

pros::Motor driveRightBack(10, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor driveRightMid(2, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor driveRightFront(15, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor intake(-1, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);

pros::Motor arm(-4, pros::MotorGearset::red, pros::MotorEncoderUnits::degrees);

pros::MotorGroup driveLeft({-14,-12,-3}); 
pros::MotorGroup driveRight({15,2,10}); 

pros::adi::DigitalOut grabber('H');
pros::adi::DigitalOut sweeper('B');

pros::Imu inertial(21);

pros::Optical optical(18);

pros::Rotation rotation(9);
pros::Rotation xPod(16); // left/right
pros::Rotation yPod(8); // front/back

// controller is declared in header class
pros::Controller controller(pros::E_CONTROLLER_MASTER); 

/*pros::Motor driveLeftBack(10, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees); 
pros::Motor driveLeftBot(19, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor driveLeftTop(20, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
 
pros::Motor driveRightBack(1, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor driveRightBot(11, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);
pros::Motor driveRightTop(2, pros::MotorGearset::blue, pros::MotorEncoderUnits::degrees);

pros::Motor intake(18, pros::MotorGearset::blue, pros::MotorEncoderUnits::counts);*/

// motor groups for drivetrain






//pros::Motor driveLeftBack(1, pros::E_MOTOR_GEARSET_06, pros::E_MOTOR_ENCODER_ROTATIONS);



