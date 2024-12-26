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
    while(optical.get_proximity() < 150){}
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
    pros::delay(1500);
}