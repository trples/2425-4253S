#include "main.h"
#include "pros/misc.h"
#include "subsystemHeaders/global.hpp"

bool isExtended = false;
 
// right arrow = up
// Y = down

void setArm(){
    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)){
        if(!isExtended){ // arm is up
            arm_1.set_value(true);
            arm_2.set_value(true);
            armGoalPosition = true;
            isExtended = !isExtended;
        }
    }else if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)){
        if(isExtended){
            arm_1.set_value(false);
            arm_2.set_value(false);
            armGoalPosition = false;
            isExtended = !isExtended;
        }
    }
}

void toggleArm(bool state){
    if(state){ // arm is up
            arm_1.set_value(true);
            arm_2.set_value(true);
            armGoalPosition = true;
    }else{
            arm_1.set_value(false);
            arm_2.set_value(false);
            armGoalPosition = false;
    }
}