#include "main.h"

void autonomous() {
    // basic auton, go back and score
    /*updateMotors(-50,-50);
    pros::delay(1500);
    updateMotors(0,0);
    toggleIntake(true,1);
    pros::delay(1000);
    toggleIntake(false,0);*/


    // red right side
    /*translate(36,-1); // back (into goal)
    pros::delay(500);
    updateMotors(-50,-50);
    pros::delay(500);
    //switchGrabber(); // grab goal
    grabber.set_value(HIGH);
    pros::delay(300);
    updateMotors(0,0);
    pros::delay(50);
    translate(14,1);
    pros::delay(100);
    translate(4,1);
    pros::delay(300);
    translate(4,-1);
    pros::delay(400);
    toggleIntake(true,1);
    pros::delay(700);
    toggleIntake(true,-1);
    rotate(280,1);

    pros::delay(100);
    toggleIntake(false,1);
    translate(12,1);
    updateMotors(50,50);
    toggleIntake(true,1);

    pros::delay(370);
    updateMotors(0,0);
    pros::delay(500);
    rotate(180,1);
    pros::delay(400);

    translate(36,1);
    
    toggleIntake(true,-1);
    
    pros::delay(100);
    rotate(10,1);
    pros::delay(200);
    updateMotors(30,30);
    toggleIntake(false,0); //*/
    //pros::delay(1500);
    //updateMotors(0,0);//



    // blue left
    translate(36,-1); // back (into goal)
    pros::delay(500);
    updateMotors(-50,-50);
    pros::delay(500);
    //switchGrabber(); // grab goal
    grabber.set_value(HIGH);
    pros::delay(300);
    updateMotors(0,0);
    pros::delay(50);
    translate(14,1);
    pros::delay(100);
    translate(4,1);
    pros::delay(300);
    translate(4,-1);
    pros::delay(400);
    toggleIntake(true,1);
    pros::delay(700);
    toggleIntake(true,-1);
    rotate(80,1);

    pros::delay(100);
    toggleIntake(false,0);
    translate(12,1);
    updateMotors(50,50);
    toggleIntake(true,1);

    pros::delay(370);
    updateMotors(0,0);
    pros::delay(500);
    rotate(180,1);
    pros::delay(400);

    translate(36,1);
    
    toggleIntake(true,-1);
    
    pros::delay(200);
    updateMotors(30,30);
    pros::delay(500);
    toggleIntake(false,0); 
    pros::delay(1000);
    updateMotors(0,0);
    //*/




    // blue right side
    /*translate(36,-1); // back (into goal)
    pros::delay(500);
    updateMotors(-70,-70);
    pros::delay(300);
    //switchGrabber(); // grab goal
    grabber.set_value(HIGH);
    pros::delay(400);
    updateMotors(0,0);
    pros::delay(50);
    translate(14,1);
    pros::delay(100);
    translate(4,1);
    pros::delay(300);
    translate(4,-1);
    pros::delay(400);
    toggleIntake(true,1);
    pros::delay(700);
    toggleIntake(true,-1);
    
    rotate(100,1);
    toggleIntake(false,0);

    translate(36,1);
    updateMotors(30,30);
    pros::delay(500); 
    
    pros::delay(1000);
    updateMotors(0,0);//*/

    
}