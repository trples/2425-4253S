#include "main.h"
#include "subsystemHeaders/global.hpp"

//bool armIsUp = false;
//bool armIsDown = true;
//bool override = false;

bool isExtended = false;

/*void oldSetArm(){
    int power = 100;
    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)){
        override = !override;
    }

    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
        arm.move(-1*power);
        pros::lcd::set_text(6,"motor reversed");
    }else if(((rotation.get_position()/100 > 30)||override)&&controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
        arm.move(1*power);
        pros::lcd::set_text(6,"motor forwarded");
    }else{
        arm.brake();
    }
}*/

void setArm(){
    int power = 100;
    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)){ // switch
        if(isExtended){ // arm is up
            arm.set_value(false);
        }else{ // arm is down
            arm.set_value(true);
        }
        isExtended = !isExtended;
    }
}

/* 0 = starting
75 = recieve
140 = neutral (not obstructing)
230 = up
300 = on goal
*/