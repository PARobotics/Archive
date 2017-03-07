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
    fire_1_shot(LONG, 3500,  AUTOA_TPAUSE);
    intaker(IN * 127);
    fire_1_shot(LONG,  3500,  AUTOA_TPAUSE);
    fire_1_shot(LONG,  3500,  AUTOA_TPAUSE);
    intaker(STOP);
    fire_1_shot(LONG,  3500,  AUTOA_TPAUSE);


    //If there is still a ball remaining, and there is time, shoot again
    if (time1[T1]<t0+13500 && (is_loaded()==1 || is_queued() ==1)) fire_1_shot(LONG, t0+14200-time1[T1],  AUTOA_TPAUSE);

    writeDebugStreamLine("%% %d-%d=%d msec",time1[T1],t0,time1[T1]-t0);

    //Stop the bat, just in case
    if (BAT_CMD==STOP) bat(STOP);
}

/*
    AUTO B -- positioned on outside to begin
    1. run AUTO_A first to fire 4 preloads
    2. move straight by X1 (no stop)
    3. rotate by R1
    4. turn on intake and move fwd by X2 to pick up balls
    5. pull back, rotate and shoot
*/

void auto_B(){
    int t0 = time1[T1];
    int status = 0;

    //Shoot 4 balls
    auto_A();

    // Move
    int R1, X1, R2, X2, X3;
    if (SIDE==BLUESIDE){
        X1 = AUTOB_BLUE_X1;
        R1 = SIDE*AUTOB_BLUE_R1;
        X2 = AUTOB_BLUE_X2;
        X3 = AUTOB_BLUE_X3;
        R2 = SIDE*AUTOB_BLUE_R2;
        X4 = AUTOB_BLUE_X4;
    }
    else if (SIDE==REDSIDE){
        X1 = AUTOB_RED_X1;
        R1 = SIDE*AUTOB_RED_R1;
        X2 = AUTOB_RED_X2;
        X3 = AUTOB_RED_X3;
        R2 = SIDE*AUTOB_RED_R2;
        X4 = AUTOB_RED_X43;
    }

    WHEEL_MONITOR=START;
    // getWheelVal();
    status = move_x_by_nostop(X1, 6000);     // timeout is 6 seconds -- may need change
    // status = move_x_by(X1, 6000);         // not sure which one is better, stop or no stop
    if (status==0) {move_x(STOP);  return; } // if it doesn't reach x1 timeout, most likely
                                             // the robot is blocked, let us stop and exit here
    status = rotate_by(R1,3000);             // timeout = 3 seconds
    if (status==0) {move_x(STOP);  return; } // same break logic to protect auton
    intaker(IN * 127);                        // turn on intaker
    move_x_by_slow(X2, t0+14800-time1[T1]);   // Move slowly to intake balls
                                              // timeout before 15 seconds to protect otors

    // go back a bit, rotate and shoot
    move_x_by(X3, t0+14800-time1[T1]);   // Move slowly to intake balls
    rotate_by(R2,t0+14000-time1[T1]);             // timeout = 3 seconds
    move_x_by(X4, t0+14800-time1[T1]);   // Move slowly to intake balls

    // not sure if we want to shoot out the ball here, may worth a tri
    while(time1[T1]<t0+14200)
    {
        if (is_loaded()==1 || is_queued() ==1){
            fire_1_shot(SHORT, t0+14200-time1[T1],100);
        }
        else{
            wait1Msec(10);
        }
    }
    intaker(STOP);                                    // Stop intake
    WHEEL_MONITOR=STOP;
    
}

/*
    AUTO C -- positioned on outside to begin
    1. run AUTO_A first to fire 4 preloads
    2. try to control the 4 near by balls
*/

void auto_C(){
    int t0 = time1[T1];
    int status = 0;

    //Shoot 4 balls
    auto_A();

    // Move
    int R1, X1, X2;
    if (SIDE==BLUESIDE){
        X1 = AUTOC_BLUE_X1;
        R1 = SIDE*AUTOC_BLUE_R1;
        X2 = AUTOC_BLUE_X2;
    }
    else if (SIDE==REDSIDE){
        X1 = AUTOC_RED_X1;
        R1 = SIDE*AUTOC_RED_R1;
        X2 = AUTOC_RED_X2;
    }

    WHEEL_MONITOR=START;
    // getWheelVal();
    status = move_x_by_nostop(X1, 4000);     // timeout is 6 seconds -- may need change
    // status = move_x_by(X1, 6000);         // not sure which one is better, stop or no stop
    if (status==0) {move_x(STOP);  return; } // if it doesn't reach x1 timeout, most likely
                                             // the robot is blocked, let us stop and exit here
    status = rotate_by(R1,3000);             // timeout = 3 seconds
    if (status==0) {move_x(STOP);  return; } // same break logic to protect auton
    intaker(IN * 127);                        // turn on intaker
    move_x_by_slow(X2, t0+14800-time1[T1]);   // Move slowly to intake balls
                                              // timeout before 15 seconds to protect otors
    intaker(STOP);
    move_x(STOP);
    WHEEL_MONITOR=STOP;
}

/*
    AUTO D -- positioned on inside to begin
    1. run AUTO_A first to fire 4 preloads
    2. try to push some balls to corner
    3. and then intake some balls
*/
void auto_D(){
    int status = 0;

    //Shoot 4 balls
    auto_A();

    // Move
    int R1, X1, R2, X2, R3, X3, X4;
    if (SIDE==BLUESIDE){
        X1 = AUTOD_BLUE_X1;
        R1 = SIDE*AUTOD_BLUE_R1;
        X2 = AUTOD_BLUE_X2;
        R2 = SIDE*AUTOD_BLUE_R2;
        X3 = AUTOD_BLUE_X3;
        R3 = SIDE*AUTOD_BLUE_R3;
        X4 = AUTOD_BLUE_X4;
    }
    else if (SIDE==REDSIDE){
        X1 = AUTOD_RED_X1;
        R1 = SIDE*AUTOD_RED_R1;
        X2 = AUTOD_RED_X2;
        R2 = SIDE*AUTOD_RED_R2;
        X3 = AUTOD_RED_X3;
        R3 = SIDE*AUTOD_RED_R3;
        X4 = AUTOD_RED_X3;
    }

    WHEEL_MONITOR=START;
    // getWheelVal();
    status = move_x_by_nostop(X1, 3000);     // timeout is 3 seconds
    if (status==0) {move_x(STOP);  return; }
    status = rotate_by(R1,2000);             // timeout = 2 seconds
    if (status==0) {move_x(STOP);  return; } //
    status = move_x_by_nostop(X2, 2000);
    if (status==0) {move_x(STOP);  return; }
    status = rotate_by(R2,3000);
    if (status==0) {move_x(STOP);  return; }
    intaker(IN * 127);                        //if you accidentally got more than 4 balls, move this later
    move_x_by_slow(X3, 2000);                // Move slowly to intake balls and push balls to base
    rotate_by(R3,2000);
    move_x_by_slow(X4, 4000);                //
    intaker(STOP);
    move_x(STOP);
    WHEEL_MONITOR=STOP;
}




void skills_fire_1()
{
	        // move_x(STOP);

  // return the time takes to shot -- if the time increases a lot, rest the bat
  int TWAIT = 1200; // a shot cannot be longer than 1.2 sec
  int tnow = time1[T1];
  BAT_CMD = SKILLS; // cmd to fire a long shot
  wait1Msec(100); // Give some time before checking status
  while (true) {
    if (BAT_CMD==SKILLS){ // Wait if still shooting
      if (is_timed_out(tnow+TWAIT )==1){ // Break if timed out
        // BAT_CMD=STOP;         // not sure if this is necessary
        break ;
      }
      else {
        shooting_dist_ctrl();
        wait1Msec(1);
      }
    }
    else if (BAT_CMD==STOP){ // Shot is over, break out return
    	 shooting_dist_ctrl();
      break;
    }
  }
}


void skills_fire_n(int nShots )
{
  //int tnow = time1[T1];

  // fire the first shot
  skills_fire_1();

  // setup for the following shots
  // BAT_LONG_PULL_ANG_MAX += SKILLS_BAT_INC0; // slightly adjust the ANG_MAX -- since not starting at angle=0
  // BAT_SKILLS_HIT_V += 2; // slightly adjust the ANG_MAX -- since not starting at angle=0
  for( int i = 2; i < nShots+1; i ++ ){
    if (is_bailed_out()==1 ) {
      bat(STOP); BAT_CMD = STOP; BAT_MONITOR = STOP;
      break;
    }
    wait1Msec(T_INBW_REST); // rest a bit between shots
    skills_fire_1();

  }

}


void magic_1()
{
    WHEEL_MONITOR=START;
    rotate_by(MAGIC1_R1,  1500);
    intaker(IN * 127);            //incase pick up balls
    move_x_by_nostop(MAGIC1_X1,  6000);
    intaker(STOP);             // stop intake

    wait1Msec(MAGIC1_T1);      // ram in to wall
    move_x(STOP);
    move_x_by(MAGIC1_X2,  6000);    // back off a bit
    intaker(IN * 127);            //incase pick up balls

    rotate_by(MAGIC1_R2,  1500); // rotate to align shooting
    // move_x_by(MAGIC1_X3,  2000);     // may need  to move fwd a bit
    WHEEL_MONITOR=STOP;
    intaker(STOP);             // stop intake
}

void magic_2()
{
    WHEEL_MONITOR=START;
    intaker(IN * 127);            //incase pick up balls
    move_x_by_nostop(MAGIC2_X1,  2000);
    rotate_by(MAGIC2_R1,  1500);
    move_x_by_slow(MAGIC2_X2, 2000);
    wait1Msec(1000);    // wait a little for intake
    move_x_by(MAGIC2_X3, 2000);
}

void prg_skills()
{
    DIST = SKILLS;
	  skills_fire_n(32 );   // fire all 32 balls
    magic_1();
    move_x(STOP);
  	skills_fire_n( 26);   // fire 32 balls
    intaker(IN * 127);    // turn on intaker just in case we got lucky
  	skills_fire_n( 10);   // fire 32 balls
    // in case we got luck, try fire a few shots
    if (is_loaded()==1 || is_queued() ==1){ fire_1_shot(SKILLS, 2000,100);  }
    if (is_loaded()==1 || is_queued() ==1){ fire_1_shot(SKILLS, 2000,100);  }
    if (is_loaded()==1 || is_queued() ==1){ fire_1_shot(SKILLS, 2000,100);  }
    if (is_loaded()==1 || is_queued() ==1){ fire_1_shot(SKILLS, 2000,100);  }
    // now try to grab the end stacks
    magic_2();
    // follow on magic_2 with shooting
    rotate_by(MAGIC2_R2,  1500);
    move_x_by(MAGIC2_X4, 2000);
    if (is_loaded()==1 || is_queued() ==1){ fire_1_shot(SKILLS, 2000,100);  }
    if (is_loaded()==1 || is_queued() ==1){ fire_1_shot(SKILLS, 2000,100);  }
    if (is_loaded()==1 || is_queued() ==1){ fire_1_shot(SKILLS, 2000,100);  }
    if (is_loaded()==1 || is_queued() ==1){ fire_1_shot(SKILLS, 2000,100);  }
    move_x(STOP);
    // move_x_by(MAGIC1_X3,  2000);     // may need  to move fwd a bit
    WHEEL_MONITOR=STOP;
    intaker(STOP);             // stop intake

}

#endif //SOMNUS_AUTON_C
