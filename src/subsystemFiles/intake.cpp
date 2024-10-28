#include "main.h"
#include "subsystemHeaders/global.hpp"

// motor 7

bool intakeOn = false;
bool intakeReversed = false;

void setIntake(){
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
        intake.move(127);
    }else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
        intake.move(-127);
    }else{
        intake.brake();
    }
    
    /*if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)){
        intakeOn = !intakeOn;
        std::cout << "L1";
    }
    if(intakeOn&&controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)){
        intakeReversed = !intakeReversed;
        std::cout << "L2";
    }
    if(intakeOn){
            if(!intakeReversed){ // intake not reversed, run normally
                intake.move(127);
            }else{
                intake.move(-127);
            }
        }
    else{
        intake.move(0);
    }*/
}