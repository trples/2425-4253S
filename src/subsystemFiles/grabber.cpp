#include "main.h"
#include "subsystemHeaders/global.hpp"

bool extended = true;

void toggleGrabber(){
    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)){
        pros::lcd::set_text(0, "x pressed");
        if(extended){
            grabber.set_value(HIGH);
            pros::lcd::set_text(4, "changed the thing pressed");
        }else{
            grabber.set_value(LOW);
        }
        extended = !extended;
        pros::lcd::set_text(5, "changed bool");
    }
    else{
        pros::lcd::set_text(0, "x NOT pressed");
    }
}