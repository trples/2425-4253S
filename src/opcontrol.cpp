#include "main.h"
#include "pros/rtos.hpp"

int delaySort = 190;
int delayForward = 200;

void driverColorSort(){
    while(true){
        if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)&&controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
			pros::lcd::set_text(6, "helo");
			if(optical.get_proximity() > 150){
				pros::lcd::set_text(5, "sort");
				if(isBlue){ // blue alliance
					if(optical.get_hue() < 30){  
						// delay a time
						pros::Task::delay(delaySort);

						// intake reverse
						toggleIntake(true,-1);

						// intake fwd again
						pros::Task::delay(delayForward);
						toggleIntake(true,1);
						
					}
				}else{ // red alliance
					if(optical.get_hue() > 50){
						// delay a time
						pros::Task::delay(delaySort);

						// intake reverse
						toggleIntake(true,-1);

						// intake fwd again
						pros::Task::delay(delayForward);
						toggleIntake(true,1);
					}
				}
				pros::Task::delay(200);
        	}else{
				toggleIntake(true,1);
				pros::lcd::set_text(5, "nooo sort");
			}
		}
		pros::delay(15);
    }
}

void armControl(){
	
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	isBlue = true;
	pros::Task driverColorSortTask(driverColorSort, "Driver Color Sort");
	chassis.setPose(56.5,24,90);
	while (true) {
		// loop for taking in driver controls
		setDrive();
		setIntake();
		setGrabber();
		setArm();


		//pros::lcd::set_text(1, std::to_string(optical.get_hue()));
		//pros::lcd::set_text(4, std::to_string(optical.get_proximity()));



		pros::lcd::set_text(1,std::to_string(driveLeftTop.get_position()));
		pros::lcd::set_text(2,std::to_string(driveLeftBot.get_position()));
		pros::lcd::set_text(3,std::to_string(driveLeftBack.get_position()));
		pros::lcd::set_text(4,std::to_string(driveRightTop.get_position()));
		pros::lcd::set_text(5,std::to_string(driveRightBot.get_position()));
		pros::lcd::set_text(6,std::to_string(driveRightBack.get_position()));
		
		//pros::lcd::set_text(4,std::to_string(optical.get_proximity()));
		//pros::lcd::set_text(5,std::to_string(optical.get_hue()));

		//pros::lcd::set_text(3, "heading" + std::to_string(inertial.get_heading()));
		//pros::lcd::set_text(3,std::to_string(rotation.get_position()/100));
		//pros::lcd::set_text(4,std::to_string(xPod.get_position()/100));
		//pros::lcd::set_text(5,std::to_string(arm.get_position()));
		//pros::lcd::set_text(1,std::to_string(inertial.get_rotation()));
		//pros::lcd::set_text(2,std::to_string(getAverageEncoderVal()));
		pros::delay(10);
	}
}