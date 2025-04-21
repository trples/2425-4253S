#include "main.h"
#include "subsystemHeaders/global.hpp"

bool intakeOn = false;
bool intakeReversed = false;
int power = 115;
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
        intake.move(int(power*multiplier));
    }else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
        intake.move(int(-1*power*multiplier));
    }else{
        intake.brake();
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
