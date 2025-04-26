#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/rtos.hpp"

//ASSET(testblueright_txt);


int delayTask = 180;
int delayIntake = 200;

void colorSort(){
    while(true){
        if(optical.get_proximity() > 150){
            if(isBlue){ // blue alliance
                if(optical.get_hue() < 30){  
                    // delay a time
                    pros::Task::delay(delayTask);

                    // intake reverse
                    toggleIntake(true,-1);

                    // intake fwd again
                    pros::Task::delay(delayIntake);
                    toggleIntake(true,1);
                    
                }
            }else{ // red alliance
                if(optical.get_hue() > 50){
                    // delay a time
                    pros::Task::delay(delayTask);

                    // intake reverse
                    toggleIntake(true,-1);

                    // intake fwd again
                    pros::Task::delay(delayIntake);
                    toggleIntake(true,1);
                }
            }
        }
        pros::Task::delay(200);
    }
}

void blueRight(){
    isBlue = true;

    chassis.setPose(56.5,24,90);

    chassis.moveToPose(28, 24, 90, 4000, {.forwards = false, .minSpeed = 70, .earlyExitRange = 2}, false);

    chassis.waitUntilDone();
    pros::delay(500);
    grabber.set_value(true);
    
    // under ladder
    chassis.moveToPose(15, 13, 0, 4000, {.forwards = false, .maxSpeed = 70, .minSpeed = 50}, false);

    toggleIntake(true, 1);
    pros::Task colorSortTask(colorSort, "Color Sort");

    //border
    chassis.moveToPose(10, 45, 0, 4000, {.maxSpeed = 80, .minSpeed = 50,. earlyExitRange = 4}, false);

    chassis.moveToPose(11, 60, 10, 4000, {.maxSpeed = 60, .minSpeed = 50,. earlyExitRange = 4}, false);
    

    //chassis.moveToPose(11.3, 58, 45, 4000, {.minSpeed = 50, .earlyExitRange = 4}, false);
    //chassis.moveToPoint(11.3, 58, 4000, {.minSpeed = 50, .earlyExitRange = 4}, false);

    chassis.moveToPoint(24, 55, 4000, {.maxSpeed = 100, .minSpeed = 50, .earlyExitRange = 6}, false);

    //chassis.moveToPose(20.4, 60.4, 110, 4000,{.earlyExitRange = 3}, false);
    //chassis.turnToPoint(24, 9, 4000);
    
    chassis.turnToHeading(165, 1000, {.minSpeed = 40, .earlyExitRange = 15}, false);

    chassis.moveToPose(26, 49, 180, 4000, {.maxSpeed = 100, .minSpeed = 50, .earlyExitRange = 3}, false);

    chassis.moveToPose(24, 4, 180, 4000, {.minSpeed = 40},false);

    colorSortTask.remove();
}

void otherBlueRight(){
    isBlue = true;
    chassis.setPose(56.5,24,90);

    chassis.moveToPose(28, 24, 90, 4000, {.forwards = false, .minSpeed = 70, .earlyExitRange = 2}, false);

    chassis.waitUntilDone();
    pros::delay(500);
    grabber.set_value(true);
    
    // under ladder
    chassis.moveToPose(16.5, 13, 0, 4000, {.forwards = false, .maxSpeed = 70, .minSpeed = 50}, false);

    pros::delay(1000);

    toggleIntake(true, 1);
    pros::Task colorSortTask(colorSort, "Color Sort");

    //border
    chassis.moveToPose(9, 44, 0, 4000, {.maxSpeed = 80, .minSpeed = 50,. earlyExitRange = 4}, false);

    chassis.moveToPose(9, 49, 0, 4000, {.maxSpeed = 80, .minSpeed = 50,. earlyExitRange = 4}, false);
    
    pros::delay(1000);

    // back
    chassis.moveToPose(17, 23, 0, 4000, {.forwards = false, .minSpeed = 60, .earlyExitRange = 3}, false);

    chassis.moveToPose(22, 42, 30, 4000, {.minSpeed = 60}, false);

    // can add alliance stacked ring OR corner ring(s) here


    // -> ladder touch
    chassis.turnToHeading(180, 4000, {.minSpeed = 40, .earlyExitRange = 10}, false);

    chassis.moveToPoint(24, 4, 4000, {.minSpeed = 50}, false);

    colorSortTask.remove();
}

void blueLeft(){
    isBlue = true;

    chassis.setPose(56.5,-24,90);

    chassis.moveToPose(28, -24, 270, 4000, {.forwards = false, .minSpeed = 70, .earlyExitRange = 2}, false);

    chassis.waitUntilDone();
    pros::delay(500);
    grabber.set_value(true);

    chassis.swingToHeading(180, lemlib::DriveSide::RIGHT, 3000);
    toggleIntake(true,1);
    chassis.moveToPoint(24, -44, 4000, {.minSpeed = 50}, 4000);

    chassis.turnToHeading(0,1000, {.minSpeed = 40, .earlyExitRange = 15}, false);

    chassis.moveToPose(24,-4,0,4000,{.minSpeed = 40}, false);
}

void redRight(){
    isBlue = false;
    
    chassis.setPose(-56.5,-24,270);

    chassis.moveToPose(-28, -24, 270, 4000, {.forwards = false, .minSpeed = 70, .earlyExitRange = 2}, false);

    chassis.waitUntilDone();
    pros::delay(500);
    grabber.set_value(true);

    chassis.swingToHeading(180, lemlib::DriveSide::LEFT, 3000);
    toggleIntake(true,1);
    chassis.moveToPoint(-24, -44, 4000, {.minSpeed = 50}, 4000);

    chassis.turnToHeading(0,1000, {.minSpeed = 40, .earlyExitRange = 15}, false);

    chassis.moveToPose(-24,-4,0,4000,{.minSpeed = 40}, false);
}

void redLeft(){
    isBlue = false;

    chassis.setPose(-56.5,24,270);

    chassis.moveToPose(-28, 24, 270, 4000, {.forwards = false, .minSpeed = 70, .earlyExitRange = 2}, false);

    chassis.waitUntilDone();
    pros::delay(500);
    grabber.set_value(true);
    
    // under ladder
    chassis.moveToPose(-15, 13, 0, 4000, {.forwards = false, .maxSpeed = 70, .minSpeed = 50}, false);

    toggleIntake(true, 1);
    pros::Task colorSortTask(colorSort, "Color Sort");

    //border
    chassis.moveToPose(-10, 45, 0, 4000, {.maxSpeed = 80, .minSpeed = 50,. earlyExitRange = 4}, false);

    chassis.moveToPose(-11, 60, 350, 4000, {.maxSpeed = 60, .minSpeed = 50,. earlyExitRange = 4}, false);
    
    chassis.moveToPoint(-24, 55, 4000, {.maxSpeed = 100, .minSpeed = 50, .earlyExitRange = 6}, false);

    chassis.turnToHeading(195, 1000, {.minSpeed = 40, .earlyExitRange = 15}, false);

    chassis.moveToPose(-26, 49, 180, 4000, {.maxSpeed = 100, .minSpeed = 50, .earlyExitRange = 3}, false);

    chassis.moveToPose(-24, 4, 180, 4000, {.minSpeed = 40},false);

    colorSortTask.remove();
}

void autonomous() {
    
    otherBlueRight();
   
    //blueRight();
    /*grabber.set_value(true);
    toggleIntake(true,1);
    pros::Task colorSortTask(colorSort,  "Color Sort");


    pros::delay(14*1000);*/


    //pros::Task blueRightAuton(blueRight, "Blue Right Auto");

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

