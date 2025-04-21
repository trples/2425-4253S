#include "main.h"
#include "pros/rtos.hpp"

ASSET(testblueright_txt);

bool isBlue;

void colorSort(){
    if(optical.get_proximity() > 150){
        if(isBlue){ // blue alliance
            if(optical.get_hue() < 30){
                toggleWeak(1);
                
                // delay a time

                // intake reverse

                // intake fwd again
                
            }
        }else{ // red alliance
            if(optical.get_hue() > 50){
                toggleWeak(1);
                
            }
        }
    }

    
    
}

void blueRight(){
    //lemlib_tarball::Decoder decoder(testblueright_txt);
    isBlue = true;

    chassis.setPose(56.5,24,90);

    chassis.moveToPose(32, 24, 90, 4000, {.forwards = false, .maxSpeed = 100, .earlyExitRange = 2}, false);

    chassis.waitUntilDone();

    chassis.moveToPose(15, 13, 0, 4000, {.forwards = false}, false);

    toggleIntake(true, 1);
    pros::Task colorSortTask(colorSort, "Color Sort");

    chassis.moveToPose(20.4, 60.4, 110, 4000, {.forwards = true}, false);

    //chassis.follow(testblueright_txt, 12, 15000,true);
}

void autonomous() {
    //test();
    //blueRight();
    
    pros::Task blueRightAuton(blueRight, "Blue Right Auto");

    //skills();
    
    //redLeftAWP(); // switch autos
    //redLeftAB();

    //redRightAWP();
    //redRightAB(); // **********************************
    
    //blueRightAWP(); // switch autos
    //blueRightAB();

    //blueLeftAWP();
    //blueLeftAB(); // ***********************************
}

