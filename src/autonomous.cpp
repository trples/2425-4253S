#include "main.h"

void autonomous() {
    //translate(100,1);
    //translate(100,-1);
    updateMotors(-50,-50);
    pros::delay(1500);
    updateMotors(0,0);
    toggleIntake(true,1);
    pros::delay(1000);
    toggleIntake(false,0);
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