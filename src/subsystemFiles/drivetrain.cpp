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
    static double kP = 0.2345;
    static double kI = 0.3;
    static double kD = 0.374;

    // for integral and deriv
    static double accumulatedError = 0;
    static double error = 0;
    static double prevError = 0;

    // proportional; based on distance to target position
    prevError = error;
    error = goal - getAverageEncoderVal();

    // integral
    accumulatedError += error*0.015;
    if ((error == 0) || (error > goal) || (error > 50))
		accumulatedError = 0;
	
    // deriv
    double IROC = (error-prevError)/0.015;

    // calculating
    double power = error*kP + accumulatedError*kI + IROC*kD;

    if (power > 127) power = 127;

    //pros::delay(15);

    return power;
}

int turnPID(int deg){ // abs val of deg
    // proportional, integral, derivative
    static double kP = 0.2345;
    static double kI = 0.3;
    static double kD = 0.374;

    // for integral and deriv
    static double accumulatedError = 0;
    static double error = 0;
    static double prevError = 0;
    
    // proportional; based on distance to target position
    prevError = error;
    error = deg - inertial.get_rotation();

    // integral
    accumulatedError += error*0.015;
    if ((error == 0) || (error > deg) || (error > 45))
		accumulatedError = 0;
	
    // deriv
    double IROC = (error-prevError)/0.015;

    // calculating
    double power = error*kP + accumulatedError*kI + IROC*kD;

    if (power > 127) power = 127;

    //pros::delay(15);

    return power;
}

void translate(double distance, int direction){
    resetDriveEncoders();

    //enableDrivePID = true;
    // drive until robot has travelled distance
    while(fabs(getAverageEncoderVal()) < fabs(distance)){
        int power = drivePID(distance);
        updateMotors(direction*power, direction*power);
        pros::delay(15);
    }
    //enableDrivePID = false;
    
    updateMotors(-direction*10, -direction*10);
    pros::delay(50);

    updateMotors(0,0);
}

void rotate(double deg, int direction){ // -1 = left, 1 = right
    
    enableTurnPID = true;
    // if x axis (on side)
    // -> put direction in parameters
    /*inertial.tare_heading();
    while(fabs(inertial.get_heading()) < deg){

    }*/



    // if z axis (upright)
    inertial.tare_rotation();
    while(fabs(inertial.get_rotation()) < deg){
        int power = turnPID(deg);
        updateMotors(direction*power, -direction*power);
        pros::delay(15);
    }
    enableTurnPID = false; 

    updateMotors(direction*-10, direction*10);
    pros::delay(50);

    updateMotors(0,0);
}



/*
int drivePID(int goal){
    // proportional, integral, derivative
    static double kP = 0.2345;
    static double kI = 0.3;
    static double kD = 0.374;

    // for integral and deriv
    static double accumulatedError = 0;
    static double error = 0;
    static double prevError = 0;

    while(enableDrivePID){
        // proportional; based on distance to target position
        prevError = error;
        error = goal - getAverageEncoderVal();

        // integral
        accumulatedError += error*0.015;
        if ((error == 0) || (error > goal))
			accumulatedError = 0;
		
		if (error > 50)
			accumulatedError = 0;

        // deriv
        double IROC = (error-prevError)/0.015;

        // calculating
        double power = error*kP + accumulatedError*kI + IROC*kD;

        if (power > 127) power = 127;

        pros::delay(15);

        return power;
    }
    return 0;
}
*/