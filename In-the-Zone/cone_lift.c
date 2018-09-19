#ifndef CONE_LIFT_C
#define CONE_LIFT_C

#define SWING_ARM_MAX 265
#define SWING_ARM_MIN 5
#define CONE_LIFT_DEFAULT_V 10
#define CONE_LIFT_MAX_V 10
#define CONE_LIFT_MIN_V -50

/*
  CONE_LIFT.C
  Contains all the code for the cone claw and lift
*/

// ** Claw **
void moveClaw(int status){ //Manually opens and closes the claw
  if(status == CLOSE) moveClaw(127);
  else if(status == OPEN) moveClaw(-127);
  else motorReq[M_CLAW] = status;
}

void openClaw(){ //Automatically opens the claw
  int t0 = time1[T1];
  moveClaw(OPEN);
  clawIsClosed = 0;
  clawIsOpened = 1;
  while(!isTimedOut(t0 + 50)){
    moveClaw(OPEN);
    wait1Msec(10);
  }
  moveClaw(-20);
}

void closeClaw(){ //Automatically closes the claw
  int t0 = time1[T1];
  moveClaw(CLOSE);
  clawIsClosed = 1;
  clawIsOpened = 0;
  while(!isTimedOut(t0 + 50)){
    moveClaw(CLOSE);
    wait1Msec(10);
  }
  moveClaw(35);
}

// ** Swinging Arm **
void moveSwingArm(int status){
  if(status == UP){
    moveSwingArm(127);
  }
  else if(status == DOWN){
    moveSwingArm(-127);
  }
  else motorReq[M_SWING_ARM] = status;
}

void swingArmUp(){
	swingArmIsUp = 1;
  moveSwingArm(UP);
  wait1Msec(800);
  moveSwingArm(STOP);
}

void swingArmDown(){
	swingArmIsUp = 0;
  moveSwingArm(DOWN);
  wait1Msec(1000);
  moveSwingArm(STOP);
}

// ** Lift **
void moveLift(int status){
  if(status == UP){
    moveLift(127);
  }
  else if(status == DOWN){
    moveLift(-127);
  }
  else motorReq[M_LIFT] = status;

}

void moveLiftToCone(int coneNum){
	int liftVals[7] = {323, 640, 800, 909, 1118, 1267, 1333};
	liftVal = liftVals[coneNum];
	CONE_LIFT_COMMAND = PRESET;
}

void autoInternalStack(){
	moveLift(UP);
	while(stillNeedToLift() && vexRT[BAILOUT_BUTTON] == 0){
		wait1Msec(10);
	}
	moveLift(STOP);
	wait1Msec(100);
	SWING_ARM_COMMAND = UP;
	wait1Msec(1000);
	moveLift(DOWN);
	wait1Msec(500);
	moveLift(STOP);
	openClaw();
	wait1Msec(200);
	moveLift(UP);
	while(stillNeedToLift() && vexRT[BAILOUT_BUTTON] == 0){
		wait1Msec(10);
	}
	moveLift(STOP);
	SWING_ARM_COMMAND = DOWN;
}

bool stillNeedToLift(){
  return SensorValue(LiftUltraSonic) < 10;
}

task swingArmTask(){
	while(true){
		if(SWING_ARM_COMMAND == UP){
			swingArmUp();
			SWING_ARM_COMMAND = HOLD;
		}
		else if(SWING_ARM_COMMAND == DOWN){
			swingArmDown();
			SWING_ARM_COMMAND = HOLD_DOWN;
		}
		else if(SWING_ARM_COMMAND == HOLD){
			moveSwingArm(40);
		}
		else if(SWING_ARM_COMMAND == HOLD_DOWN){
			moveSwingArm(-20);
		}
		else if(SWING_ARM_COMMAND != PRESET){
			moveSwingArm(STOP);
		}

		wait1Msec(50);
	}
}

task coneLiftTask(){

  pid liftPid;
  liftPid.kp = 13;
  liftPid.kd = 0.1;

  initializeSensor(&liftSensor, 1.0, dgtl2, &liftPid);

  int dbgCnt = 0;
  int vcmd = 0;
  int targetVal = 0;
  int isHolding = 0;

  while(true){
  	if(CONE_LIFT_COMMAND != HOLD) isHolding = 0;

    if(CONE_LIFT_COMMAND == HOLD){ //Keeps the lift at the same place
    	updateSensor(&liftSensor);

    	if(isHolding == 0){
    		isHolding = 1;
    		targetVal = liftSensor.val;
    	}

    	vcmd = sensorHold(&liftSensor, targetVal, CONE_LIFT_DEFAULT_V, CONE_LIFT_MIN_V, CONE_LIFT_MAX_V);
   		if(liftSensor.val <= 15) motorReq[M_LIFT] = vcmd;
   		else motorReq[M_LIFT] = 0;

      #if DEBUG_CONE_LIFT == 1
        if(dbgCnt == 10){
          dbgCnt = 0;
        }
        else dbgCnt++;
      #endif
    }
    else if(CONE_LIFT_COMMAND == UP){
      moveLift(UP);
    }
    else if(CONE_LIFT_COMMAND == DOWN){
      moveLift(DOWN);
    }
    else if(CONE_LIFT_COMMAND == PRESET){
    	updateSensorValue(&liftSensor);
   		moveLift(SIGN(liftVal - liftSensor.val));
      if(abs(liftSensor.val - liftVal) < 3) CONE_LIFT_COMMAND = STOP;
    }
    else if(CONE_LIFT_COMMAND == AUTO_INTERNAL_STACK){
    	autoInternalStack();
  	}
    else if(CONE_LIFT_COMMAND == STOP){
      moveLift(STOP);
    }

    wait1Msec(10);
  }
}

#endif
