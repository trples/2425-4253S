#include "main.h"
#include "subsystemHeaders/global.hpp"

bool enableDrivePID = false;
bool enableTurnPID = false;
bool driveReversed = false;
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
    return (fabs(driveLeftBot.get_position())+fabs(driveLeftTop.get_position())
            +fabs(driveLeftBack.get_position())+fabs(driveRightBot.get_position())
            +fabs(driveRightTop.get_position())+fabs(driveRightBack.get_position()))/6;
            
    /*return (fabs(driveLeftBot.get_position())+fabs(driveLeftTop.get_position())
            +fabs(driveRightBot.get_position())
            +fabs(driveRightTop.get_position())+fabs(driveRightBack.get_position()))/5;*/
}

void updateMotors(double left, double right){
    driveLeft.move(left);
    driveRight.move(right);
}

// driver control
void setDrive(){
    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)){
        driveReversed = !driveReversed;
    }

    // arcade drive; left = turn, right = forwards/backwards
    int rotate = 0.8*controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int power = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
    
    // rotate and power variables for arcade drive
    int left = power + rotate;
    int right = power - rotate;

    if(driveReversed){
        updateMotors(-1*right,-0.85*left);
    }else{
        updateMotors(1*left,0.85*right);
    }
    //updateMotors(left,right);
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
    static double kP = 1;

    static double error = 0;
    static double prevError = 0;

    prevError = error;
    error = goal - getAverageEncoderVal();

    double power = error*kP;

    std::string before = std::to_string(power);
    pros::lcd::set_text(0, before);

    //if (power > 127) power = 127;
    //if (power > 100) power = 100;
    if (power > 70) power = 70;

    std::string after = std::to_string(power);
    pros::lcd::set_text(1, after);

    return power;
}

int turnP(int deg){
    static double kP = 1;
    static double kD = 1;

    static double error = 0;
    static double prevError = 0;

    prevError = error;
    error = deg - inertial.get_rotation();

    //double power = error*kP;
    double power = error*kP + (prevError-error)*kD;

    std::string before = std::to_string(power);
    pros::lcd::set_text(0, before);

    //if (power > 127) power = 127;
    if (power > 60) power = 60;

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
    error = deg - fabs(inertial.get_rotation()); // changed to fabs

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
                                                // distance in INCHES
    resetDriveEncoders();

    // 360 degrees = 3.25*3.14 inches, 8.255*3.14 cm
    //double distanceInUnits = (distance/(3.25*3.14))*360;
    double distanceInUnits = (distance/(3.25*3.14))*360*(30/26);
    pros::lcd::set_text(5,std::to_string(distanceInUnits));

    // drive until robot has travelled distance
    /*while(fabs(getAverageEncoderVal()) < fabs(distance)){*/
    while(fabs(getAverageEncoderVal()) < fabs(distanceInUnits)){
        int power = driveP(distanceInUnits);
        updateMotors(0.9*direction*power, 1*direction*power);
        pros::lcd::set_text(3,std::to_string(getAverageEncoderVal()));
        pros::delay(15);

    }
    
    updateMotors(-direction*50, -direction*50);
    //updateMotors(-direction*100, -direction*100);
    pros::delay(75);

    updateMotors(0,0);
}

void rotate(double deg, int direction){ // -1 = left, 1 = right
    int timeElapsed = 0;
    // if z axis (upright)
    inertial.tare_rotation();

    /*updateMotors(direction*70, direction*-70);
    pros::delay(300);*/
    
    updateMotors(direction*120, direction*-120); // **** this might need adjustments
    pros::delay(150);

    while(fabs(inertial.get_rotation()) < fabs(deg)){
        //int power = turnPID(deg);
        int power = turnP(deg);
        updateMotors(direction*power, -direction*power);
        pros::lcd::set_text(2,std::to_string(inertial.get_rotation()));
        timeElapsed+=15;
        pros::delay(15);
        if(timeElapsed > 1500){
            break;
        }
    }

    updateMotors(direction*-50, direction*50);
    pros::delay(50);

    updateMotors(0,0);
}

void slowTranslate(double distance, int direction){ // -1 = backward, 1 = forward
                                                // distance in INCHES
    resetDriveEncoders();

    // 360 degrees = 3.25*3.14 inches, 8.255*3.14 cm
    //double distanceInUnits = (distance/(3.25*3.14))*360;
    double distanceInUnits = (distance/(3.25*3.14))*360*(4/3)*(5/4);
    pros::lcd::set_text(5,std::to_string(distanceInUnits));

    // drive until robot has travelled distance
    /*while(fabs(getAverageEncoderVal()) < fabs(distance)){*/
    while(fabs(getAverageEncoderVal()) < fabs(distanceInUnits)){
        updateMotors(0.9*direction*50, 1*direction*50);
        pros::delay(15);

    }
    
    updateMotors(-direction*30, -direction*30);
        //updateMotors(-direction*100, -direction*100);
    pros::delay(50);

    updateMotors(0,0);
}

/*void resetPosition(){
    double encoderVal = getAverageEncoderVal();
    double error = 0-encoderVal;
    int direction;
    if(encoderVal>0){ // encoder val is negative -> too far forward, go backwards
        direction = -1;
    }else{
        direction = 1;
    } 
    pros::lcd::set_text(3,std::to_string(direction));

    /*while(fabs(error)>720){
        updateMotors(0.9*direction*50, 1*direction*50);
        pros::delay(10);
        encoderVal = getAverageEncoderVal();
        error = 0-encoderVal;
        pros::lcd::set_text(4,std::to_string(encoderVal));
        pros::lcd::set_text(5,std::to_string(error));
        pros::lcd::set_text(6,std::to_string(fabs(error)>45));
    }

    // degrees/360 * circumfrence of wheel * wheels to gear ratio
    double distanceInInches = (encoderVal/360)*3.25*3.14*(3/4);
    pros::lcd::set_text(0,std::to_string(direction));

    translate(fabs(distanceInInches),direction);

    //updateMotors(-direction*50, -direction*50);
    //updateMotors(-direction*100, -direction*100);
    //pros::delay(75);

    //updateMotors(0,0);
}*/

void resetRotation(){
    double currentAngle = inertial.get_rotation();
    double error = 0-currentAngle;
    int direction;
    bool positionAdjusted = false;
    if(error<0){ // encoder val is positive -> rotated right, go left
        direction = -1;
    }else{
        direction = 1;
    } 

    while(fabs(error)>3){
        updateMotors(0.9*direction*50, -1*direction*50);
        pros::delay(10);
        currentAngle = inertial.get_rotation();
        error = 0-currentAngle;
        positionAdjusted = true;
        pros::lcd::set_text(4,std::to_string(inertial.get_rotation()));
        pros::lcd::set_text(5,std::to_string(error));
        pros::lcd::set_text(6,std::to_string(fabs(currentAngle<45)));
    }
    if(positionAdjusted){
        updateMotors(-direction*50, direction*50);
        //updateMotors(-direction*100, -direction*100);
        pros::delay(75);

        updateMotors(0,0);
    }
}




void test(){
    //resetRotation();
    //resetPosition();
    /*slowTranslate(20,1);
    pros::delay(200);
    toggleIntake(true,1);
    pros::delay(1000);
    resetRotation();
    slowTranslate(20,-1);
    pros::delay(400);
    toggleIntake(false,1);*/
    
    /*toggleIntake(true,1);
    slowTranslate(36,1); // grab two rings
    pros::delay(1000);
    toggleIntake(true,-1);
    pros::delay(500);
    toggleIntake(true,1);
    pros::delay(1500);
    //translate(18,-1);
    translate(24,-1);
    toggleIntake(true,-1);
    pros::delay(500);
    toggleIntake(false,0);
    //rotate(30,1); // to third ring (GOES WITH TRANSLATE 18,1)
    rotate(45,1);
    pros::delay(1000);
    toggleIntake(true,1);
    translate(18,1);
    pros::delay(1000);
    translate(18,-1); // to og position
    toggleIntake(false,0);
    pros::delay(500);


    //rotate(300,1); // turn to face wall (perpendicular)
    //rotate(45,-1); // turn to face wall
    resetRotation(); // reset to wall?
    pros::delay(1000);
    translate(18,1);
    pros::delay(500);
    //rotate(180 + 45,1); // face goal to corner
    rotate(120,-1); // face goal to corner
    pros::delay(1000);
    inertial.tare_rotation();
    translate(24,-1); // go back into goal
    pros::delay(500);
    grabber.set_value(LOW); // drop goal
    pros::delay(200);
    translate(24,1);*/
    translate(24,1); //1 tile
    pros::delay(750);
    resetRotation();
    pros::delay(250);
    translate(24,1); //2 tiles
    pros::delay(750);
    resetRotation();
    pros::delay(250);
    translate(30,1); //2 tiles
    pros::delay(250);
    resetRotation();
    pros::delay(250);

}

void skills(){
    toggleIntake(true,1);
    pros::delay(700);
    toggleIntake(false,0); // alliance stake

    translate(20,1);
    rotate(270,1);
    //rotate(90,-1);
    pros::delay(700);
    inertial.tare_rotation();
    translate(20,-1);
    pros::delay(500);
    resetRotation();
    pros::delay(500);
    slowTranslate(12,-1);
    grabber.set_value(HIGH);
    pros::delay(500);
    resetRotation(); // fix rotation after running into goal
    pros::delay(500);
    rotate(180,1);
    pros::delay(500);
    inertial.tare_rotation();
    
    toggleIntake(true,1);
    slowTranslate(36,1); // grab two rings
    pros::delay(1000);
    toggleIntake(true,-1);
    pros::delay(500);
    toggleIntake(true,1);
    slowTranslate(6,1);
    pros::delay(1500);
    //translate(18,-1);
    translate(24+6,-1); // go back
    toggleIntake(true,-1);
    pros::delay(500);
    toggleIntake(false,0);
    resetRotation();
    pros::delay(300);
    //rotate(30,1); // to third ring (GOES WITH TRANSLATE 18,1)
    rotate(45,1);
    pros::delay(1000);
    toggleIntake(true,1);
    translate(15,1);
    pros::delay(1000);
    translate(15,-1); // to og position
    toggleIntake(false,0);
    pros::delay(500);


    //rotate(300,1); // turn to face wall (perpendicular)
    //rotate(45,-1); // turn to face wall
    resetRotation(); // reset to wall?
    pros::delay(1000);
    translate(14,1);
    pros::delay(500);
    //rotate(180 + 45,1); // face goal to corner
    rotate(105,-1); // face goal to corner
    pros::delay(1000);
    inertial.tare_rotation();
    translate(28,-1); // go back into goal
    pros::delay(500);
    grabber.set_value(LOW); // drop goal
    pros::delay(200);
    translate(28,1);

    // trans into 2ND SECTION
    rotate(135,1); // face wall
    pros::delay(500);
    translate(18,1); // overshoot 24" so bot's against the wall
    pros::delay(1000);
    //resetRotation();
    //rotate(180,1); // face other quadrant
    //pros::delay(500);
    //translate(10,-1); // back into wall
    rotate(180,1);


    // TRAVERSE FIELD
    //translate(6+24*2,1); // traverse 3 tiles (6 in + 2 tiles)
    //inertial.tare_rotation();
    translate(24,1); //1 tile
    pros::delay(750);
    resetRotation();
    pros::delay(250);
    translate(24,1); //2 tiles
    pros::delay(750);
    resetRotation();
    pros::delay(250);
    translate(30,1); //3 tiles
    pros::delay(250);
    resetRotation();
    pros::delay(250);

    //pros::delay(1200);
    rotate(180,1); 
    pros::delay(500);
    translate(20,-1);

    resetRotation(); // fix rotation before goal
    pros::delay(500);
    slowTranslate(12,-1); // back into goal
    grabber.set_value(HIGH);
    pros::delay(500);
    resetRotation(); // fix rotation after running into goal
    pros::delay(500);
    rotate(180,1); // face 2 rings
    pros::delay(500);

    toggleIntake(true,1);
    slowTranslate(36,1); // grab two rings
    pros::delay(1000);
    toggleIntake(true,-1);
    pros::delay(500);
    toggleIntake(true,1);
    pros::delay(1500);
    translate(18,-1); // 1.5 tiles back (perfect 45 deg from 3rd ring)  //maybe change this
    toggleIntake(true,-1);
    pros::delay(500);
    toggleIntake(false,0);
    //
    rotate(45,-1); // to third ring
    pros::delay(1000);
    toggleIntake(true,1);
    translate(12,1); // pick ring up
    pros::delay(1000);
    translate(12,-1); // to og position
    toggleIntake(false,0);
    pros::delay(500);

    resetRotation(); // reset to wall?
    pros::delay(1000);
    translate(6,1);
    pros::delay(500);
    //rotate(180 + 45,1); // face goal to corner
    rotate(135,1); // face goal to corner
    pros::delay(1000);
    inertial.tare_rotation();
    translate(4,-1); // go back into goal
    pros::delay(500);
    grabber.set_value(LOW); // drop goal
    pros::delay(200);
    translate(4,1);

    // max pts: 3 (alliance top) + 6 (2 top stake) + 4 (4 normal rings) + 10 (2 corner stakes)
    //          = 23 pts
}

void redLeft(){
    translate(22,-1); // back (into goal)
    pros::delay(500);
    //updateMotors(-70,-70);
    resetRotation();
    pros::delay(200);
    slowTranslate(24,-1);
    pros::delay(400);
    //switchGrabber(); // grab goal
    grabber.set_value(HIGH);
    pros::delay(600);
    resetRotation();
    pros::delay(300);
    translate(20,1);
    pros::delay(500);
    translate(4,1);
    pros::delay(300);
    translate(4,-1);
    pros::delay(400);
    toggleIntake(true,1);
    resetRotation();
    pros::delay(700);
    toggleIntake(true,-1);
    rotate(90,1); // face wall
    inertial.tare_rotation();

    toggleIntake(false,0);
    pros::delay(500);
    translate(12,1); // twds rings
    toggleIntake(true,1);
    pros::delay(400);
    resetRotation();
    pros::delay(200);
    slowTranslate(20,1); // into ring
    
    pros::delay(1000);
    translate(20,-1);
    pros::delay(500);
    rotate(180,1);
    pros::delay(400);

    translate(36,1);
    toggleIntake(true,-1);
    pros::delay(500);
    updateMotors(50,50);
    toggleIntake(false,0);//*/
}

void redRight(){
    // red right side
    //translate(36,-1); // back (into goal)
    translate(22,-1);
    pros::delay(500);
    resetRotation();
    pros::delay(200);
    //updateMotors(-50,-50);
    slowTranslate(30,-1);
    pros::delay(500);
    //switchGrabber(); // grab goal
    grabber.set_value(HIGH);
    pros::delay(500);
    resetRotation();
    pros::delay(300);
    translate(12,1);
    pros::delay(500);
    //translate(14,1);
    //pros::delay(100);
    translate(4,1);
    pros::delay(300);
    translate(4,-1);
    pros::delay(400);
    toggleIntake(true,1);
    resetRotation();
    pros::delay(500);
    toggleIntake(true,-1);
    rotate(270,1); // face wall
    inertial.tare_rotation();

    pros::delay(100);
    toggleIntake(false,0);
    pros::delay(400);
    translate(12,1);
    //updateMotors(50,50);
    toggleIntake(true,1);
    pros::delay(400);
    resetRotation();
    //pros::delay(200);
    slowTranslate(16,1);

    //pros::delay(370);
    //updateMotors(0,0);
    pros::delay(700);
    toggleIntake(false,0);
    translate(16,-1);
    pros::delay(500);
    rotate(180,1);
    pros::delay(400);

    translate(40,1);
    
    /*toggleIntake(true,-1);
    
    pros::delay(500);
    rotate(10,1);
    pros::delay(200);
    updateMotors(50,50);
    toggleIntake(false,0); //*/
    //pros::delay(1500);
    //updateMotors(0,0);//
}

void blueLeft(){
    // blue left
    //translate(36,-1); // back (into goal)
    translate(22,-1);
    pros::delay(500);
    resetRotation();
    pros::delay(200);
    //updateMotors(-50,-50);
    slowTranslate(30,-1);
    pros::delay(500);
    //switchGrabber(); // grab goal
    grabber.set_value(HIGH);
    pros::delay(500);
    resetRotation();
    pros::delay(300);
    translate(16,1);
    pros::delay(500);
    translate(4,1);
    pros::delay(300);
    translate(4,-1);
    pros::delay(400);
    toggleIntake(true,1);
    resetRotation();
    pros::delay(700);
    toggleIntake(true,-1);
    rotate(90,1);
    inertial.tare_rotation();

    pros::delay(100);
    toggleIntake(false,0);
    pros::delay(400);
    translate(12,1);
    //updateMotors(50,50);
    toggleIntake(true,1);
    slowTranslate(16,1);

    //pros::delay(370);
    //updateMotors(0,0);
    pros::delay(700);
    translate(16,-1);
    pros::delay(400);
    rotate(180,1);
    pros::delay(400);

    translate(50,1);
    
    /*toggleIntake(true,-1);
    
    pros::delay(100);
    updateMotors(30,30);
    pros::delay(200);
    toggleIntake(false,0); 
    pros::delay(1000);
    updateMotors(0,0);
    //*/
}

void blueRight(){
    translate(36,-1); // back (into goal)
    pros::delay(500);
    updateMotors(-70,-70);
    pros::delay(300);
    //switchGrabber(); // grab goal
    grabber.set_value(HIGH);
    pros::delay(400);
    updateMotors(0,0);
    pros::delay(50);
    translate(14,1);
    pros::delay(100);
    translate(4,1);
    pros::delay(300);
    translate(4,-1);
    pros::delay(400);
    toggleIntake(true,1);
    pros::delay(700);
    toggleIntake(true,-1);
    
    rotate(100,1);
    toggleIntake(false,0);

    translate(36,1);
    updateMotors(30,30);
    pros::delay(500); 
    
    pros::delay(1000);
    updateMotors(0,0);//*/
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