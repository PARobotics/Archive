/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"
#include "mobile_goal.h"
#include "base.h"
#include "lift.h"
#include "auton.h"

void operatorControl() {
	TaskHandle driver_control_task = taskCreate(driver_control, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
	TaskHandle lift_control_task = taskCreate(lift_control, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);

	//motorSet(claw_motor, 0);
	while(true) {
		//Flip the mobile goal
		// if(joystickGetDigital(1, 7, JOY_UP)) {
		//  		flip_mobile_goal();
		// 		mg_out();
		// }
		// if(joystickGetDigital(1, 7, JOY_DOWN)) {
		// 	mg_in();
		// }
		if(joystickGetDigital(1, 7, JOY_RIGHT)) {
			down_with_pid();
		}
		// TODO: Remove auton button before competition
		// if(joystickGetDigital(1, 8, JOY_LEFT)) {
		// 	autonomous();
		// }

			if(joystickGetDigital(1, 7, JOY_UP)) {
				motorSet(mobile_goal_lift_1, 127);
			  motorSet(mobile_goal_lift_2, -127);
			} else if(joystickGetDigital(1, 7, JOY_DOWN)) {
				motorSet(mobile_goal_lift_1, -127);
			  motorSet(mobile_goal_lift_2, 127);
			} else {
				motorSet(mobile_goal_lift_2, 0);
				motorSet(mobile_goal_lift_1, 0);
			}
	}

	taskDelete(driver_control_task);
	taskDelete(lift_control_task);
}
