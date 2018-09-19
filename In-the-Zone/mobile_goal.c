#ifndef MOBILE_GOAL_C
#define MOBILE_GOAL_C

#define MOBILE_GOAL_BOTTOM_LIMIT  1550
#define MOBILE_GOAL_DEFAULT_V 15
#define MOBILE_GOAL_MIN_V -127
#define MOBILE_GOAL_MAX_V 127
#define MOBILE_GOAL_KP 0.08
#define MOBILE_GOAL_KD 0.03


/*
  MOBILE_GOAL.C
  Contains all the code for the mobile goal lift.
*/

void moveMobileGoalLift(int status){ //Manually sets value for mobile goal lift
  if(status == UP) moveMobileGoalLift(127);
  else if(status == DOWN) moveMobileGoalLift(-127);
  else motorReq[M_MOBILE_GOAL] = status;
}

bool mobileGoalIsInPlace(){ //Returns if the mobile goal has been loaded onto the robot.
  return SensorValue(mobileGoalLimitSwitch) == 1;
}

task mobileGoalTask(){
	mobileGoalPID.kp = MOBILE_GOAL_KP;
	mobileGoalPID.kd = MOBILE_GOAL_KD;
	initializeSensor(&mobileGoalLift, 1.0, in2, &mobileGoalPID);
	int mobileGoalAppliedVoltage = 0;
  int t0;

  while (true) {
    if(MOBILE_GOAL_COMMAND == UP){ //Automatically raises mobile goal into place
      t0 = time1[T1];
      moveMobileGoalLift(UP);
      while(!isTimedOut(t0 + 2000) && mobileGoalIsInPlace() == 0 && vexRT[BAILOUT_BUTTON] == 0){
      	updateSensor(&mobileGoalLift);
     		if(mobileGoalLift.val > 2900) moveMobileGoalLift(80);
     		else moveMobileGoalLift(UP);
        wait1Msec(10);
      }
      moveMobileGoalLift(STOP);
      MOBILE_GOAL_COMMAND = STOP;
    }
    else if(MOBILE_GOAL_COMMAND == DOWN){ //Automatically lowers mobile goal stack into place
      t0 = time1[T1];
      moveMobileGoalLift(DOWN);
      updateSensor(&mobileGoalLift);

	      while(!isTimedOut(t0 + 2500) && !isBailedOut() && mobileGoalLift.val > MOBILE_GOAL_BOTTOM_LIMIT){
	      	updateSensor(&mobileGoalLift);
	      	mobileGoalAppliedVoltage = sensorHold(&mobileGoalLift, MOBILE_GOAL_BOTTOM_LIMIT, MOBILE_GOAL_DEFAULT_V, MOBILE_GOAL_MIN_V, MOBILE_GOAL_MAX_V);
	      	if(mobileGoalLift.val > 1600) mobileGoalAppliedVoltage -= 20; //Push the goal down to the ground at the end
	     		moveMobileGoalLift(mobileGoalAppliedVoltage);

					#if DEBUG_MOBILE_GOAL == 1
						writeDebugStreamLine("[MOBILE_GOAL] %d %d %d %d %d", time1[T1] - t0, MOBILE_GOAL_BOTTOM_LIMIT, mobileGoalLift.val, mobileGoalLift.speed, mobileGoalAppliedVoltage);
					#endif

	        wait1Msec(10);
	      }

      coneNum = 0;
      moveMobileGoalLift(STOP);
      MOBILE_GOAL_COMMAND = STOP;
    }
    else if(MOBILE_GOAL_COMMAND == DOWN_WITHOUT_GOAL){ //Pushes the lift to ground if it doesn't have a mobile goal
      t0 = time1[T1];
      moveMobileGoalLift(DOWN);
      updateSensorValue(&mobileGoalLift);
      while(!isTimedOut(t0 + 2500) && !isBailedOut() && mobileGoalLift.val > MOBILE_GOAL_BOTTOM_LIMIT){
      	updateSensorValue(&mobileGoalLift);
        moveMobileGoalLift(DOWN);
        wait1Msec(10);
      }
      moveMobileGoalLift(STOP);
      MOBILE_GOAL_COMMAND = STOP;
    }
    else if(MOBILE_GOAL_COMMAND == MANUAL_UP){
      moveMobileGoalLift(UP);
      MOBILE_GOAL_COMMAND = STOP;
    }
    else if(MOBILE_GOAL_COMMAND == MANUAL_DOWN){
      moveMobileGoalLift(DOWN);
      MOBILE_GOAL_COMMAND = STOP;
    }
    else{
      moveMobileGoalLift(STOP);
    }

		wait1Msec(50);
  }
}

#endif
