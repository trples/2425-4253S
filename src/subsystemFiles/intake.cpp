#include "main.h"
#include "subsystemHeaders/global.hpp"

// motor 7

bool intakeOn = false;
bool intakeReversed = false;

void setIntake(){
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
        intake.move(100);
    }else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
        intake.move(-100);
    }else{
        intake.brake();
    }

    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
        intakeBot.move(100);
    }else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
        intakeBot.move(-100);
    }else{
        intakeBot.brake();
    }
}

void toggleIntake(bool status, int direction){
    //intake.move(status*127*direction);
    //intakeBot.move(status*127*direction);
    if(status){
        intake.move(127*direction);
        intakeBot.move(127*direction);
    }else{
        intake.brake();
        intakeBot.brake();
    }
}