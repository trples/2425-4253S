#include "main.h"
#include "subsystemHeaders/global.hpp"

bool extended = true;
bool sweeping = false;

void setGrabber(){
    /*if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)){
        if(extended){
            grabber.set_value(HIGH);
        }else{
            grabber.set_value(LOW);
        }
        extended = !extended;
    }*/
    
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
        grabber.set_value(false);
    }else{
        grabber.set_value(true);
    }
}

void toggleGrabber(bool status){
    grabber.set_value(status);
}

void toggleSweeper(bool status){
    sweeper.set_value(status);
}

void switchGrabber(){
    if(extended){
            grabber.set_value(HIGH);
        }else{
            grabber.set_value(LOW);
        }
    extended = !extended;
}