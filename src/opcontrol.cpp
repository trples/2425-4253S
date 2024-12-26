#include "main.h"

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
	while (true) {
		// loop for taking in driver controls
		setDrive();
		setIntake();
		setGrabber();
		oldSetArm();

		/*pros::lcd::set_text(1,std::to_string(driveLeftFront.get_position()));
		pros::lcd::set_text(2,std::to_string(driveLeftMid.get_position()));
		pros::lcd::set_text(3,std::to_string(driveLeftBack.get_position()));
		pros::lcd::set_text(4,std::to_string(driveRightFront.get_position()));
		pros::lcd::set_text(5,std::to_string(driveRightMid.get_position()));
		pros::lcd::set_text(6,std::to_string(driveRightBack.get_position()));*/
		
		pros::lcd::set_text(3, "heading" + std::to_string(inertial.get_heading()));
		//pros::lcd::set_text(3,std::to_string(rotation.get_position()/100));
		//pros::lcd::set_text(4,std::to_string(xPod.get_position()/100));
		//pros::lcd::set_text(5,std::to_string(arm.get_position()));
		//pros::lcd::set_text(1,std::to_string(inertial.get_rotation()));
		//pros::lcd::set_text(2,std::to_string(getAverageEncoderVal()));
		pros::delay(10);
	}
}