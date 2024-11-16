#include "main.h"

void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::register_btn1_cb(on_center_button);
	pros::lcd::set_text(0, "Initializing...");

	driveLeft.set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
	driveRight.set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
	
	intake.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	intakeBot.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	grabber.set_value(true);   

	inertial.reset();

	pros::lcd::set_text(1, "4253S bot is READY! :D");

	pros::delay(100);
}