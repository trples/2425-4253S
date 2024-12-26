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
    /*if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)){
        driveReversed = !driveReversed;
    }*/

    // arcade drive; left = turn, right = forwards/backwards
    int rotate = 0.8*controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int power = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
    
    // rotate and power variables for arcade drive
    int left = power + rotate;
    int right = power - rotate;

    /*if(driveReversed){
        updateMotors(-1*right,-1*left);
    }else{*/
        updateMotors(1*left,1*right);
    //}
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

    static double kP = 0.6;
    static double kD = 0.5;

    static double error = 0;
    static double prevError = 0;

    prevError = error;
    //error = goalDeg - fabs(inertial.get_rotation());
    error = deg - fabs(normalizeAngle(inertial.get_rotation()));

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

int smallTurnP(int deg){
    goalDeg = fabs(normalizeAngle(deg + startingDeg));

    //goalDeg = deg + startingDeg;

    static double kP = 1;
    static double kD = 0.5;

    /*
    static double kP = 0.8;
    static double kD = 0//.4;
    */

    static double error = 0;
    static double prevError = 0;

    prevError = error;
    //error = goalDeg - fabs(inertial.get_rotation());
    error = deg - fabs(normalizeAngle(inertial.get_rotation()));

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

void oldRotateSmall(double deg, int direction){ // -1 = left, 1 = right
    int timeElapsed = 0;
    bool PIDed = true;
    // if z axis (upright)
    inertial.tare_rotation();

    while(fabs(inertial.get_rotation()) < fabs(deg)){
        //int power = turnPID(deg);
        int power = smallTurnP(deg);
        updateMotors(direction*power, -direction*power);
        pros::lcd::set_text(2,std::to_string(inertial.get_rotation()));
        timeElapsed+=15;
        pros::delay(15);
        if(timeElapsed > 1500){
            PIDed = false;
            break;
        }
    }
    if(PIDed){
        updateMotors(direction*-50, direction*50);
        pros::delay(50);
    }

    updateMotors(0,0);
}

void oldRotateSlow(double deg, int direction){ // -1 = left, 1 = right
    int timeElapsed = 0;
    bool PIDed = true;
    // if z axis (upright)
    inertial.tare_rotation();

    while(fabs(inertial.get_rotation()) < fabs(deg)){
        //int power = turnPID(deg);
        updateMotors(direction*30, -direction*30);
        pros::lcd::set_text(2,std::to_string(inertial.get_rotation()));
        timeElapsed+=15;
        pros::delay(15);
        /*if(timeElapsed > 1500){
            PIDed = false;
            break;
        }*/
    }
    if(PIDed){
        updateMotors(direction*-50, direction*50);
        pros::delay(30);
    }

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

    static double kP = 0.9;
    static double kD = 0.5;

    static double error = 0;
    static double prevError = 0;
    static bool rightTurn = true;

    prevError = error;
    //error = goalDeg - fabs(inertial.get_rotation());

    
    
    //error = fabs(deg - (inertial.get_heading()));
    error = fixHeading(deg - (inertial.get_heading())); // [0,360]

    if (error > 180) {
            error -= 360; 
    } else if (error < -180) {
        error += 360;
    }

    //error = fabs(error);

    //double power = fabs(error)*kP;
    int power = int(fabs(error*kP) + (prevError-error)*kD);

    std::string before = std::to_string(power);
    pros::lcd::set_text(0, before);

    //if (power > 127) power = 127;
    if (power > 60) power = 60;
    if (power < -60) power = -60;

    std::string after = std::to_string(power);
    pros::lcd::set_text(1, after);

    if(error > 0){
        return power;
    }else{
        return power*-1;
    }
    
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
        updateMotors(
            //direction*
            power, 
            //-direction*
            -power);
        timeElapsed+=15;
        pros::delay(15);
        error = fabs((inertial.get_heading()) - targetHeading);
        if(timeElapsed > 1500){
            PIDed = false;
            break;
        }
    }
    
    if(PIDed){
        updateMotors(-50*direction, 50*direction);
        pros::delay(50);
    }


    updateMotors(0,0);
    
    pros::delay(100);
}

void turnToHeading(double deg){ // -1 = left, 1 = right
    int timeElapsed = 0;
    bool PIDed = false;
    //startingDeg = inertial.get_heading();
    
    
    double targetHeading = fixHeading(deg);
    error = fixHeading(deg - (inertial.get_heading()));
    int direction;
    int power;

    while(error > 1){
        PIDed = true;
        power = HturnP(targetHeading);
        updateMotors(
            //direction*
            power, 
            //-direction*
            -power);
        timeElapsed+=15;
        pros::delay(15);
        error = fabs((inertial.get_heading()) - targetHeading);
        if(timeElapsed > 1500){
            PIDed = false;
            break;
        }
    }

    if(power > 0){
        direction = 1;
    }else{
        direction = -1;
    }
    
    if(PIDed){
        updateMotors(-50*direction, 50*direction);
        pros::delay(50);
    }


    updateMotors(0,0);
    
    pros::delay(100);
}

void turnToHeadingSmall(double deg, int direction){
    int timeElapsed = 0;
    bool PIDed = false;
    //startingDeg = inertial.get_heading();
    
    
    double targetHeading = fixHeading(deg);
    error = fixHeading(deg - (inertial.get_heading()));
    int power;

    while(error > 1){
        PIDed = true;
        updateMotors(
            direction*
            40, 
            direction*
            -40);
        timeElapsed+=15;
        pros::delay(15);
        error = fabs((inertial.get_heading()) - targetHeading);
        if(timeElapsed > 1500){
            PIDed = false;
            break;
        }
    }
    
    if(PIDed){
        updateMotors(-50*direction, 50*direction);
        pros::delay(50);
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
    double distanceInUnits = (distance/(3.25*3.14))*360*(48.0/36.0);
    double distanceToClamp = (grabTime/(3.25*3.14))*360*(48.0/36.0);
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

void pickRingUp(){
    updateMotors(50,50);
    toggleIntake(true,1);
    int timeElapsed = 0;
    while(optical.get_proximity() < 150){
        timeElapsed += 15;
        if(timeElapsed > 1500){
            break;
        }
    }
    toggleIntake(false,0);
    updateMotors(0,0);
}



// USE     rotate(180,1);


void test(){
    isRed = true;
    
    toggleIntake(true,1);
    pros::delay(700);
    toggleIntake(false,0);

    translate(12+1,1);
    armPID(80,-1);

    // 

    turnToHeading(90); 
    translate(20,-1);
    backIntoGoal(8+2,-1,6);
    grabber.set_value(HIGH);
    turnToHeading(90);
    slowTranslate(6+2,1);

    turnToHeading(180); // 
    turnToHeading(270);

    //inertial.tare_rotation();
    toggleIntake(true,1);

    slowTranslate(20,1);
    pros::delay(500);
    //resetRotation();
    slowTranslate(10 + 6,1); // 10 @ 24 here

    translate(28+2 + 6 - 2,-1);
    //resetRotation();

    // side rings

    turnToHeading(0);


    translate(24-4,1);
    // 
    turnToHeading(270);
    //rotate(90,-1);

    translate(24,1);


    // go back
    turnToHeading(180);//

    translate(24-4,1);
    
    slowTranslate(12,1);
    translate(12-4,-1);
    //resetRotation();
    
    // corner 

    turnToHeading(45);

    translate(15+3,-1);
    grabber.set_value(LOW);
    toggleIntake(true,-1);
    translate(12,1); // 12
    toggleIntake(false,0);
    
    // pick up "ring"
    turnToHeading(270);

    toggleIntake(true,1);
    slowTranslate(18,1);
    pros::delay(500);
    toggleIntake(false,0);
    translate(6,-1);



    //reset

    turnToHeading(90);

    updateMotors(-50,-50);

    pros::delay(1000);

    updateMotors(0,0); 

    inertial.set_heading(90);

    // traverse

    

    translate(24 + 24 + 15,1);//+8,1);



    turnToHeading(270);


    // mirror

    translate(20,-1);
    backIntoGoal(12,-1,8);
    grabber.set_value(HIGH);
    turnToHeading(270);
    //resetRotation();
    slowTranslate(10,1);

    turnToHeading(90);
    //rotate(180,1);
    toggleIntake(true,1);

    slowTranslate(20,1); //
    pros::delay(500);
    slowTranslate(10 + 6,1); // 10

    translate(12,-1);

    turnToHeading(180);

    slowTranslate(12,1); // pick 3rd ring
    translate(12,-1);

    turnToHeading(270+45);
    // corner
    translate(16,-1);

    grabber.set_value(LOW);
    toggleIntake(true,-1);
    translate(14,1);
    toggleIntake(false,0);


    //reset
    /*turnToHeading(180);
    toggleIntake(true,1);
    slowTranslate(11,1);
    toggleIntake(false,0);*/
    turnToHeading(90);

    turnToHeading(0);
    updateMotors(-50,-50);
    pros::delay(750);
    updateMotors(0,0);
    inertial.set_heading(0);



    // 2nd sector

    translate(15,1);
    toggleIntake(true,1);
    translate(48,1);
    //toggleIntake(false,0);
    oldRotateSlow(45,-1);

    translate(34+3,1);

    toggleIntake(true,1);
    pros::delay(750);
    toggleIntake(false,0);

    turnToHeading(135);

    backIntoGoal(34+3-3,-1,35-3); // get goal

    turnToHeading(135+90);

    toggleIntake(true,1);

    translate(34,1);

    turnToHeading(135);

    sweeper.set_value(HIGH);

    translate(34,-1);

    turnToHeading(45);
    turnToHeading(360-45);

    turnToHeading(135);

    /*oldRotateSlow(45,-1);

    translate(48,-1);

    turnToHeading(0);
    turnToHeading(270);
    turnToHeading(135);*/

    translate(18+5,-1);
    grabber.set_value(LOW);
    translate(15,1);
}

void test1(){
    isRed = true;
    
    toggleIntake(true,1);
    pros::delay(700);
    toggleIntake(false,0);

    translate(12+1,1);
    armPID(80,-1);

    // 

    rotate(90,1); 
    translate(20,-1);
    backIntoGoal(8+2,-1,6+2);
    grabber.set_value(HIGH);
    slowTranslate(6+2,1);


    rotate(180,1);
    //inertial.tare_rotation();
    toggleIntake(true,1);

    slowTranslate(20,1);
    pros::delay(500);
    //resetRotation();
    slowTranslate(10 + 6,1); // 10 @ 24 here

    translate(28+2 + 6 - 2,-1);
    //resetRotation();

    // side rings

    rotate(90,1);


    translate(24-4,1);
    // 
    rotate(90,-1);
    //rotate(90,-1);

    translate(24,1);


    // go back
    rotate(90,-1);//
    inertial.tare_rotation();

    translate(24-4,1);
    
    slowTranslate(6,1);
    slowTranslate(6,-1);
    //resetRotation();
    
    // corner 

    rotate(135,-1);

    translate(15+3,-1);
    grabber.set_value(LOW);
    toggleIntake(false,0);
    translate(12,1); // 12
    
    pros::delay(2000);

    oldRotateSlow(45,1);

    updateMotors(-50,-50);

    pros::delay(1500);

    updateMotors(0,0); 

    // traverse

    

    translate(24 + 24 + 15,1);//+8,1);



    oldRotateSlow(180,1);

    inertial.tare_rotation();

    // mirror

    translate(20,-1);
    backIntoGoal(12,-1,10);
    //resetRotation();
    slowTranslate(10,1);

    rotate(180,1);
    //rotate(180,1);
    //
    inertial.tare_rotation();
    toggleIntake(true,1);

    translate(20,1); //
    pros::delay(500);
    resetRotation();
    slowTranslate(10 + 4,1); // 10

    slowTranslate(6,1);

    rotate(90,-1);

    translate(6,-1);

    grabber.set_value(LOW);

    translate(6,1);

    rotate(90,-1);








    /*toggleIntake(true,1);
    pros::delay(700);
    toggleIntake(false,0);

    translate(12,1);
    armPID(100,1);

    // 

    rotate(90,1);
    translate(20,-1);
    backIntoGoal(8+2,-1,6+2);
    grabber.set_value(HIGH);
    slowTranslate(6+2,1);


    //
    oldRotate(180,-1);
    inertial.tare_rotation();
    toggleIntake(true,1);

    slowTranslate(20,1);
    pros::delay(500);
    resetRotation();
    slowTranslate(10 + 6,1); // 10

    translate(28+2 + 6 - 2,-1);
    //resetRotation();

    rotate(90,1);


    translate(24-2 -2,1);
    // 
    oldRotateSmall(90,-1);
    //rotate(90,-1);

    translate(24+2-2,1);

    oldRotateSmall(90,-1);//
    inertial.tare_rotation();

    translate(20+4,1);
    
    slowTranslate(6,1);
    slowTranslate(6,-1);
    //resetRotation();
    
    // corner 

    oldRotate(135,-1);
    inertial.tare_rotation();

    translate(10+5-3,-1);
    grabber.set_value(LOW);
    toggleIntake(false,0);
    translate(10-2-3,1); // 12
    resetRotation();


    oldRotateSlow(45,1);

    updateMotors(-50,-50);

    pros::delay(1500);

    updateMotors(0,0); 

    // traverse

    inertial.tare_rotation();

    translate(24 + 24 + 8,1);

    resetRotation();

    oldRotateSlow(180,1);

    inertial.tare_rotation();

    // mirror

    translate(20,-1);
    backIntoGoal(12,-1,10);
    //resetRotation();
    slowTranslate(10,1);

    oldRotate(180,1);
    //rotate(180,1);
    //
    inertial.tare_rotation();
    toggleIntake(true,1);

    translate(20,1); //
    pros::delay(500);
    resetRotation();
    slowTranslate(10 + 4,1); // 10

    slowTranslate(6,1);

    oldRotateSmall(90,-1);

    translate(6,-1);

    grabber.set_value(LOW);

    translate(6,1);

    oldRotateSmall(90,-1);*/
   
}

// bot = 15.5in lengthwise
void skills(){
    isRed = true;
    
    toggleIntake(true,1);
    pros::delay(700);
    toggleIntake(false,0);

    translate(12+1,1);
    armPID(80,-1);

    // 

    turnToHeading(90); 
    translate(20,-1);
    backIntoGoal(8+2,-1,6);
    grabber.set_value(HIGH);
    turnToHeading(90);
    slowTranslate(6+2,1);

    turnToHeading(180); // 
    turnToHeading(270);

    toggleIntake(true,1);

    slowTranslate(20,1);
    pros::delay(500);

    slowTranslate(10 + 6,1); // 10 @ 24 here

    translate(28+2 + 6 - 2,-1);

    // side rings

    turnToHeading(0);


    translate(24-4,1);
    // 
    turnToHeading(270);

    translate(24,1);


    // go back
    turnToHeading(180);

    translate(24-4,1);
    
    slowTranslate(12,1);
    translate(12-4,-1);
    
    // corner 

    turnToHeading(45);

    translate(15+3,-1);
    grabber.set_value(LOW);
    toggleIntake(true,-1);
    translate(12,1); // 12
    toggleIntake(false,0);
    
    // pick up "ring"
    turnToHeading(270);

    toggleIntake(true,1);
    slowTranslate(18,1);
    pros::delay(500);
    toggleIntake(false,0);
    translate(6,-1);


    //reset

    turnToHeading(90);

    updateMotors(-50,-50);

    pros::delay(1000);

    updateMotors(0,0); 

    inertial.set_heading(90);

    // traverse

    translate(24 + 24 + 15,1);


    turnToHeading(270);


    // mirror

    translate(20,-1);
    backIntoGoal(12,-1,8);
    grabber.set_value(HIGH);
    turnToHeading(270);
    slowTranslate(10,1);

    turnToHeading(90);
    toggleIntake(true,1);

    slowTranslate(20,1); //
    pros::delay(500);
    slowTranslate(10 + 6,1); // 10

    translate(12,-1);

    turnToHeading(180);

    slowTranslate(12,1); // pick 3rd ring
    translate(12,-1);

    turnToHeading(270+45);
    // corner
    translate(16,-1);

    grabber.set_value(LOW);
    toggleIntake(true,-1);
    translate(14,1);
    toggleIntake(false,0);


    //reset
    turnToHeading(90);

    turnToHeading(0);
    updateMotors(-50,-50);
    pros::delay(750);
    updateMotors(0,0);
    inertial.set_heading(0);

    // 2nd sector

    translate(15,1);
    toggleIntake(true,1);
    translate(48,1);
    oldRotateSlow(45,-1);

    translate(34+3,1);

    toggleIntake(true,1);
    pros::delay(750);
    toggleIntake(false,0);

    turnToHeading(135);

    backIntoGoal(34+3-3,-1,35-3); // get goal

    turnToHeading(135+90);

    toggleIntake(true,1);

    translate(34,1);

    turnToHeading(135);

    sweeper.set_value(HIGH);

    translate(34,-1);

    turnToHeading(45);
    turnToHeading(360-45);

    turnToHeading(135);

    translate(18+5,-1);
    grabber.set_value(LOW);
    translate(15,1);
}

void redLeftAWP(){
    // alliance stake /*
    isRed = true;
    redLeftRingRush();
    //redLeftDoubleStake();
}

// on line
void redLeftRingRush(){
    inertial.set_heading(0);
    translate(22,-1);
    backIntoGoal(10,-1,7);
    translate(12,1);

    turnToHeading(135);
    toggleIntake(true,1);
    translate(15,1);
    toggleIntake(false,0);
    slowTranslate(4+1,1);
    toggleIntake(true,1);
    pros::delay(200);
    score();
    
    translate(4+1,-1);
    turnToHeading(45);
    toggleIntake(false,0);

    slowTranslate(8,1);
    toggleIntake(true,1);
    slowTranslate(2,1);

    turnToHeading(270);
    slowTranslate(30,1);
    toggleIntake(false,0);
    updateMotors(30,30);
}

// facing alliance stake
void redLeftDoubleStake(){
    inertial.set_heading(360-45);
    slowTranslate(9+1,1);
    armPID(180,-1);
    armNeutral();
    translate(18,-1);
    oldRotateSlow(45,1);
    
    translate(22,-1);
    backIntoGoal(10,-1,7);
    translate(12,1);



    turnToHeading(90);
    toggleIntake(true,1);
    translate(12,1);
    toggleIntake(false,0);
    slowTranslate(4,1);
    toggleIntake(true,1);
    pros::delay(200);
    score();
    // ladder
    translate(4+1,-1);
    turnToHeading(270);

    slowTranslate(30,1);
    toggleIntake(false,0);
    updateMotors(30,30);
}

void redLeftAB(){
    isRed = true;
    inertial.set_heading(0);

    armPID(100,-1);
    translate(22,-1);
    backIntoGoal(10,-1,7);
    translate(12,1);


    turnToHeading(135);
    toggleIntake(true,1);
    translate(15,1);
    toggleIntake(false,0);
    slowTranslate(4+1,1);
    toggleIntake(true,1);
    pros::delay(500);
    score();
    
    translate(4+1,-1);
    turnToHeading(45);
    toggleIntake(false,0);

    slowTranslate(8,1);
    toggleIntake(true,1);
    slowTranslate(2,1);

    // 2nd border ring
    turnToHeading(180-7);
    toggleIntake(false,0);
    slowTranslate(13,1);
    slowTranslate(2,1);
    score();

    slowTranslate(5,-1);
}

// 2 goals
void redRightAWP(){
    // red right side
    isRed = true;
    armPID(100,1);
    translate(22,-1);
    backIntoGoal(10,-1,7);
    resetRotation();
    translate(12,1);

    oldRotate(90,-1);

    translate(20,1);
    slowTranslate(4+1,1);
    toggleIntake(true,1);
    pros::delay(500);
    

    oldRotate(170,-1);
    translate(24,1);
    toggleIntake(false,0);
    updateMotors(30,30);
}

void redRightAB(){
    // ALSO AWP

    isRed = true;
    inertial.set_heading(45);
    slowTranslate(9+1,1);
    armPID(180,-1);
    armNeutral();
    translate(18,-1);
    oldRotateSlow(45,-1);

    armPID(90,1);

    translate(22,-1);
    backIntoGoal(7,-1,5);
    //resetRotation();
    translate(10-3,1);

    turnToHeading(270);
    //toggleIntake(true,1);
    translate(18,1);
    slowTranslate(4+1,1);
    toggleIntake(true,1);
    slowTranslate(2,1);
    pros::delay(500);
    toggleIntake(false,0);
    turnToHeading(90);
    toggleIntake(true,1);

    translate(18,1);
    toggleIntake(false,0);
    updateMotors(30,30);
}

// also 2 goals side
void blueLeftAWP(){
    // blue left
    isBlue = true;
    inertial.set_heading(360-45);
    slowTranslate(9+1,1);
    armPID(180,-1);
    armNeutral();
    translate(18,-1);
    oldRotateSlow(45,1);

    armPID(90,1);

    translate(22,-1);
    backIntoGoal(7,-1,5);

    translate(10-3,1);

    turnToHeading(90); // turn to ring

    translate(18,1);
    slowTranslate(4+1,1);
    toggleIntake(true,1);
    slowTranslate(2,1);
    pros::delay(500);
    toggleIntake(false,0);

    // turn to ladder
    turnToHeading(270);
    toggleIntake(true,1);

    translate(18,1);
    toggleIntake(false,0);
    updateMotors(30,30);
}

void blueLeftAB(){
    isBlue = true;
    slowTranslate(8,1);
    armPID(180,1);
    armNeutral();
    translate(18,-1);
    oldRotate(135,1);
    oldRotate(90,-1);

    translate(22,-1);
    backIntoGoal(7,-1,5);
    //resetRotation();
    inertial.tare_rotation();
    translate(10-3,1);

    oldRotate(90,1);
    //toggleIntake(true,1);
    translate(18,1);
    slowTranslate(4+1,1);
    toggleIntake(true,1);
    slowTranslate(2,1);
    pros::delay(500);
    toggleIntake(false,0);
    oldRotate(170,1);
    toggleIntake(true,1);

    translate(18,1);
    toggleIntake(false,0);
    updateMotors(30,30);




    /*armNeutral();
    translate(22,-1);
    backIntoGoal(10,-1,7);
    resetRotation();
    translate(10,1);

    oldRotate(90,1);
    toggleIntake(true,1);
    translate(18,1);
    toggleIntake(false,1);
    slowTranslate(4+1,1);
    toggleIntake(true,1);
    pros::delay(500);
    toggleIntake(false,0);
    translate(23,-1);

    oldRotate(115,-1); //
    grabber.set_value(LOW);
    translate(30+2+0.5,1);
    
    armPID(40,1);
    toggleIntake(true,0.5);
    pros::delay(1000);
    toggleIntake(true,-1);
    pros::delay(200);
    toggleIntake(false,0);
    pros::delay(500);
    //armPID(180,1);
    armPID(180,1);
    
    translate(36,-1);
    armPID(80,1);
    oldRotate(90,-1);
    translate(16,1);
    updateMotors(30,30);*/
}

void blueRightAWP(){
    isBlue = true;
    //blueRightRingRush(); // on line
    blueRightDoubleStake(); // facing alliance
}

// on line
void blueRightRingRush(){
    inertial.set_heading(0);
    translate(22,-1);
    backIntoGoal(10,-1,7);
    translate(12,1);

    turnToHeading(360-135);
    toggleIntake(true,1);
    translate(15,1);
    toggleIntake(false,0);
    slowTranslate(4+1,1);
    toggleIntake(true,1);
    pros::delay(200);
    score();
    
    translate(4+1,-1);
    turnToHeading(360-45);
    toggleIntake(false,0);

    slowTranslate(8,1);
    toggleIntake(true,1);
    slowTranslate(2,1);

    //ladder 
    turnToHeading(90);
    slowTranslate(30,1);
    toggleIntake(false,0);
    updateMotors(30,30);
}

// facing alliance
void blueRightDoubleStake(){
    inertial.set_heading(45);
    slowTranslate(9+1,1);
    armPID(180,-1);
    armNeutral();
    translate(18,-1);
    oldRotateSlow(45,-1);
    
    translate(22,-1);
    backIntoGoal(10,-1,7);
    translate(12,1);

    turnToHeading(270);
    toggleIntake(true,1);
    translate(12,1);
    toggleIntake(false,0);
    slowTranslate(4,1);
    toggleIntake(true,1);
    pros::delay(200);
    score();

    // ladder
    translate(4+1,-1);
    turnToHeading(90);

    slowTranslate(30,1);
    toggleIntake(false,0);
    updateMotors(30,30);
}

void blueRightAB(){
    isBlue = true;
    inertial.set_heading(0);

    armPID(100,-1);
    translate(22,-1);
    backIntoGoal(10,-1,7);
    translate(12,1);


    turnToHeading(360-135);
    toggleIntake(true,1);
    translate(15,1);
    toggleIntake(false,0);
    slowTranslate(4+1,1);
    toggleIntake(true,1);
    pros::delay(500);
    score();
    
    translate(4+1,-1);
    turnToHeading(360-45);
    toggleIntake(false,0);

    slowTranslate(8,1);
    toggleIntake(true,1);
    slowTranslate(2,1);

    // 2nd border ring
    turnToHeading(187);
    toggleIntake(false,0);
    slowTranslate(13,1);
    slowTranslate(2,1);
    score();
    
    slowTranslate(5,-1);
}

