/*
    Control the lift
 */

#include "main.h"
#include "second_elbow_pid.h"

#define ELBOW_HOLDING_SPEED 20
#define LIFT_HOLDING_SPEED 10

float lift_height; // The most recently updated location of the lift.
TaskHandle magic_stack;

float get_height() {
  // Heighest value: 2967 = 35.5
  // Lowest value: 1329 = 4.5in
  // 4.5 = m*1329 + b
  // 35.5 = m*2967 + b
  // b=-20.65201,\:m=0.01892552
  float m = 0.01892552;
  float b = -20.65201;
  return m*analogRead(lift_potentiometer) + b;
}
void stack_cone() {
  int distance = ultrasonicGet(sonar);
  printf("DISTANCE %d \n", distance);
  // while (distance != -1) { // clear path = -1, raise the lift
  //   motorSet(lift_motor_L, 127);
  //   motorSet(lift_motor_R, 127);
  // }
  // motorSet(lift_motor_L, 0);
  // motorSet(lift_motor_R, 0);

  // retract the arm
  // TODO: insert PID movement function
  moveSecondElbow(127);
  delay(40);
  moveSecondElbow(ELBOW_HOLDING_SPEED);

  // drop cone, open claw
  motorSet(claw_motor, 127);
  delay(10);
  motorSet(claw_motor, 0);


}
void lift_control() {
  int elbow_holding_voltage = ELBOW_HOLDING_SPEED;
  int lift_holding_voltage = LIFT_HOLDING_SPEED;
  // secondElbowSetPos(0);
    while (true) {
      lift_height = get_height();
        // Lift
        if (joystickGetDigital(1, 6, JOY_DOWN)) { // down
            printf("%f \n", lift_height);
            motorSet(lift_motor_L, 127);
            motorSet(lift_motor_R, -127);
        } else if (joystickGetDigital(1, 6, JOY_UP)) { // up
            motorSet(lift_motor_L, -127);
            motorSet(lift_motor_R, 127);
            printf("%f \n", lift_height);
        } else { // stationary
            //motorSet(lift_motor_L, lift_holding_voltage);
            //motorSet(lift_motor_R, lift_holding_voltage);
            motorSet(lift_motor_L, lift_holding_voltage);
            motorSet(lift_motor_R, lift_holding_voltage);

        }
        // Claw
        if (joystickGetDigital(1, 5, JOY_UP)) { // up
            motorSet(claw_motor, -127);
        } else if (joystickGetDigital(1, 5, JOY_DOWN)) { // down
            motorSet(claw_motor, 127);
        } else { // stationary
            motorSet(claw_motor, 20);
        }

        // Second ELbow
        if (joystickGetDigital(1, 8, JOY_UP)) { // up
          // stopHoldElbowPos();
          moveSecondElbow(-127);
          elbow_holding_voltage = -1*ELBOW_HOLDING_SPEED;
        } else if (joystickGetDigital(1, 8, JOY_DOWN)) { // down
          // stopHoldElbowPos();
          moveSecondElbow(127);
          elbow_holding_voltage = ELBOW_HOLDING_SPEED/2;
        } else { // stationary
          moveSecondElbow(elbow_holding_voltage);
          // holdElbowPos();
        }

        if (joystickGetDigital(1, 8, JOY_UP)) { // up
          // stopHoldElbowPos();
          if (magic_stack == NULL || taskGetState(magic_stack) != TASK_RUNNING)
            magic_stack = taskCreate(stack_cone, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
        }
        delay(25);
    }
}
