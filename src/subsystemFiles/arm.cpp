#include "main.h"
#include "subsystemHeaders/global.hpp"

bool armIsUp = false;
bool armIsDown = true;
bool override = false;

void oldSetArm(){
    int power = 100;
    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)){
        override = !override;
    }

    if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
        arm.move(-1*power);
        pros::lcd::set_text(6,"motor reversed");
    }else if(((rotation.get_position()/100 > 30)||override)&&controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
        arm.move(1*power);
        pros::lcd::set_text(6,"motor forwarded");
    }else{
        arm.brake();
    }
}

void setArm(){
    int power = 100;
    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)){ // up
        if(!armIsUp&&!armIsDown){ // arm @ neutral
            armUp();
        }else if(armIsDown&&!armIsUp){ // arm @ down
            armNeutral();
        }/*else{
            armUp();
        }*/
    }else if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)&&(rotation.get_position()/100 < 25)){ // down
        if(!armIsUp&&!armIsDown){ // arm @ neutral
            armDown();
        }else if(!armIsDown&&armIsUp){ // arm @ up
            armNeutral();
        }/*else{
            armDown();
        }*/
    }else{
        arm.brake();
    }

    pros::lcd::set_text(1,std::to_string(armIsUp));
    pros::lcd::set_text(2,std::to_string(armIsDown));

}

/* 0 = starting
75 = recieve
140 = neutral (not obstructing)
230 = up
300 = on goal
*/

static double kP = 4;
static double kD = 5;

void oldArmPID(double deg){
    int direction;
    if(deg < fabs(arm.get_position())){ // degree < current position = go down
        direction = -1; // reverse
    }else{
        direction = 1; // forward
    }

    while(fabs(arm.get_position()) > deg+2 || fabs(arm.get_position()) < deg-2){
        static double error = 0;
        static double prevError = 0;

        prevError = error;
        error = fabs(deg - arm.get_position()); 

        double power = error*kP + (prevError-error)*kD;

        std::string before = std::to_string(power);
        pros::lcd::set_text(0, before);

        if (power > 127) power = 127;
        //if (power > 60) power = 60;

        std::string after = std::to_string(power);
        pros::lcd::set_text(1, after);

        arm.move(power*direction);

        pros::lcd::set_text(4, std::to_string(arm.get_position()));
    }
    arm.brake();
}

void armPID(double deg, int direction){
    //int direction;
    //if(deg<0) deg = deg + 180;

    /*if(deg < rotation.get_position()/100.0){ // degree < current position = go down
        direction = 1; // reverse
        pros::lcd::set_text(6,"motor reversed");
    }else{
        direction = -1; // forward
        pros::lcd::set_text(6,"motor forwarded");
    }*/

    double error = fabs(rotation.get_position()/100.0 - deg);
    double prevError = 0;
    int timeElapsed = 0;
    while(error > 5){
        prevError = error;
        error = fabs(fabs(deg) - fabs(rotation.get_position()/100.0)); 

        double power = error*kP; //+ (prevError-error)*kD;

        /*std::string before = std::to_string(power);
        pros::lcd::set_text(0, before);*/

        if (power > 127) power = 127;
        //if (power > 60) power = 60;

        /*std::string after = std::to_string(power);
        pros::lcd::set_text(1, after);*/
        pros::lcd::set_text(7,std::to_string(error));

        arm.move(power*direction);

        pros::lcd::set_text(4, std::to_string(rotation.get_position()/100));
        timeElapsed+=15;
        if(timeElapsed > 700){
            break;
        }
        pros::delay(15);
    }
    arm.move(50*direction*-1);
    pros::delay(100);
    arm.brake();
}

void armUp(){
    //armPID(230);
    armPID(150,1);
    armIsUp = true;
    armIsDown = false;
}

void armNeutral(){
    //armPID(140);
    armPID(80,0);
    armIsUp = false;
    armIsDown = false;
}

void armDown(){
    //armPID(75);
    armPID(30,-1);
    armIsUp = false;
    armIsDown = true;
}