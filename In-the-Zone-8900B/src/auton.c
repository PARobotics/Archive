/** @file auto.c
 * This file should contain the user autonomous() function and any functions related to it.
 */

#include "main.h"
#include "base.h"
#include "auton.h"
#include "mobile_goal.h"

/*
static void move_full_forward(void);
static void move_full_backward(void);
static void turn_full_right(void);
static void turn_full_left(void);

static void stop_all_motors(void);
*/
void autonomous(void) {
  //Write calls to auton_move(double seconds, direction d)
  // auton_move(REV_TO_MG_1, FORWARD);
  // mg_out();
  // auton_move(REV_TO_MG_2, FORWARD);
  // mg_in();
  // auton_move(REV_TO_TURN_AROUND, LEFT);
  // auton_move(REV_BACK_TO_ZONE, FORWARD);
  // mg_out();
  // auton_move(REV_MG_REV_RELEASE, BACKWARD);
  // mg_in();
  // auton_move(REV_MOVE_AWAY, BACKWARD);

  /*
  auton_move([double seconds], FORWARD);
  auton_move([double seconds], BACKWARD);
  auton_move([double seconds], RIGHT);
  auton_move([double seconds], LEFT);
  */
}

void check_encoder() {

  printf("HERE!");
  int counts;
  imeGet(0, &counts);

  printf("! %d", counts);
  delay(20);
}

void auton_move(double revolutions, direction d) {
  printf("MOVING\n");
  delay(20);
  int counts = 0, terminate = 0;
  // motorSet(0, 0);
  switch (d) {
    case FORWARD: move_full_forward(); break;
    case BACKWARD: move_full_backward(); break;
    case RIGHT: turn_full_right(); break;
    case LEFT: turn_full_left(); break;
    default:
      /* TODO: Report a horrible error; no valid direction provided */
      return;
  }
  while (!terminate) {
    imeGet(0, &counts);
    printf("counts: %d\n", counts);
    delay(20);
    if ((double)counts >= (revolutions * TICKS_PER_REVOLUTION))
      terminate = 1;
  }

  // imeReset(0);

  stop_all_motors();
}

void move_full_forward(void) {
  printf("WHEELS SET!\n");
  delay(20);
  motorSet(wheel_R2, -127);
  motorSet(wheel_L2, 127);
  motorSet(wheel_L1, 127);
  motorSet(wheel_R1, -127);
}

void move_full_backward(void) {
  motorSet(wheel_R2, 127);
  motorSet(wheel_L2, -127);
  motorSet(wheel_L1, -127);
  motorSet(wheel_R1, 127);
}

void turn_full_right(void) {
  motorSet(wheel_R2, 127);
  motorSet(wheel_L2, 127);
  motorSet(wheel_L1, 127);
  motorSet(wheel_R1, 127);
}

void turn_full_left(void) {
  motorSet(wheel_R2, -127);
  motorSet(wheel_L2, -127);
  motorSet(wheel_L1, -127);
  motorSet(wheel_R1, -127);
}

void stop_all_motors(void) {
  motorSet(wheel_R2, 0);
  motorSet(wheel_L2, 0);
  motorSet(wheel_L1, 0);
  motorSet(wheel_R1, 0);
}
