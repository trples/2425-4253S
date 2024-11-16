#include "main.h"
#include "subsystemHeaders/global.hpp"

bool extended = true;

void setGrabber(){
    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)){
        if(extended){
            grabber.set_value(HIGH);
        }else{
            grabber.set_value(LOW);
        }
        extended = !extended;
    }
}

void toggleGrabber(bool status){
    grabber.set_value(status);
}