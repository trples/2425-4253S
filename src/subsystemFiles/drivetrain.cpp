#include "main.h"
#include "subsystemHeaders/global.hpp"

bool enableDrivePID = false;
bool enableTurnPID = false;

// helper functions
void resetDriveEncoders(){
    driveLeftBot.tare_position();
    driveLeftTop.tare_position();
    driveLeftBack.tare_position();
    driveRightBot.tare_position();
    driveRightTop.tare_position();
    driveRightBack.tare_position();
}

double getAverageEncoderVal(){
    return (fabs(driveLeftBot.get_position())+fabs(driveLeftTop.get_position())
            +fabs(driveLeftBack.get_position())+fabs(driveRightBot.get_position())
            +fabs(driveRightTop.get_position())+fabs(driveRightBack.get_position()))/6;
}

void updateMotors(double left, double right){
    driveLeft.move(left);
    driveRight.move(right);
}

// driver control
void setDrive(){
    // arcade drive; left = turn, right = forwards/backwards
    int rotate = 0.8*controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int power = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
    
    // rotate and power variables for arcade drive
    int left = power + rotate;
    int right = power - rotate;

    updateMotors(left,0.85*right);
}

// autonomous
int drivePID(int goal){
    // proportional, integral, derivative
    double kP = 0.2345;
    double kI = 0.3;
    double kD = 0.374;


    return 0;
}

int turnPID(int deg){
    return 0;
}

void translate(double distance, double deg){
    resetDriveEncoders();
    
}

void rotate(double deg){

}