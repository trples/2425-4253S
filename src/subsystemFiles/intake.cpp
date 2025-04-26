#include "main.h"
#include "pros/misc.h"
#include "subsystemHeaders/global.hpp"

bool intakeOn = false;
bool intakeReversed = false;
int power = 100;
double multiplier = 1;



void setIntake(){
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)&&controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
        pros::lcd::set_text(8, "both pressed");
    }else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
        intake.move(int(power*multiplier));
    }else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
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
