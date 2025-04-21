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
	grabber.set_value(false);   
	arm.set_value(false);

	//arm.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	//arm.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

	inertial.reset();
	inertial.tare_heading();
	resetDriveEncoders();

	optical.set_led_pwm(50);
	optical.set_integration_time(15);

	rotation.reset_position();
	rotation.set_data_rate(15);
	rotation.set_reversed(true);

	chassis.calibrate();

	/*xPod.reset_position();
	xPod.set_data_rate(15);
	xPod.set_reversed(true);*/

	pros::lcd::set_text(0, "4253S bot is READY! :D");
	
	pros::Task screen_task([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            // delay to save resources
            pros::delay(20);
        }
    });

	//pros::delay(100);
}