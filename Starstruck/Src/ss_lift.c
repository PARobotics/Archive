#ifndef SS_LIFT
#define SS_LIFT

// #define LIFT_MIN         -50
// #define LIFT_MAX         2400
// #define LIFT_H_GRAB        3000      // lift value to check if height good for grab
// #define LIFT_H_GRAB2       2500      // second height to check if grabbed
// #define LIFT_SPEED_CAP     199      //lift_v threshold
#define LIFT_MIN         50
#define LIFT_MAX         950
#define LIFT_H_GRAB      900      // lift value to check if height good for grab
#define LIFT_H_GRAB2     780      // second height to check if grabbed
#define LIFT_SPEED_CAP   600      //1000/5 = 200 is good speed
#define LIFT_H_STOP1   100      //1000/5 = 200 is good speed
#define LIFT_H_STOP2   50      //1000/5 = 200 is good speed

//int LIFT_GRABBED = 0;

/*void lift_grabbed(){
	//Check between 2100 and 2300: Vel must be less than 1500 to be grabbed
	LIFT_MONITOR = START;
	if(LIFT_VAL >= 2100 && LIFT_VAL <= 2300 && LIFT_V > 1500) {
		LIFT_GRABBED = 0;
		break;
	}
	else LIFT_GRABBED = 1;
}*/

void lift_up(){
  // lift up is to ppo up the lift or lower the robot from pole
  // should be done in 0.75
	/*LIFT_COMMAND = UP;
	int tnow = time1[T1];
	// while((LIFT_VAL < LIFT_MAX || LIFT_VAL > 3000) && time1[T1] - tnow < 1500){
	while((LIFT_VAL < LIFT_MAX ) && time1[T1] - tnow < 750){
		wait1Msec(5);
	}
	LIFT_COMMAND = STOP;*/
	SensorValue(P_LIFT) = 1;
}

void lift_down(){
  // lift up is to lift the robot up
  // stop when lift_val reach threshold???
	LIFT_COMMAND = DOWN;
	int tnow = time1[T1];
	while(LIFT_VAL > LIFT_MIN || time1[T1] - tnow >= 5000){
		//lift_grabbed();
		if(LIFT_VAL >= LIFT_H_STOP2 && LIFT_VAL <= LIFT_H_STOP1) {
			break;
		}
		wait1Msec(5);
	}
	LIFT_COMMAND = STOP;
}

void wiggle_align() {

  move_force(-30, 60);
  wait1Msec(300);
  move_force(-30, -60);
  wait1Msec(300);
}

void go_lift() {
  // make sure use the correct value
  // go lift lift robot up with check if it grabs correctly and if not do adjustment
  // first check if lift is up there before lower it
  // now lift down
  LIFT_MONITOR = START;
  // if (LIFT_VAL < LIFT_H_GRAB) {
    // SensorValue(P_LIFT) = 0;
    // wait1Msec(500);
    // lift_up();
    // wait1Msec(500);

    // if (LIFT_VAL < LIFT_H_GRAB) { return;}
  // }
  lift_up();
  int n_trial = 0;
  // LIFT_COMMAND = DOWN;
  SensorValue(P_LIFT) = 0
  wait1Msec(500);
   // fail to grab, we try 2 times
  while (n_trial < 2) {
    // we didn't go too fast -- a sign of good grab
    // if (LIFT_VAL>LIFT_H_GRAB2 && LIFT_V<LIFT_SPEED_CAP) {
    if (LIFT_VAL>LIFT_H_GRAB2 ) {
      break;
    }
    // otherwise, realign and try again
    n_trial ++;
    if (n_trial>2) return;    // we tried twice, let us give up
    lift_up();
    wait1Msec(500);
    wiggle_align();
    // LIFT_COMMAND = DOWN;
    SensorValue(P_LIFT) = 0
    wait1Msec(500);
  }

  // now let us do the real lift
  lift_down ();
  LIFT_MONITOR = STOP;
}

 /*
void one_lift(){
	int status = 0;
	//Lift up
	SensorValue(P_LIFT) = 1;
	while(LIFT_VAL < LIFT_MAX || LIFT_VAL > 3000){
		wait1Msec(5);
	}

	//Test if grabbed
	do{
		LIFT_COMMAND = DOWN;
		while(true){
			//lift_grabbed();
			if(LIFT_VAL >= 2100 && LIFT_VAL <= 2300) {
				status = 0;
				break;
			} else if(LIFT_VAL >= 2100 && LIFT_VAL <= 2300){
				status = 1;
				LIFT_COMMAND = STOP;
				break;
			}
			wait1Msec(5);
		}
		if(status == 0){
			lift_up();
			//wiggle
		}
	} while(status == 0);

	//Go down
	int tnow = time1[T1];
	LIFT_COMMAND = DOWN;
	while(LIFT_VAL > LIFT_MIN || time1[T1] - tnow >= 5000){
		wait1Msec(5);
	}
	LIFT_COMMAND = STOP;
}
*/

void lift_piston(int status){
  if(status == TOGGLE){
    if(SensorValue(P_LIFT) == 1) status = 0;
    else status = 1;
  }
  SensorValue(P_LIFT) = status;
}

void dump_wiggle(){
	DUMP_COMMAND = PRG;
  dumper(70);
  wait1Msec(500);
  dumper(-70);
  wait1Msec(500);
  DUMP_COMMAND = STOP;
}

void transmission(int status){
  //Toggle piston
  if(status == TOGGLE){
    if(SensorValue(P_TRANS) == 1) {
			LIFT_MONITOR = STOP;
			SensorValue(P_TRANS) = 0;
		}
    else{
			LIFT_MONITOR = START;
			SensorValue(P_TRANS) = 1;
		}
  }
  else{
    SensorValue(P_TRANS) = status;
  }
  //Wiggle
  // dump_wiggle();
  DUMP_COMMAND = WIGGLE;
}

/*
  LIFT TASK
  Constantly monitor lift value and stop when it goes out of lift values
*/

task ss_lift(){
  while (true) {
		if(BAILOUT == 1) LIFT_COMMAND = STOP;
    if(LIFT_COMMAND == UP){
      SensorValue(P_LIFT) = 1;
      dumper(-127);
    }
    else if(LIFT_COMMAND == DOWN){
      SensorValue(P_LIFT) = 0;
      dumper(127);
    }
    else if(LIFT_COMMAND == MANUAL_UP){
      dumper(-127);
    }
    else if(LIFT_COMMAND == MANUAL_DOWN){
      dumper(127);
    }
    else if(LIFT_COMMAND == STOP && DUMP_COMMAND == STOP){
      dumper(STOP);
    }
    wait1Msec(5);
  }
}

#endif //SS_LIFT
