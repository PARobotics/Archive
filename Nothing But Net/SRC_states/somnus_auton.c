#ifndef SOMNUS_AUTON_C
#define SOMNUS_AUTON_C


void fire_1_shot(int CMD, int TWAIT, int TPAUSE)
{
  // only fire when ball is loaded
    int tnow = time1[T1];

    while (true)
    {
        if (is_timed_out(tnow+TWAIT )==1){  // Break if timed out
            break ;
        }
        else if (is_loaded()==0){          // Do not fire if it isn't loaded
            wait1Msec(5);
        }
        else if (is_loaded()==1){            // Fire if loaded
            wait1Msec(TPAUSE);                 // Wait for ball to settle
            if (is_timed_out(tnow+TWAIT )==1)  break ;
            BAT_CMD = CMD;
            break;
        }
    }

    //Return after shot
    wait1Msec(100);                      // Give time for ball to shoot
    while (true) {
    if (BAT_CMD==CMD){             // Wait if still shooting
        wait1Msec(5);
    }
    else if (BAT_CMD==STOP){         // If it isn't shooting, break out
        break;
    }
  }
}

/*
    AUTO A
    Fire 4 shots and wait
*/

void auto_A(){
    int t0 = time1[T1];
    BAT_MODE = SINGLE;
    AUTO_ADJUST = ON;
    fire_1_shot(LONG, 3500,  100);
    intaker(IN * 127);
    fire_1_shot(LONG,  3500,  100);
    fire_1_shot(LONG,  3500,  100);
    intaker(STOP);
    fire_1_shot(LONG,  3500,  100);
    AUTO_ADJUST = OFF;


    //If there is still a ball remaining, and there is time, shoot again
    if (time1[T1]<t0+13500 && (is_loaded()==1 || is_queued() ==1)) fire_1_shot(LONG, t0+14200-time1[T1],  100);

    writeDebugStreamLine("%% %d-%d=%d msec",time1[T1],t0,time1[T1]-t0);

    //Stop the bat, just in case
    if (BAT_CMD==STOP) bat(STOP);
}

/*
    AUTO B
    Fire 4 shots, pick up 0 - 4 balls and shoot them
*/

void auto_B(){
    int t0 = time1[T1];

    //Shoot 4 balls
    auto_A();

    // Move
    int R1, X1, R2, X2, R3;
    if (SIDE==BLUESIDE){
        R1 = AUTOB_BLUE_R1;
        X1 = AUTOB_BLUE_X1;
        R2 = AUTOB_BLUE_R2;
        X2 = AUTOB_BLUE_X2;
        R3 = AUTOB_BLUE_R3;
    }
    else if (SIDE==REDSIDE){
        R1 = AUTOB_RED_R1;
        X1 = AUTOB_RED_X1;
        R2 = AUTOB_RED_R2;
        X2 = AUTOB_RED_X2;
        R3 = AUTOB_RED_R3;
    }

    WHEEL_MONITOR=START;
    // getWheelVal();
    int rot0 = Gyro_val;
    int wheel0 = Wheel_val;
    rotate_by(R1,500);                                // Rotate Slightly
    intaker(IN*127);                                  // Start intake
    move_x_by_slow(X1, t0+14000-time1[T1]);           // Move slowly to intake
    rotate_by(R2,t0+14000-time1[T1]);                 // Rotate slightly
    move_x_by_slow(X2, t0+14000-time1[T1]);           // Move slowly to intake again
    intaker(STOP);                                    // Stop intake
    rotate_by(R3,t0+14000-time1[T1]);                 // Rotate to align with net
    WHEEL_MONITOR=STOP;

    if (abs(Wheel_val - wheel0-X1-X2)>1000) return; // If the distance is off, don't shoot
    if (abs(Gyro_val-rot0-R1-R2-R3) >200) return;   // If the rotation is off, don't shoot

    while(time1[T1]<t0+14200)
    {
        //If there is enough time and the ball is loaded shoot
        if (is_loaded()==1 || is_queued() ==1){
            fire_1_shot(SHORT, t0+14200-time1[T1],100);
        }
        else{
            wait1Msec(10);
        }
    }
}

void auto_C(){

}


void skills_fire_1()
{
  // return the time takes to shot -- if the time increases a lot, rest the bat
  int TWAIT = 1200; // a shot cannot be longer than 1.2 sec
  int tnow = time1[T1];
  BAT_CMD = LONG; // cmd to fire a long shot
  wait1Msec(100); // Give some time before checking status
  while (true) {
    if (BAT_CMD==LONG){ // Wait if still shooting
      if (is_timed_out(tnow+TWAIT )==1){ // Break if timed out
        // BAT_CMD=STOP;         // not sure if this is necessary
        break ;
      }
      else {
        wait1Msec(5);
      }
    }
    else if (BAT_CMD==STOP){ // Shot is over, break out return
      break;
    }
  }
}


void prg_skills(int nShots )
{
  int tnow = time1[T1];

  // fire the first shot and record how long it takes to pull
  BAT_MODE = SINGLE;
  skills_fire_1();
  int tShot0 = TIME_PULL;

  // setup for the following shots
  BAT_LONG_PULL_ANG_MAX += SKILLS_BAT_INC0; // slightly adjust the ANG_MAX -- since not starting at angle=0
  AUTO_ADJUST = ON; // turn on the auto_adjust
  for( int i = 2; i < nShots+1; i ++ ){
    if (is_bailed_out()==1 ) {
      bat(STOP); BAT_CMD = STOP; BAT_MONITOR = STOP;
      break;
    }
    wait1Msec(T_INBW_REST); // rest a bit between shots
    skills_fire_1();

    // if taking too long to pull; battery needs some rest
    if (TIME_PULL-tShot0 > D_TIME_PULL) {
      bat(STOP); BAT_CMD = STOP; BAT_MONITOR = STOP;
      wait1Msec(T_BATT_REST);
      // if shot too fast after rest; need to reduce Max
      // if(abs(BAT_LONG_PULL_ANG_MAX-BAT_LONG_PULL_ANG_MAX_BASE)>10) BAT_LONG_PULL_ANG_MAX -= 5;
    }

    // we can also force some rest time at specified shot number
    // if (i==20) {
      // wait1Msec(T_BATT_REST);
      // //if(abs(BAT_LONG_PULL_ANG_MAX-BAT_LONG_PULL_ANG_MAX_BASE)>10) BAT_LONG_PULL_ANG_MAX -= 5;
    // }
  }
  writeDebugStreamLine("%% tshot=%d",time1[T1]-tnow);

  // Repositions
  magic_1();

  // second round of
  BAT_LONG_PULL_ANG_MAX -= SKILLS_BAT_INC2; // maybe need to readjust the range (since it took a while to move and batt got enough rest
  skills_fire_1();
  for( int i = 2; i < 33; i ++ ){
    wait1Msec(T_INBW_REST); // rest a bit between shots
    skills_fire_1();
    if (TIME_PULL-tShot0 > D_TIME_PULL) {
      bat(STOP); BAT_CMD = STOP; BAT_MONITOR = STOP;
      wait1Msec(T_BATT_REST);
    // if(abs(BAT_LONG_PULL_ANG_MAX-BAT_LONG_PULL_ANG_MAX_BASE)>10) BAT_LONG_PULL_ANG_MAX -= 5;
    }
  }
  AUTO_ADJUST = OFF;

}

/*
    MAGIC 1
    Move fwd, turn 90 degrees, move backwd
*/

void magic_1()
{
    WHEEL_MONITOR=START;
    move_x_by(MAGIC1_X1,  4000);
    rotate_by(SIDE * MAGIC1_R1,  1500);
    move_x_by(MAGIC1_X2, 4000);
    WHEEL_MONITOR=STOP;
}

void magic_2()
{
    WHEEL_MONITOR=START;
    rotate_by(MAGIC2_R1,  1500);
    intaker(IN * 127);

    move_x_by(MAGIC2_X1,  4000);
    rotate_by(MAGIC2_R2,  1500);
        intaker(STOP);

    move_x_by(MAGIC2_X2,  4000);
    WHEEL_MONITOR=STOP;
}


#endif //SOMNUS_AUTON_C
