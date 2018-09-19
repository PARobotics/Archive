/** @file init.c
 * This file should contain the user initialize() function and any functions related to it.
 */

#include "main.h"
#include "auton.h"
#include "second_elbow_pid.h"

void initializeIO() {
  sonar = ultrasonicInit(ultrasonic_out, ultrasonic_in);

  pinMode(MG_LIFT_BS, INPUT);

  analogCalibrate(MG_POT);
  analogCalibrate(second_elbow_potentiometer);
}

void initialize() {

  //Calibrate the sensors
  secondElbowInit();
  /*
  int timeout = 1.5; // Timeout of mg movement is 1.5 seconds
  int count = 0; // Number of seconds
  while (digitalRead(MG_LIFT_BS)) { // Bring mobile goal in
    motorSet(mobile_goal_lift_1, 127);
    motorSet(mobile_goal_lift_2, -1*127);
    count += 5.0/1000;
    if (count > timeout) {
      // printf('INITIAL MOBILE GOAL MOVEMENT RETRACTION FAILED!');
      break;
    }
    delay(5);
  }*/
  motorSet(mobile_goal_lift_1, 0);
  motorSet(mobile_goal_lift_2, 0);

  // Check count to ensure all IMEs are plugged in!
  int IMECount = imeInitializeAll();
  if(IMECount != NUMBER_OF_IME){
    printf("Cannot find integrated motor encoder!\n");
  }

}
