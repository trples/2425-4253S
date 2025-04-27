#include "main.h"
#include "pros/misc.h"
#include "subsystemHeaders/global.hpp"

bool extended = true;
bool doinkerDown = false;

void setGrabber(){
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
        grabber.set_value(false);
    }else{
        grabber.set_value(true);
    }

    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)){
        if(doinkerDown){ // extended
            doinker.set_value(false);
            doinkerDown = !doinkerDown;
        }else{ // retracted
            doinker.set_value(true);
            doinkerDown = !doinkerDown;
        }
    }
}

void toggleGrabber(bool status){
    grabber.set_value(status);
}

void toggleDoinker(bool status){
    doinker.set_value(status);
}

void switchGrabber(){
    if(extended){
            grabber.set_value(HIGH);
        }else{
            grabber.set_value(LOW);
        }
    extended = !extended;
}