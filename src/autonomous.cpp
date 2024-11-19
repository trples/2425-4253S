#include "main.h"

void autonomous() {
    /*updateMotors(-50,-50);
    pros::delay(1500);
    updateMotors(0,0);
    toggleIntake(true,1);
    pros::delay(1000);
    toggleIntake(false,0);*/
    

    //translate(100,1);
    //pros::delay(500);

    //rotate(45,-1);
    rotate(270,1);
    pros::delay(2000);

    //inertial.tare_rotation();
    rotate(90,1);
    //translate(720,-1);
    //pros::lcd::set_text(0,"backward done");
    /*toggleIntake(true, 1);

    pros::delay(2000);

    toggleIntake(false,0);*/

    //toggleGrabber(HIGH);

    //pros::delay(1000);

    //toggleGrabber(LOW);
    
    //rotate(90,1);
    //rotate(90,-1);
    //rotate(-90,1);
    //rotate(180,1);
    //translate(720,1);
}