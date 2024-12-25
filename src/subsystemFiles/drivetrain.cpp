#include "main.h"
#include "subsystemHeaders/global.hpp"

bool enableDrivePID = false;
bool enableTurnPID = false;
bool driveReversed = false;
//static double accumulatedError = 0;

// helper functions
void resetDriveEncoders(){
    driveLeftFront.tare_position();
    driveLeftMid.tare_position();
    driveLeftBack.tare_position();
    driveRightFront.tare_position();
    driveRightMid.tare_position();
    driveRightBack.tare_position();
}

double getAverageEncoderVal(){
    return (fabs(driveLeftFront.get_position())+fabs(driveLeftMid.get_position())
            +fabs(driveLeftBack.get_position())+fabs(driveRightFront.get_position())
            +fabs(driveRightMid.get_position())/*+fabs(driveRightBack.get_position())*/)/5;//6;
            
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
        updateMotors(-1*right,-1*left);
    }else{
        updateMotors(1*left,1*right);
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

double previousPower = 0;
double error = 0;
double prevError = 0;

int limit = 10;

void resetDriveVaris(){
    previousPower = 0;
    error = 0;
    prevError = 0;

}

int driveP(int goal){
    static double kP = 0.2;
    static double kD = 2;

    static double error = 0;
    static double prevError = 0;

    prevError = error;
    error = goal - getAverageEncoderVal();

    double power = error*kP;

    //std::string before = std::to_string(power);
    //pros::lcd::set_text(0, before);

    //if (power > 127) power = 127;
    //if (power > 100) power = 100;
    if (power > 70) power = 70;
    if ((previousPower - power) > limit) power = previousPower - limit;// decreasing too much
    else if ((power - previousPower) > limit) power = previousPower + limit; // increasing too much

    //std::string after = std::to_string(power);
    //pros::lcd::set_text(1, after);

    previousPower = power;
    return power;
}

int controlledSpeed(int goal){
    static double kP = 1;

    prevError = error;
    error = goal - getAverageEncoderVal();

    double power = error*kP;

    std::string before = std::to_string(power);
    pros::lcd::set_text(0, before);

    if (fabs(power - previousPower) > limit){
        if(power-previousPower>0){ // power is increasing
            power = previousPower + limit;
        }else{
            power = previousPower - limit;
        }
    }
    if (power > 70) power = 70;

    std::string after = std::to_string(power);
    pros::lcd::set_text(1, after);
    
    previousPower = power;

    return power;
}

int controlledSlow(int goal){
    static double kP = 1;

    prevError = error;
    error = goal - getAverageEncoderVal();

    double power = previousPower*((goal-(2*getAverageEncoderVal()))/goal);


    return power;
}

double startingDeg;
double goalDeg;

double normalizeAngle(double angle) {
    while (angle > 180) angle -= 360;
    while (angle < -180) angle += 360;
    return angle;
}

int turnP(int deg){
    goalDeg = fabs(normalizeAngle(deg + startingDeg));

    //goalDeg = deg + startingDeg;

    static double kP = 1.2;
    static double kD = 2;

    static double error = 0;
    static double prevError = 0;

    prevError = error;
    //error = goalDeg - fabs(inertial.get_rotation());
    error = goalDeg - fabs(normalizeAngle(inertial.get_rotation()));

    double power = error*kP;
    //double power = error*kP + (prevError-error)*kD;

    std::string before = std::to_string(power);
    pros::lcd::set_text(0, before);

    //if (power > 127) power = 127;
    if (power > 60) power = 60;

    std::string after = std::to_string(power);
    pros::lcd::set_text(1, after);

    return power;

}

/*int turnPID(int deg){ // abs val of deg
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
}*/

void translate(double distance, int direction){ // -1 = backward, 1 = forward
                                                // distance in INCHES
    resetDriveEncoders();
    resetDriveVaris();
    bool PIDed = false;
    int timeElapsed = 0;
    // 360 degrees = 3.25*3.14 inches, 8.255*3.14 cm
    //double distanceInUnits = (distance/(3.25*3.14))*360;
    double distanceInUnits = (distance/(3.25*3.14))*360*(48.0/36.0); // need to fix gear ratio
    //pros::lcd::set_text(5,std::to_string(distanceInUnits));
    //pros::lcd::set_text(6,std::to_string(getAverageEncoderVal()));

    // drive until robot has travelled distance
    /*while(fabs(getAverageEncoderVal()) < fabs(distance)){*/
    while(fabs(getAverageEncoderVal()) < fabs(distanceInUnits)){
        int power = driveP(distanceInUnits);
        updateMotors(1*direction*power, 1*direction*power);
        pros::lcd::set_text(4,std::to_string(getAverageEncoderVal()));
        pros::delay(15);
        PIDed = true;
        timeElapsed += 15;
        if(timeElapsed > 2000){
            PIDed = false;
            break;
        }
    }
    
    if(PIDed){
        updateMotors(-direction*50, -direction*50);
        //updateMotors(-direction*100, -direction*100);
        pros::delay(75);
    }

    updateMotors(0,0);

    pros::delay(100);
}

void controlledTranslate(double distance, int direction){ // -1 = backward, 1 = forward
                                                          // distance in INCHES
    resetDriveEncoders();
    resetDriveVaris();
    bool PIDed = false;
    int timeElapsed = 0;
    int power;
    // 360 degrees = 3.25*3.14 inches, 8.255*3.14 cm
    double distanceInUnits = (distance/(3.25*3.14))*360*(30/26); // need to fix gear ratio
    pros::lcd::set_text(5,std::to_string(distanceInUnits));

    // drive until robot has travelled halfway distance
    while(fabs(getAverageEncoderVal()) < fabs(0.5*distanceInUnits)){
        power = controlledSpeed(distanceInUnits);
        updateMotors(1*direction*power, 1*direction*power);
        pros::lcd::set_text(3,std::to_string(getAverageEncoderVal()));
        pros::delay(15);
        PIDed = true;
        timeElapsed += 15;
        if(timeElapsed > 2000){
            PIDed = false;
            break;
        }
    }
    
    while(fabs(getAverageEncoderVal()) < fabs(distanceInUnits)){
        int power = controlledSlow(distanceInUnits);
        updateMotors(1*direction*power, 1*direction*power);
        pros::lcd::set_text(3,std::to_string(getAverageEncoderVal()));
        pros::delay(15);
        PIDed = true;
        timeElapsed += 15;
        if(timeElapsed > 2000){
            PIDed = false;
            break;
        }
    }

    if(PIDed){
        updateMotors(-direction*50, -direction*50);
        //updateMotors(-direction*100, -direction*100);
        pros::delay(75);
    }

    updateMotors(0,0);

    pros::delay(100);
}

void oldRotate(double deg, int direction){ // -1 = left, 1 = right
    int timeElapsed = 0;
    // if z axis (upright)
    inertial.tare_rotation();

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

int fixHeading(double deg){
    /*if(deg > 360){
        deg = deg-360;
        return deg;
    }
    if(deg < 0){
        deg = deg + 360;
        return deg;
    }*/
    return fmod((deg + 360), 360);
}

int HturnP(int deg){
    //goalDeg =  

    //goalDeg = deg + startingDeg;

    static double kP = 1.2;
    static double kD = 2;

    static double error = 0;
    static double prevError = 0;

    prevError = error;
    //error = goalDeg - fabs(inertial.get_rotation());
    error = fabs(deg - (inertial.get_heading()));

    double power = error*kP;
    //double power = error*kP + (prevError-error)*kD;

    std::string before = std::to_string(power);
    pros::lcd::set_text(0, before);

    //if (power > 127) power = 127;
    if (power > 60) power = 60;

    std::string after = std::to_string(power);
    pros::lcd::set_text(1, after);

    return power;
}

void rotate(double deg, int direction){ // -1 = left, 1 = right
    int timeElapsed = 0;
    bool PIDed = false;
    startingDeg = inertial.get_heading();
    
    
    double targetHeading = fixHeading(deg*direction + startingDeg);
    double error = fabs((inertial.get_heading()) - targetHeading);

    while(error > 1){
        PIDed = true;
        int power = HturnP(targetHeading);
        updateMotors(direction*power, -direction*power);
        timeElapsed+=15;
        pros::delay(15);
        error = fabs((inertial.get_heading()) - targetHeading);
        if(timeElapsed > 1500){
            PIDed = false;
            break;
        }
    }
    if(PIDed){
        updateMotors(direction*-50, direction*50);
        pros::delay(100);
    }

    updateMotors(0,0);
    
    pros::delay(100);
}

void rotateSmall(double deg, int direction){
    int timeElapsed = 0;
    bool PIDed = false;
    startingDeg = inertial.get_heading();
    
    
    double targetHeading = fixHeading(deg*direction + startingDeg);
    double error = fabs((inertial.get_heading()) - targetHeading);

    while(error > 1){
        updateMotors(direction*50, -direction*50);
        timeElapsed+=15;
        pros::delay(15);
        error = fabs((inertial.get_heading()) - targetHeading);
        if(timeElapsed > 1500){
            PIDed = false;
            break;
        }
    }
    if(PIDed){
        updateMotors(direction*-50, direction*50);
        pros::delay(100);
    }

    updateMotors(0,0);
    
    pros::delay(100);
}

void threadTranslate(double distance, int direction){
    resetDriveEncoders();
    resetDriveVaris();
    bool PIDed = false;
    int timeElapsed = 0;
    // 360 degrees = 3.25*3.14 inches, 8.255*3.14 cm
    //double distanceInUnits = (distance/(3.25*3.14))*360;
    double distanceInUnits = (distance/(3.25*3.14))*360*(48.0/36.0); // need to fix gear ratio
    //pros::lcd::set_text(5,std::to_string(distanceInUnits));
    //pros::lcd::set_text(6,std::to_string(getAverageEncoderVal()));

    // drive until robot has travelled distance
    /*while(fabs(getAverageEncoderVal()) < fabs(distance)){*/
    while(fabs(getAverageEncoderVal()) < fabs(distanceInUnits)){
        int power = driveP(distanceInUnits);
        updateMotors(1*direction*power, 1*direction*power);
        pros::lcd::set_text(4,std::to_string(getAverageEncoderVal()));
        pros::delay(15);
        PIDed = true;
        timeElapsed += 15;
        if(timeElapsed > 2000){
            PIDed = false;
            break;
        }
    }
}

void threadRotate(double deg, int direction){ // -1 = left, 1 = right
    int timeElapsed = 0;
    bool PIDed = false;
    startingDeg = inertial.get_rotation();
    //inertial.tare_rotation();

    /*updateMotors(direction*120, direction*-120); // **** this might need adjustments
    pros::delay(150);*/

    while(fabs(inertial.get_rotation()) < fabs(normalizeAngle(deg + startingDeg))){
        PIDed = true;
        int power = turnP(deg);
        updateMotors(direction*power, -direction*power);
        //pros::lcd::set_text(2,std::to_string(inertial.get_rotation()));
        timeElapsed+=15;
        pros::delay(15);
        if(timeElapsed > 1500 /*|| deg <=45*/){
            PIDed = false;
            break;
        }
    }
}

void slowTranslate(double distance, int direction){ // -1 = backward, 1 = forward
                                                // distance in INCHES
    resetDriveEncoders();

    bool PIDed = false;
    int timeElapsed = 0;

    // 360 degrees = 3.25*3.14 inches, 8.255*3.14 cm
    //double distanceInUnits = (distance/(3.25*3.14))*360;
    double distanceInUnits = (distance/(3.25*3.14))*360*(4/3)*(5/4);
    pros::lcd::set_text(5,std::to_string(distanceInUnits));

    // drive until robot has travelled distance
    /*while(fabs(getAverageEncoderVal()) < fabs(distance)){*/
    while(fabs(getAverageEncoderVal()) < fabs(distanceInUnits)){
        updateMotors(0.9*direction*50, 1*direction*50);
        pros::delay(15);
        PIDed = true;
        timeElapsed += 15;
        if(timeElapsed > 2000){
            PIDed = false;
            break;
        }
    }
    
    if(PIDed){
        updateMotors(-direction*30, -direction*30);
        pros::delay(50);
    }

    updateMotors(0,0);
    pros::delay(100);
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
    pros::delay(100);
}

void switchIntake(){
    toggleIntake(true,-1);
    pros::delay(250);
    toggleIntake(true,1);
}

void backIntoGoal(double distance, int direction, double grabTime){
    resetDriveEncoders();

    bool PIDed = false;
    int timeElapsed = 0;
    bool goalClamped = false;
    // 360 degrees = 3.25*3.14 inches, 8.255*3.14 cm
    //double distanceInUnits = (distance/(3.25*3.14))*360;
    double distanceInUnits = (distance/(3.25*3.14))*360*(4/3)*(5/4);
    double distanceToClamp = (grabTime/(3.25*3.14))*360*(4/3)*(5/4);
    pros::lcd::set_text(5,std::to_string(distanceInUnits));

    // drive until robot has travelled distance
    /*while(fabs(getAverageEncoderVal()) < fabs(distance)){*/
    while(fabs(getAverageEncoderVal()) < fabs(distanceInUnits)){
        updateMotors(0.9*direction*50, 1*direction*50);
        pros::delay(15);
        PIDed = true;
        timeElapsed += 15;

        if(fabs(getAverageEncoderVal()) > distanceToClamp && !goalClamped){
            grabber.set_value(true);
            goalClamped = true;
        }

        if(timeElapsed > 2000){
            PIDed = false;
            break;
        }
    }
    
    if(PIDed){
        updateMotors(-direction*30, -direction*30);
        pros::delay(50);
    }

    updateMotors(0,0);
    pros::delay(100);
}





void test(){
    rotate(90,-1);
    rotateSmall(45,1);
    //rotateSmall(45,-1);
    while(true){
        pros::lcd::set_text(1,std::to_string(inertial.get_rotation()));
        pros::delay(15);
    }
}

// bot = 15.5in lengthwise
void skills(){
    isRed = true;
    toggleIntake(true,1);
    pros::delay(700);
    toggleIntake(false,0); // alliance stake

    //
    translate(16,1);
    rotate(270,1);
    pros::delay(500);
    inertial.tare_rotation();
    translate(20,-1);
    resetRotation();
    pros::delay(250);
    slowTranslate(12,-1);
    grabber.set_value(HIGH);
    resetRotation(); // fix rotation after running into goal
    pros::delay(250);
    rotate(180,1);
    inertial.tare_rotation();
    
    toggleIntake(true,1);
    slowTranslate(36,1); // grab two rings
    switchIntake();
    slowTranslate(6,1);
    switchIntake();
    resetRotation(); /////////////////////////////
    //translate(18,-1);
    translate(24+6,-1); // go back
    //toggleIntake(true,-1);
    //pros::delay(500);
    toggleIntake(false,0);
    resetRotation();
    //rotate(30,1); // to third ring (GOES WITH TRANSLATE 18,1)
    rotate(45,1);
    toggleIntake(true,1);
    slowTranslate(15,1); // into 3rd ring
    pros::delay(1000);
    translate(15,-1); // to og position
    switchIntake();
    toggleIntake(false,0);

    resetRotation(); // reset to wall?
    translate(14,1); // to corner of tile
    rotate(180 + 45,1); // face goal to corner
    translate(20,-1); // go back into goal
    pros::delay(500);
    grabber.set_value(LOW); // drop goal
    pros::delay(200);
    translate(20,1);

    // trans into 2ND SECTION
    rotate(135,1); // face wall
    rotate(180,1);
    //translate(24,-1); // bot = 15.5 in lengthwise
    updateMotors(-50,-50);
    pros::delay(1000);
    updateMotors(0,0);
    inertial.tare_rotation();
    
    // 
    toggleIntake(true,1); 
    slowTranslate(24,1);
    resetRotation();
    slowTranslate(24,1);
    resetRotation();
    slowTranslate(24-15.5/2 + 5,1);
    toggleIntake(false,0);



    // face grabber to goal
    rotate(180,1);
    inertial.tare_rotation();
    translate(20,-1);
    resetRotation(); // fix rotation before goal
    slowTranslate(12,-1); // back into goal
    grabber.set_value(HIGH);
    resetRotation(); // fix rotation after running into goal
    pros::delay(250);

    rotate(180,1); // face 2 rings
    inertial.tare_rotation();


    //2ND SECTION

    toggleIntake(true,1);
    slowTranslate(36,1); // grab two rings
    slowTranslate(6,1);
    resetRotation();
    
    translate(24+6,-1); // 1.5 tiles back (perfect 45 deg from 3rd ring)  //maybe change this
    toggleIntake(true,-1);
    pros::delay(500);
    toggleIntake(false,0);
    resetRotation();
    //
    rotate(360-45,1); // to third ring
    toggleIntake(true,1);
    slowTranslate(15,1); // pick ring up
    pros::delay(1000);
    translate(15,-1); // to og position
    toggleIntake(false,0);

    //resetRotation(); // reset to wall?
    rotate(45,1);
    translate(6,1);
    rotate(90+45,1); // face goal to corner
    inertial.tare_rotation();
    translate(20+12,-1); // go back into goal
    pros::delay(500);
    grabber.set_value(LOW); // drop goal
    pros::delay(500);
    translate(20,1);

    // max pts: 3 (alliance top) + 6 (2 top stake) + 4 (4 normal rings) + 10 (2 corner stakes)
    //          = 23 pts
}

void redLeftAWP(){
    isRed = true;
    armPID(100,1);
    translate(22,-1);
    backIntoGoal(25,-1,24);


    resetRotation();
    //translate(10,1);
    translate(8-0.5,1);
    resetRotation();

    rotate(90,1);
    toggleIntake(true,1);
    inertial.tare_rotation();
    translate(12,1);
    resetRotation();
    slowTranslate(12+2,1);
    translate(6-2-1+2-1,-1); //back up
    switchIntake();
    rotate(90,1);
    inertial.tare_rotation();

    slowTranslate(16,1);  // border ring

    pros::delay(1000);
    
    slowTranslate(16,-1);
    //translate(16,-1);
    resetRotation();
    switchIntake();

    rotate(270,1);
    toggleIntake(false,0);
    toggleIntake(true,-1);
    translate(12+2+2,1);

    rotate(90,1);
    toggleIntake(true,1);
    slowTranslate(16+1,1);
    pros::delay(1000);
    translate(16,-1);
}

void redLeftAB(){

}

void redRightAWP(){
    // red right side
    isRed = true;
    translate(22,-1);
    slowTranslate(25,-1);
    pros::delay(200);
    grabber.set_value(HIGH);
    resetRotation();
    translate(10,1);
    resetRotation();

    rotate(270,1);
    toggleIntake(true,1);
    inertial.tare_rotation();
    translate(12,1);
    resetRotation();
    slowTranslate(14,1);
    //pros::delay(500);
    
    translate(4,-1);
    //toggleIntake(false,0);
    resetRotation();
    pros::delay(250);

    //
    rotate(180,1);
    /*toggleIntake(true,1);
    translate(24,1);
    rotate(45,1);
    translate(20-5,1);
    rotate(45,1);*/
    translate(37,1);
    toggleIntake(false,0);
    slowTranslate(10,1);
    //
}

void redRightAB(){

}

void blueLeftAWP(){
    // blue left
    isBlue = true;
    translate(22,-1);
    slowTranslate(25,-1);
    pros::delay(200);
    grabber.set_value(HIGH);
    resetRotation();
    //translate(10,1);
    translate(7,1);
    resetRotation();

    rotate(90,1);
    toggleIntake(true,1);
    inertial.tare_rotation();
    translate(12,1);
    resetRotation();
    slowTranslate(14-2,1);
    //pros::delay(500);
    
    translate(4,-1);
    
    resetRotation();
    pros::delay(250);

    //
    rotate(180,1);
    toggleIntake(false,0);
    translate(24,1);
    rotate(360-45,1);
    translate(18,1);
    rotate(45,1);
    //
}

void blueLeftAB(){

}

void blueRightAWP(){
    isBlue = true;
    // blue right side
    translate(22,-1);
    slowTranslate(25,-1);
    pros::delay(200);
    grabber.set_value(HIGH);
    resetRotation();
    //translate(10,1);
    translate(8,1);
    resetRotation();

    rotate(270,1);
    toggleIntake(true,1);
    inertial.tare_rotation();
    translate(12,1);
    resetRotation();
    slowTranslate(12-1,1); // ring 1
    pros::delay(500);
    // NORMAL RING 1 

    switchIntake();
    translate(4-2,-1);
    rotate(270,1);
    inertial.tare_rotation();

    slowTranslate(15,1); // get border ring 1
    pros::delay(1250);
    switchIntake();
    
    translate(16,-1); 
    resetRotation(); 
    switchIntake();


    rotate(90,1);
    toggleIntake(false,0);
    toggleIntake(true,-1);
    translate(12+2+2,1);

    rotate(270,1);
    toggleIntake(true,1);
    slowTranslate(16+1,1);
    pros::delay(1000);
    translate(16,-1);

    // CUTOFF LADDER TOUCH
    /*pros::delay(250);

    rotate(270,1);
    translate(35,1);
    slowTranslate(20,1);*/
    /*rotate(90,1);
    translate(7+3,1);
    rotate(270,1);
    slowTranslate(16,1);
    pros::delay(1000);
    translate(16,-1);*/


    // TRY 2ND BORDER RING


    /*translate(4+24,-1);
    switchIntake();
    rotate(270+45,1);
    translate(12,1);
    slowTranslate(6+2,1);
    pros::delay(500);
    translate(17,-1);
    rotate(45,1);

    translate(24+8,1);
    rotate(270,1);
    slowTranslate(16,1);

    //*/



    //
    /*rotate(270,1);
    
    
    translate(48,1);
    toggleIntake(false,0);*/
    //
    
    /*translate(36,-1); // back (into goal)
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

void blueRightAB(){

}

