#include "main.h"
#include "subsystemHeaders/global.hpp"

void setDrive(){
    // arcade drive; left = turn, right = forwards/backwards
    int rotate = 0.8*controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int power = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
    
    // rotate and power variables for arcade drive
    int left = power + rotate;
    int right = power - rotate;

    updateMotors(left,0.85*right);
}

void updateMotors(double left, double right){
    driveLeft.move(left);
    driveRight.move(right);
}

void translate(double distance, double deg){
    // for auton
}