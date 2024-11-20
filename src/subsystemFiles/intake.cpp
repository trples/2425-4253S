#include "main.h"
#include "subsystemHeaders/global.hpp"

// motor 7

bool intakeOn = false;
bool intakeReversed = false;
int power = 100;

void setIntake(){
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
        intake.move(power);
    }else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
        intake.move(-1*power);
    }else{
        intake.brake();
    }

    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
        intakeBot.move(power);
    }else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
        intakeBot.move(-1*power);
    }else{
        intakeBot.brake();
    }
}

void toggleIntake(bool status, int direction){
    //intake.move(status*127*direction);
    //intakeBot.move(status*127*direction);
    if(status){
        intake.move(power*direction*0.8);
        intakeBot.move(power*direction*0.8);
    }else{
        intake.brake();
        intakeBot.brake();
    }
}