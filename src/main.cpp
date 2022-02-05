#include "main.h"
#include "functions.hpp"


int stickMultiplier = 1;
bool intakeToggle = false;
bool frontClipToggle = true;
bool backClipToggle = false;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

void initialize() {
	//Initialize PROS LCD
	initSelector();

	//Set lift to hold so the mobile goal doesn't drop after the button is let go
	lift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

	//Calibrates and initializes sensors
	rightEncoder.set_reversed(true);
	ResetSensors(true);

	//Initialize pneumatics
	frontClip.set_value(true);
	frontClip.set_value(false);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	//Execute the autonomous program previously set by the auton selector
	getSelection()();
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
	while(true) {
		//Updates the on-screen buttons
		updateSelector();

		//Button to invert sticks
		if(master.get_digital_new_press(DIGITAL_A))
			stickMultiplier *= -1;

		//Button to toggle ringle intake
		if(master.get_digital_new_press(DIGITAL_B))
			intakeToggle = !intakeToggle;

		//Button to toggle the front pneumatic clip
		if(master.get_digital_new_press(DIGITAL_R1))
			frontClipToggle = !frontClipToggle;

		//Button to toggle the back pneumatic clip
		if(master.get_digital_new_press(DIGITAL_R2))
			backClipToggle = !backClipToggle;


		//Get the values of the y-axes of the left and right sticks, and store them in left and right respectively
		int left = master.get_analog(ANALOG_LEFT_Y) * stickMultiplier;
		int right = master.get_analog(ANALOG_RIGHT_Y) * stickMultiplier;

		//Calculate the movement of the lift based on the L1 and L2 buttons
		int liftValue = (master.get_digital(DIGITAL_L1) - master.get_digital(DIGITAL_L2));

		//Set the power of the drivetrain motors based on the controller sticks
		if(stickMultiplier + 1)
			drive_op(left, right);
		else
			drive_op(right, left);

		//Set the power of the mobile goal lifts motor based on the value calculated above
		lift.move_velocity(liftValue * 200);

		//Set the power of the ringle intake based on value calculated above
		intake.move_velocity(600 * intakeToggle);

		//Sets the state of the pneumatic clips based on the values calculated above
		frontClip.set_value(frontClipToggle);
		backClip.set_value(backClipToggle);


		pros::delay(20);
	}
}
