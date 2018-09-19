#include "main.h"
#include "fbc.h"
#include "fbc_pid.h"
#include "second_elbow_pid.h"

/* SECOND ELBOW PID STUFF */
fbc_t secondElbowController;
fbc_pid_t secondElbowPID;


TaskHandle holding_task;

void secondElbowSetPos(int position) {
  fbcSetGoal(&secondElbowController, position);
}

void stopHoldElbowPos() {
  if (holding_task != NULL && taskGetState(holding_task) != TASK_SUSPENDED) {
    taskSuspend(holding_task);
  }
}

void moveSecondElbow(int power) {
  motorSet(second_elbow, power);
}
//
static int _secondElbowSense() {
  return analogReadCalibrated(second_elbow_potentiometer);
}

static void _secondElbowReset() {
  // If using a sensor like an encoder that needs reset, do that here.
}

void secondElbowInit() {
  fbcInit(&secondElbowController, moveSecondElbow, _secondElbowSense, _secondElbowReset,
          fbcStallDetect, -EXAMPLE_DB, EXAMPLE_DB, EXAMPLE_TOLERANCE, EXAMPLE_CONFIDENCE);
  fbcPIDInitializeData(&secondElbowPID, EXAMPLE_KP, EXAMPLE_KI, EXAMPLE_KD, -EXAMPLE_INTEG_CAP, EXAMPLE_INTEG_CAP);
  fbcPIDInit(&secondElbowController, &secondElbowPID);
  holding_task = fbcRunParallel(&secondElbowController);
  taskSuspend(holding_task);
}

void holdElbowPos() {
  if (holding_task == NULL || taskGetState(holding_task) != TASK_RUNNING) {
    taskResume(holding_task);
  }
  delay(100);
}
