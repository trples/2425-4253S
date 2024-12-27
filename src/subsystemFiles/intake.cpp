#include "main.h"
#include "subsystemHeaders/global.hpp"

bool intakeOn = false;
bool intakeReversed = false;
int power = 127;
double multiplier = 1;

void setIntake(){
    /*if(isBlue){
        if(optical.get_proximity() > 150){
            if(optical.get_hue() < 30){ // red ring
                weakener = 3;
            }
        }else{
            weakener = 1;
        }
    }else{
        if(optical.get_proximity() > 150){
            if(optical.get_hue() > 50){
                weakener = 3;
            }
        }else{
            weakener = 1;
        }
    }*/
    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)){
        if(multiplier == 1){
            multiplier = 0.75;
        }else{
            multiplier = 1;
        }
    }
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
        intake.move(int(power*multiplier));
    }else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
        intake.move(int(-1*power*multiplier));
    }else{
        intake.brake();
    }
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
void score(){
    toggleIntake(true,1);
    int timeElapsed = 0;
    bool broken = false;
    while(optical.get_proximity() < 150){
        pros::delay(15);
        timeElapsed += 15;
        if(timeElapsed > 1000){
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

    double distanceInUnits = (distance/(3.25*3.14))*360*(4/3)*(5/4);

    // drive until robot has travelled distance
    while(fabs(getAverageEncoderVal()) < fabs(distanceInUnits)){
        updateMotors(0.9*1*50, 1*1*50);
        pros::delay(15);
        timeElapsed += 15;
        /*if(running&&(fabs(getAverageEncoderVal()) >= fabs(distanceInUnits))){ // at distance

            running = false;
        }*/

        if(optical.get_proximity() > 150){ // within distance detected
            broken = true;
            if(isBlue){ // blue alliance
            if(optical.get_hue() < 30){
                toggleWeak(1);
                pros::lcd::set_text(3,"blah");
            }
            }else if(isRed){ // red alliance
                if(optical.get_hue() > 50){
                    toggleWeak(1);
                    pros::lcd::set_text(3,"halb");
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
    }

    if(!broken){
        if(isBlue){ // blue alliance
            if(optical.get_hue() < 30){
                toggleWeak(1);
            }
        }else if(isRed){ // red alliance
            if(optical.get_hue() > 50){
                toggleWeak(1);
            }
        }
    }

    updateMotors(0,0);
    pros::delay(100);
}