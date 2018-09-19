/*
  This files primary function is to include all the code related to
  the mobile goal.
*/

#include "main.h"
#include "base.h"
#define split_point 770 // The point at which it should go down or up.
#define max_extension 1550
#define min_extension 90
#define kp 0.01
/*
int min(int a, int b) {
    return (a<b)?a:b;
}

int max(int a, int b) {
    return (a>b)?a:b;
}*/
void mg_out() {
  int multiplier = -1;
  // Start motors moving
  motorSet(mobile_goal_lift_1, multiplier*127);
  motorSet(mobile_goal_lift_2, multiplier*-1*127);

  // Wait until they are done
  int pot_val = abs(analogReadCalibrated(MG_POT));
  while (pot_val < max_extension ) { // Going in
     // If potentiometer reads between max and min
     // or multiplier is -1 and the switch is not pushed,
    // then move.
    pot_val = abs(analogReadCalibrated(MG_POT));
    if (joystickGetDigital(1, 7, JOY_LEFT)) { // Bail out
      break;
    }
    delay(100);
  }
  // Stop motors
  motorSet(mobile_goal_lift_1, 0);
  motorSet(mobile_goal_lift_2, 0);
  delay(125);
}
void mg_in() {
  int multiplier = 1;
  // Start motors moving
  motorSet(mobile_goal_lift_1, multiplier*127);
  motorSet(mobile_goal_lift_2, multiplier*-1*127);

  // Wait until they are done
  int pot_val = abs(analogReadCalibrated(MG_POT));
  int switch_val = digitalRead(MG_LIFT_BS);
  while (switch_val || pot_val > min_extension) { // Going in

     // If potentiometer reads between max and min

    //pot_val = abs(analogReadCalibrated(MG_POT));
    switch_val = digitalRead(MG_LIFT_BS);
    if (joystickGetDigital(1, 7, JOY_LEFT)) { // Bail out
      break;
    }
    delay(100);
  }
  // Stop motors
  motorSet(mobile_goal_lift_1, 0);
  motorSet(mobile_goal_lift_2, 0);
  delay(125);
}
int vcmd_stripped(
  int x, int x_target, float kx,
  int v_def, int v_min, int v_max
) {
  int vcmd = v_def - kx * (x - x_target);
  vcmd = min(v_max, max(vcmd, v_min));

  return vcmd;
}
/*
int vcmd_ctrl_xv(
  int x, int v,
  int x_target, int v_target, float kx, float kv,
  int v_def, int v_min, int v_max) {
  // parameters:
  // x:  position from sensor (say angle)
  // v:  velocity from sensor (say rpm)
  // x_target:  target position
  // v_target:  target velocity
  // kx: control parameter
  // kv: control parameter
  // v_def: default vcmd if y is within the band
  // v_min: min value of vcmd
  // v_max: max value of vcmd

  // How it works:
  // this can be used for holding a target at a position -- essentially set v_target=0
  // vcmd = v_def-kx*(x-x_target)-kv*(v-v_target)
  // if you are ahead of x_target; will lower vcmd to pull back
  // if the speed is in the direction of getting ahead, also lower vcmd to pull back

  int vcmd = v_def - kx * (x - x_target) - kv * (v - v_target);
  vcmd = min(v_max, max(vcmd, v_min));

  return vcmd;
}*/

void down_with_pid() {

  // Wait until they are done
  int pot_val = abs(analogReadCalibrated(MG_POT));

  int vcmd = vcmd_stripped(pot_val, max_extension, kp, 100, 127, 50);

  int multiplier = -1;
  // Start motors moving
  motorSet(mobile_goal_lift_1, multiplier*vcmd);
  motorSet(mobile_goal_lift_2, multiplier*-1*vcmd);

  while (pot_val < max_extension ) { // Going in
     // If potentiometer reads between max and min
     // or multiplier is -1 and the switch is not pushed,
    // then move.
    pot_val = abs(analogReadCalibrated(MG_POT));

    vcmd = vcmd_stripped(pot_val, max_extension, kp, 100, 127, 50);
    motorSet(mobile_goal_lift_1, multiplier*vcmd);
    motorSet(mobile_goal_lift_2, multiplier*-1*vcmd);

    if (joystickGetDigital(1, 7, JOY_LEFT)) { // Bail out
      break;
    }
    delay(100);
  }
  // Stop motors
  motorSet(mobile_goal_lift_1, 0);
  motorSet(mobile_goal_lift_2, 0);
  delay(125);
}

void flip_mobile_goal() {
  /*
  If down, moves the mobile goal up. If up, it moves it down.
  */
  // Read mobile goal value
  bool mg_is_out = (abs(analogReadCalibrated(MG_POT)) > split_point);
  printf("IS_OUT: %d AT: %d SPLIT_POINT: %d \n", mg_is_out, abs(analogReadCalibrated(MG_POT)), split_point);
  int multiplier = -1;
  if (mg_is_out) multiplier *= -1;

  // Start motors moving
  motorSet(mobile_goal_lift_1, multiplier*127);
  motorSet(mobile_goal_lift_2, multiplier*-1*127);
  // Wait until they are done
  int pot_val = abs(analogReadCalibrated(MG_POT));
  int switch_val = digitalRead(MG_LIFT_BS);
  while ((!mg_is_out && pot_val < max_extension ) || // Coming out
         (mg_is_out && switch_val && pot_val > min_extension)) { // Going in
     // If potentiometer reads between max and min
     // or multiplier is -1 and the switch is not pushed,
    // then move.
    pot_val = abs(analogReadCalibrated(MG_POT));
    switch_val = digitalRead(MG_LIFT_BS);
    if (joystickGetDigital(1, 7, JOY_LEFT)) { // Bail out
      break;
    }
    delay(100);
  }
  // Stop motors
  motorSet(mobile_goal_lift_1, 0);
  motorSet(mobile_goal_lift_2, 0);
  delay(125);
}
