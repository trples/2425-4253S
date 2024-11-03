#include "main.h"
#include "subsystemHeaders/global.hpp"

// motor ports
pros::Motor driveLeftBack(1, pros::MotorGearset::blue, pros::MotorEncoderUnits::counts); 
pros::Motor driveLeftBot(12, pros::MotorGearset::blue, pros::MotorEncoderUnits::counts);
pros::Motor driveLeftTop(11, pros::MotorGearset::blue, pros::MotorEncoderUnits::counts);
 
pros::Motor driveRightBack(20, pros::MotorGearset::blue, pros::MotorEncoderUnits::counts);
pros::Motor driveRightBot(19, pros::MotorGearset::blue, pros::MotorEncoderUnits::counts);
pros::Motor driveRightTop(10, pros::MotorGearset::blue, pros::MotorEncoderUnits::counts);

pros::Motor intake(7, pros::MotorGearset::blue, pros::MotorEncoderUnits::counts);

// motor groups for drivetrain
pros::MotorGroup driveLeft({-1,11,-12}); // 11 top
pros::MotorGroup driveRight({-10,19,20}); // 10 top

pros::adi::DigitalOut grabber('A');

pros::Imu inertial(14);

// controller is declared in header class
pros::Controller controller(pros::E_CONTROLLER_MASTER); 




//pros::Motor driveLeftBack(1, pros::E_MOTOR_GEARSET_06, pros::E_MOTOR_ENCODER_ROTATIONS);



