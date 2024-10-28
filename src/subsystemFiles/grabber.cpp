#include "main.h"
#include "subsystemHeaders/global.hpp"

bool extended = true;

void toggleGrabber(){
    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)){
        if(extended){
            grabber.set_value(HIGH);
        }else{
            grabber.set_value(LOW);
        }
        extended = !extended;
    }
}