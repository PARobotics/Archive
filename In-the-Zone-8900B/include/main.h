/** @file main.h
 *
 * This file is included by default in the predefined stubs in each VEX Cortex PROS Project.
 */
// Define motors! (Oritentation: have the mobile goal lifter closest to you.)
//
//            Front
//          \\____//
//        |----------|
//        |          |
//  Left  |          | Right
//        |          |
//        |__|____|__|
//            Back

#define MG_POT 1 // Mobile goal potentiometer
#define CLAW_POT 2
#define MG_LIFT_BS 2 // Mobile Goal lift button sensor
#define lift_potentiometer 2 // Lift angle potentiometer
#define second_elbow_potentiometer 3
#define ultrasonic_in 3
#define ultrasonic_out 3


#define mobile_goal_lift_1 1
#define mobile_goal_lift_2 10
/*
#define wheel_L2 8
#define wheel_L1 9
#define wheel_R1 6
#define wheel_R2 7
*/
#define wheel_R2 8
#define wheel_R1 9
#define wheel_L1 6
#define wheel_L2 7

#define lift_motor_R 5
#define lift_motor_L 4
// #define claw_motor 1
#define second_elbow 2
#define claw_motor 3

#define MOTOR_ENCODER wheel_L2
#define NUMBER_OF_IME 1
#define TICKS_PER_REVOLUTION 392.0f

void exampleInit();
void exampleSetPos(int position);
void exampleMoveToPos();

// #pragma config(Sensor, in1,    POT_SENSOR,     sensorPotentiometer)
// #pragma config(Sensor, dgtl2,  LIM_SWITCH,     sensorTouch)


#ifndef MAIN_H_
#define MAIN_H_

#include <API.h>
Ultrasonic sonar;
Encoder motor_encoder;

// Allow usage of this file in C++ programs
#ifdef __cplusplus
extern "C" {
#endif

// Definition of Global functions!
void autonomous();
void initializeIO();
void initialize();
void operatorControl();

#ifdef __cplusplus
}
#endif

#endif
