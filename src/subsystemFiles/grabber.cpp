#include "main.h"
#include "pros/misc.h"
#include "subsystemHeaders/global.hpp"

bool extended = true;
bool sweeping = false;

void setGrabber(){
    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
        grabber.set_value(false);
    }else{
        grabber.set_value(true);
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