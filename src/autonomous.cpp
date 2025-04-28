#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/rtos.hpp"

//ASSET(testblueright_txt);


int delayTask = 190;
int delayIntake = 200;

void colorSort(){
    while(true){
        if(optical.get_proximity() > 150){
            if(isBlue){ // blue alliance
                if(optical.get_hue() < 50){  
                    // delay a time
                    pros::Task::delay(delayTask);

                    // intake reverse
                    toggleIntake(true,-1);

                    // intake fwd again
                    pros::Task::delay(delayIntake);
                    toggleIntake(true,1);
                    
                }
            }else{ // red alliance
                if(optical.get_hue() > 150){
                    // delay a time
                    pros::Task::delay(delayTask);

                    // intake reverse
                    toggleIntake(true,-1);

                    // intake fwd again
                    pros::Task::delay(delayIntake);
                    toggleIntake(true,1);
                }
            }
            pros::Task::delay(100);
        }
    }
}

void blueRight(){ // 4 ring; working
    isBlue = true;
    chassis.setPose(56.5,24,90);

    chassis.moveToPose(28, 24, 90, 4000, {.forwards = false, .minSpeed = 60, .earlyExitRange = 2}, false);

    //chassis.moveToPoint(28, 24, 4000, {.forwards = false, .minSpeed = 70, .earlyExitRange = 2}, false);

    /*chassis.waitUntilDone();
    pros::delay(500);
    grabber.set_value(true);*/
    
    // under ladder
    chassis.moveToPose(16.5, 13, 0, 4000, {.forwards = false, .maxSpeed = 70 , .minSpeed = 30}, false);

    grabber.set_value(true);

    pros::delay(1000);

    toggleIntake(true, 1);
    pros::Task colorSortTask(colorSort, "Color Sort");

    //border
    chassis.moveToPose(11, 44, 0, 4000, {.maxSpeed = 70, .minSpeed = 40,. earlyExitRange = 4}, true); 
    chassis.waitUntilDone();

    pros::delay(1000);

    chassis.moveToPose(11, 55, 0, 4000, {.maxSpeed = 70, .minSpeed = 40, /*.earlyExitRange = 4*/}, true);
    chassis.waitUntilDone();
    pros::delay(500);

    // back
    chassis.moveToPose(18, 23, 0, 4000, {.forwards = false, .minSpeed = 60, .earlyExitRange = 3}, false);

    chassis.moveToPose(24, 46, 0, 4000, {.minSpeed = 50}, false);

    pros::delay(1000);
    // can add alliance stacked ring OR corner ring(s) here


    // -> ladder touch
    chassis.turnToHeading(180, 1500, {.earlyExitRange = 5}, false);

    chassis.moveToPoint(24, 6, 4000, {.maxSpeed = 60, .minSpeed = 30, .earlyExitRange = 12}, false);
    chassis.moveToPoint(24, 6, 4000, {.maxSpeed = 40, .minSpeed = 10}, false);

    toggleIntake(false,0);
    colorSortTask.remove();
}

void blueLeft(){ // unfinished goal rush
    isBlue = true;

    chassis.setPose(52,-37,270);

    //chassis.moveToPose(24, -38, 270, 4000, {.minSpeed = 60, .earlyExitRange = 3}, false);
    //chassis.moveToPose(18, -40, 270, 4000, {.minSpeed = 50}, false);
    chassis.moveToPoint(24, -38, 4000, {.minSpeed = 60, .earlyExitRange = 5}, false);
    chassis.moveToPoint(16, -40, 4000, {}, false);

    chassis.waitUntilDone();

    pros::delay(2000);
    
    // deploy doinker

    //chassis.moveToPose(36, -35, 285, 4000, {.forwards = false, .minSpeed = 90, .earlyExitRange = 2}, false);
    chassis.moveToPoint(24,-35, 4000, {.forwards = false, .minSpeed = 90, .earlyExitRange = 2}, false);

    // undeploy doinker 

    pros::delay(1000);

    chassis.turnToHeading(135, 3000, {.maxSpeed = 70, .earlyExitRange = 5}, false);

    pros::delay(500);
    // goal 1
    //chassis.moveToPose(24, -24, 135, 4000, {.forwards = false, .minSpeed = 40}, false);
    chassis.moveToPoint(24, -24, 4000, {.forwards = false, .minSpeed = 40}, false);

    grabber.set_value(true);

    toggleIntake(true,1);
    pros::Task colorSortTask(colorSort, "Color Sort");

    chassis.moveToPoint(26,-46, 4000, {.maxSpeed = 70, .minSpeed = 40}, false);


    

    //chassis.moveToPoint(24,-49, 4000, {.minSpeed = 50, .earlyExitRange = 2}, false);
    //chassis.moveToPoint(24,-38, 4000, {.forwards = false, .minSpeed = 50, .earlyExitRange = 2}, false);
    chassis.moveToPose(24, -38, 180, 4000, {.forwards = false, .minSpeed = 50, .earlyExitRange = 2}, false);

    pros::delay(700);
    toggleIntake(false,0);
    grabber.set_value(false);    

    chassis.swingToHeading(90, lemlib::DriveSide::LEFT, 4000, {.minSpeed = 30, .earlyExitRange = 5}, false);

    // goal 2
    //chassis.moveToPose(15, -45, 70, 4000, {.forwards = false, .minSpeed = 40}, false);
    chassis.moveToPoint(16, -42, 4000, {.forwards = false, .minSpeed = 40}, false);

    grabber.set_value(true);

    toggleIntake(true,1);

    chassis.moveToPoint(48, -30+12, 4000, {.minSpeed = 40, .earlyExitRange = 5}, false);

    chassis.turnToHeading(315, 4000, {.minSpeed = 20, .earlyExitRange = 5}, false);

    // go ladder

    //chassis.moveToPose()
    //chassis.moveToPose(52, -30+24, 40, 4000, {.minSpeed = 40}, false); 
    /*chassis.moveToPoint(48,-6,4000, {.minSpeed = 40}, false);



    pros::delay(1000);

    chassis.turnToHeading(315, 4000, {.minSpeed = 30, .earlyExitRange = 5}, false);

    chassis.moveToPoint(24, -6, 4000, {.maxSpeed = 50, .minSpeed = 20, .earlyExitRange = 12}, false);
    chassis.moveToPoint(24, -6, 4000, {.maxSpeed = 30, .minSpeed = 20,}, false);
*/

    /*
    // goal 2
    chassis.moveToPose(20, -45, 90, 4000, {.minSpeed = 40}, false);

    grabber.set_value(true);

    toggleIntake(true, 1);

    chassis.moveToPoint(52, -28, 4000, {.minSpeed = 40}, false);

    chassis.turnToHeading(315, 4000, {.minSpeed = 20, .earlyExitRange = 5}, false);

    chassis.moveToPose(24, -6, 0, 4000, {.maxSpeed = 50, .earlyExitRange = 12}, false);

    chassis.moveToPose(24, -6, 0, 4000, {.maxSpeed = 30, .minSpeed = 20, .earlyExitRange = 12}, false);

    colorSortTask.remove();
    toggleIntake(false,0);*/



    /*chassis.moveToPoint(23, -36, 4000, {.forwards = false, .minSpeed = 50, .earlyExitRange = 4}, false);

    chassis.moveToPose(13, -33, 135, 4000, {.forwards = false, .minSpeed = 40, .earlyExitRange = 2}, false);

    grabber.set_value(true);

    toggleIntake(true,1);
    pros::Task colorSortTask(colorSort, "Color Sort");

    chassis.moveToPoint(20, -45, 4000, {.minSpeed = 50, .earlyExitRange = 2}, false);

    pros::delay(500);
    toggleIntake(false, 0);
    grabber.set_value(false);

    chassis.moveToPose(24, -48, 135, 4000, {.minSpeed = 40}, false);

    chassis.turnToHeading(180, 4000, {.minSpeed = 40, .earlyExitRange = 10}, false);

    chassis.moveToPose(24, -30, 180, 4000, {.minSpeed = 40}, false);
    grabber.set_value(true);

    chassis.swingToHeading(90, lemlib::DriveSide::LEFT, 4000, {.minSpeed = 30, .earlyExitRange = 5}, false);

    toggleIntake(true,1);
    chassis.moveToPoint(48, -28, 4000, {.minSpeed = 40}, false);

    pros::delay(500);

    chassis.turnToHeading(0, 4000, {.minSpeed = 40, .earlyExitRange = 10}, false);

    chassis.moveToPose(24, -6, 0, 4000, {.maxSpeed = 50, .earlyExitRange = 12}, false);

    chassis.moveToPose(24, -6, 0, 4000, {.maxSpeed = 30, .minSpeed = 20, .earlyExitRange = 12}, false);

    colorSortTask.remove();
    toggleIntake(false,0);*/
}

void redRight(){ // middle ring path
    isBlue = false;
    
    chassis.setPose(-56.5,-24,270);

    chassis.moveToPose(-28, -24, 270, 4000, {.forwards = false, .minSpeed = 70, .earlyExitRange = 2}, false);

    chassis.waitUntilDone();
    pros::delay(500);
    grabber.set_value(true);

    chassis.swingToHeading(45, lemlib::DriveSide::LEFT, 4000);

    //chassis.moveToPose(-11.75, -9.275, 45, 4000, {.minSpeed = 40}, false);
    chassis.moveToPose(-11, -14, 60, 4000, {.minSpeed = 40}, false);

    toggleDoinker(true);

    chassis.moveToPose(-40, -20, 110, 4000, {.forwards = false, .minSpeed = 60, .earlyExitRange = 2}, false);
    
    toggleDoinker(false);

    toggleIntake(true,1);
    pros::Task colorSortTask(colorSort, "Color Sort");

    chassis.moveToPose(-24, -44, 180, 4000, {.minSpeed = 40}, 4000);

    chassis.turnToHeading(0,1000, {.minSpeed = 40, .earlyExitRange = 15}, false);

    chassis.moveToPose(-24,-4,0,4000,{.minSpeed = 40}, false);

    toggleIntake(false,0);
    colorSortTask.remove();

    /*chassis.swingToHeading(180, lemlib::DriveSide::LEFT, 3000);
    toggleIntake(true,1);
    chassis.moveToPoint(-24, -44, 4000, {.minSpeed = 50}, 4000);

    chassis.turnToHeading(0,1000, {.minSpeed = 40, .earlyExitRange = 15}, false);

    chassis.moveToPose(-24,-4,0,4000,{.minSpeed = 40}, false);*/
}

void redLeft(){ // 4 rings; working
    isBlue = false;

    chassis.setPose(-56.5,24,270);

    chassis.moveToPose(-28, 24, 270, 4000, {.forwards = false, .minSpeed = 60, .earlyExitRange = 2}, false);

    // ladder
    chassis.moveToPose(-16.5, 13, 0, 4000, {.forwards = false, .maxSpeed = 70 , .minSpeed = 40}, false);

    grabber.set_value(true);

    pros::delay(1000);

    toggleIntake(true, 1);
    pros::Task colorSortTask(colorSort, "Color Sort");

    //border
    chassis.moveToPose(-12.5, 44, 0, 4000, {.maxSpeed = 70, .minSpeed = 40,. earlyExitRange = 4}, true); 
    chassis.waitUntilDone();

    pros::delay(1000);

    chassis.moveToPose(-12.5, 53, 0, 4000, {.maxSpeed = 70, .minSpeed = 40, /*.earlyExitRange = 4*/}, true);
    //chassis.moveToPoint(-12,50, 4000, {.maxSpeed = 70, .minSpeed = 40, /*.earlyExitRange = 4*/}, true);
    chassis.waitUntilDone();
    pros::delay(500);

    // back
    chassis.moveToPose(-18, 23, 0, 4000, {.forwards = false, .minSpeed = 60, .earlyExitRange = 3}, false);

    chassis.moveToPose(-24, 46, 0, 4000, {.minSpeed = 50}, false);

    pros::delay(1000);
    // can add alliance stacked ring OR corner ring(s) here


    // -> ladder touch
    chassis.turnToHeading(180, 1500, {.earlyExitRange = 5}, false);

    chassis.moveToPoint(-24, 6, 4000, {.maxSpeed = 60, .minSpeed = 30, .earlyExitRange = 12}, false);
    chassis.moveToPoint(-24, 7, 4000, {.maxSpeed = 20, .minSpeed = 10}, false);

    toggleIntake(false,0);
    colorSortTask.remove();
}

void autonomous() {
    
    //blueRight();
    //redLeft();
    blueLeft();

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

