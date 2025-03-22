#include "main.h"
#include "subsystemHeaders/global.hpp"

bool intakeOn = false;
bool intakeReversed = false;
int power = 80;
double multiplier = 1;

void setIntake(){
    /*if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)){
        if(multiplier == 1){
            multiplier = 0.75;
            intakeSlowed = true;
            intakeTimer = 0;
        }else{
            multiplier = 1;
        }
    }*/
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
        intakeLower.move(int(power*multiplier));
        intakeUpper.move(int(power*multiplier));
    }else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
        intakeLower.move(int(-1*power*multiplier));
        intakeUpper.move(int(-1*power*multiplier));
    }else{
        intakeLower.brake();
        intakeUpper.brake();
    }

    /*if(intakeSlowed && intakeTimer > 4000){
        intakeSlowed = false;
        multiplier = 1;
    }*/
}

void toggleIntake(bool status, double direction){
    if(status){
        intake.move(int(power*direction));
    }else{
        intake.brake();
    }
}

void toggleWeak(int direction){
    intake.move(127/3*direction);
}

// proximity > 150 or 200 = in front of sensor; max 255
// HUE:
// red [0,~20]
// blue [180,210]
void score(int breakoutTime){
    toggleIntake(true,1);
    int timeElapsed = 0;
    bool broken = false;
    while(optical.get_proximity() < 150){
        pros::delay(15);
        timeElapsed += 15;
        if(timeElapsed > breakoutTime){
            broken = true; 
            break;
        }

    }
    if(!broken){

    
    if(isBlue){ // blue alliance
        if(optical.get_hue() < 30){
            toggleWeak(1);
            pros::lcd::set_text(3,"blah");
        }
    }else{ // red alliance
        if(optical.get_hue() > 50){
            toggleWeak(1);
            pros::lcd::set_text(3,"halb");
        }
    }
    pros::delay(300);
    }
}

void moveAndScore(double distance, int timeout){
    resetDriveEncoders();

    bool PIDed = true;
    int timeElapsed = 0;
    bool running = true;
    bool broken = false;
    bool scored = false;

    double distanceInUnits = (distance/(3.25*3.14))*360*(4/3)*(5/4);

    // drive until robot has travelled distance
    while(fabs(getAverageEncoderVal()) < fabs(distanceInUnits)){
        updateMotors(0.9*1*50, 1*1*50);
        pros::delay(15);
        timeElapsed += 15;
        /*if(running&&(fabs(getAverageEncoderVal()) >= fabs(distanceInUnits))){ // at distance

            running = false;
        }*/
        toggleIntake(true,1);

        if(optical.get_proximity() > 150){ // within distance detected
            broken = true;
            if(isBlue){ // blue alliance
                if(optical.get_hue() < 30){
                    toggleWeak(1);
                    pros::lcd::set_text(3,"blah");
                    scored = true;
                }
            }else if(isRed){ // red alliance
                if(optical.get_hue() > 50){
                    toggleWeak(1);
                    pros::lcd::set_text(3,"halb");
                    scored = true;
                }
            }
        }

        if(timeElapsed > timeout){
            PIDed = false;
            broken = true;
            break;
        }
    }
    
    if(PIDed){
        updateMotors(-1*30, -1*30);
        pros::delay(50);
        updateMotors(0,0);

    }

    while(timeElapsed < timeout){
        toggleIntake(true,1);
        if(!broken&&!scored){
            if(isBlue){ // blue alliance
                if(optical.get_hue() < 30){
                    toggleWeak(1);
                    scored = true;
                    break;
                }
            }else if(isRed){ // red alliance
                if(optical.get_hue() > 50){
                    toggleWeak(1);
                    scored = true;
                    break;
                }
            }
        }
        timeElapsed += 15;
        pros::delay(15);
    }
}