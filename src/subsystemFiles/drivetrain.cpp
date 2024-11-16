#include "main.h"
#include "subsystemHeaders/global.hpp"

bool enableDrivePID = false;
bool enableTurnPID = false;
//static double accumulatedError = 0;

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
    /*return (fabs(driveLeftBot.get_position())+fabs(driveLeftTop.get_position())
            +fabs(driveLeftBack.get_position())+fabs(driveRightBot.get_position())
            +fabs(driveRightTop.get_position())+fabs(driveRightBack.get_position()))/6;*/
            
    return (fabs(driveLeftBot.get_position())+fabs(driveLeftTop.get_position())
            +fabs(driveRightBot.get_position())
            +fabs(driveRightTop.get_position())+fabs(driveRightBack.get_position()))/5;
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

    //updateMotors(left,0.85*right);
    updateMotors(left,right);
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

    std::string before = std::to_string(power);
    pros::lcd::set_text(0, before);

    if (power > 127) power = 127;

    std::string after = std::to_string(power);
    pros::lcd::set_text(1, after);

    //pros::delay(15);

    return power;
}

int driveP(int goal){
    static double kP = 0.2;

    static double error = 0;
    static double prevError = 0;

    prevError = error;
    error = goal - getAverageEncoderVal();

    double power = error*kP;

    std::string before = std::to_string(power);
    pros::lcd::set_text(0, before);

    if (power > 127) power = 127;

    std::string after = std::to_string(power);
    pros::lcd::set_text(1, after);

    return power;
}

int turnP(int deg){
    static double kP = 1.3;

    static double error = 0;
    static double prevError = 0;

    prevError = error;
    error = deg - inertial.get_rotation();

    double power = error*kP;

    std::string before = std::to_string(power);
    pros::lcd::set_text(0, before);

    if (power > 127) power = 127;

    std::string after = std::to_string(power);
    pros::lcd::set_text(1, after);

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

    std::string before = std::to_string(power);
    pros::lcd::set_text(0, before);

    if (power > 127) power = 127;

    //pros::delay(15);
    std::string after = std::to_string(power);
    pros::lcd::set_text(1, after);

    return power;
}

void translate(double distance, int direction){ // -1 = backward, 1 = forward
    resetDriveEncoders();

    pros::lcd::set_text(0,std::to_string(getAverageEncoderVal()));
    //enableDrivePID = true;
    // drive until robot has travelled distance
    while(fabs(getAverageEncoderVal()) < fabs(distance)){
        //int power = drivePID(distance);
        int power = driveP(distance);
        updateMotors(direction*power, 0.85*direction*power);
        //pros::lcd::set_text(2,std::to_string(getAverageEncoderVal()));
        pros::lcd::set_text(1,"DL"+std::to_string(driveLeft.get_position()));
        pros::lcd::set_text(2,"DLBack"+std::to_string(driveLeftBack.get_position()));
        pros::lcd::set_text(3,"DLBot"+std::to_string(driveLeftBot.get_position()));
        pros::lcd::set_text(4,"DR"+std::to_string(driveRight.get_position()));
        pros::lcd::set_text(5,"DRBack"+std::to_string(driveRightBack.get_position()));
        pros::lcd::set_text(6,"DRBot"+std::to_string(driveRightBot.get_position()));
        pros::delay(15);

    }
    //enableDrivePID = false;
    
    updateMotors(-direction*10, -direction*10);
    pros::delay(50);

    updateMotors(0,0);
}

void rotate(double deg, int direction){ // -1 = left, 1 = right
    
    // if z axis (upright)
    inertial.tare_rotation();
    while(fabs(inertial.get_rotation()) < fabs(deg)){
        //int power = turnPID(deg);
        int power = turnP(deg);
        updateMotors(direction*power, -direction*power);
        pros::lcd::set_text(2,std::to_string(inertial.get_rotation()));
        pros::delay(15);
    }
    //enableTurnPID = false; 

    /*int timeToDelay = deg/90;

    updateMotors(direction*127,-direction*127);

    pros::delay(200);*/

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