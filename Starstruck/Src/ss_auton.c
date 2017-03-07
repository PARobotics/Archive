#ifndef SS_AUTON
#define SS_AUTON
/*
  AUTO A: Main autonomous mode
  Dump preload where 3 stars are
  1) Move curved towards cube. Grab and dump
  2) Rotate slightly and grab 2 stars on the left side of the center 4 stars. Dump
  3) Grab 2 remaining center stars and dump
*/
void one_dump(int arm_down_angle, int tlimit_up, int tlimit_down);
void move_bkwd_dump(int arm_down_angle);
void move_s_fwd(int dir, int dx, int d0, int ang0);
// void move_s_bkwd_dump(int dir, int dx, int arm_down_angle, int d0, int ang0);
void auto_A();
void auto_B();
void auto_C();
void prg_skills();
void prg_mv1();
void prg_mv2();
void prg_mv3();
void prg_mv4();
void prg_mv5();
void prg_mv6();
// #define VOLT_PUSH_FENCE 30
// #define VOLT_PUSH_FENCE -30         // this value has to be negative since it is going backward
// #define VOLT_PUSH_FENCE -50         // this value has to be negative since it is going backward
#define VOLT_PUSH_FENCE_0 -50         // this value has to be negative since it is going backward
// #define VOLT_PUSH_FENCE_1 -6127         // this value has to be negative since it is going backward
#define VOLT_PUSH_FENCE_1 -45        // according to jeff, reduce the puch fence voltage
// the following 2 numbers require data dump to be more accurate
// #define Y_POS_FENCE 660      // pos of Y where to start DUMP
#define Y_POS_FENCE 640      // pos of Y on fence
// #define Y_POS_DUMP 480      // pos of Y where to start DUMP
// #define Y_POS_DUMP 400      // pos of Y where to start DUMP
//#define Y_POS_DUMP 420      // pos of Y where to start DUMP
#define Y_POS_DUMP 450
                            // from video 500 should be good, but a little slow
                            // let us use 480
// #define Y_POS_ONEDUMP 600      // not really matter, when to check on dump 600 good
#define Y_POS_ONEDUMP 560      // 600 is too far

#define Y_POS_WALL 300     // pos pf Y where to stop move and start to grab
                           // from video it is something like 260, but use 300

#define AUTO_DELTA_ANG 300   // this is the parameter to adjust in the first rotation of autoA or autoB
int AUTON_MV1_ANG = 100;     //rotate extra 10 degree for the first throw
int IS_ALIGN_FENCE  = 0;
// convert angle from left side (+1) default to right side default
int ang_side(int ang) {
  if (SIDE==-1) ang += 3600;
  return ang;
}


void move_fwd_and_grab(){
    // move_fwd(); move_by(DRV.pos[1]-Y_POS_WALL ,1400); move_stop();  // 1 second too short , that is why not moving enough to position
    move_fwd(); move_by((DRV.pos[1]-Y_POS_WALL)/fabs(sin(DRV.pos[2]/DRV.pi2d)) ,1500); move_stop();  // 1 second too short , that is why not moving enough to position
    // move_fwd(); move_by(DRV.pos[1]-Y_POS_WALL + 200,1000); move_stop();
    // claw(CLOSE); DUMP_COMMAND = CARRY; wait1Msec(100);
    claw(CLOSE); DUMP_COMMAND = CARRY; wait1Msec(100);
}

void one_dump(int arm_down_angle, int tlimit_up, int tlimit_down) {
  /*we will do one dump (by raising the stuff high than release
  and then move the dump down till certain angle, will
  keep DUMP_COMMAND to down
  it takes about 1.0 seconds for the arm to go down and hold on ground
    0.6 seconds to about 80 deg
    0.8 seconds to 40 degree
    1.0 seconds to 0 degree
  arm_down_angle: the arm angle when robot can starts to move again
  for fwd grab motion, we have about 0.4 seconds till arm has to be down
     one_dump(500,1200,1000)  should work
  for complex motion when we have more time
     one_dump(1200,1200,1000)  gives 1.0 seconds to lower arm*/

  DUMP_COMMAND = UP;
  int tnow = time1[T1];
  // or so if claw is already open, no need to go up again???

  // while (DUMP_VAL < DUMP_RELEASE && SensorValue(P_CLAW_L)==CLOSE && time1[T1] - tnow < tlimit_up) {  // given it an extra degree to be safe
  // while (DUMP_VAL < DUMP_MAX && SensorValue(P_CLAW_L)==CLOSE && time1[T1] - tnow < tlimit_up) {  // given it an extra degree to be safe
  // *** it let go and stop too soon at DUMP_RELEASE .. go all the way to DUMP_MAX
  while (DUMP_VAL < DUMP_MAX && time1[T1] - tnow < tlimit_up) {  // given it an extra degree to be safe
    // somehow
    dumper(127);   // somehow in ss_dumper the 127 will suddenly goes to zero, let us put it here to be safe
    wait1Msec(5);
  }
  DUMP_COMMAND = STOP;
  // wait1Msec(100);
  wait1Msec(100);
  tnow = time1[T1];
  DUMP_COMMAND = DOWN;

  while(DUMP_VAL > arm_down_angle && time1[T1] - tnow < tlimit_down){
    wait1Msec(5);
  }
}

void move_bkwd_dump(int arm_down_angle)
{
  // arm_down_angle: the arm angle when robot can starts to move again
  // move bkwd till hit the trigger Y to dump
  // also push the fence during the dump

  // -- mv to Y_POS_DUMP and start to dump
  dbgMsg("Move_bkwd and dump ");
  wheel_refresh(); move_bkwd();
  if (DRV.pos[1]<Y_POS_DUMP) move_by(Y_POS_DUMP-DRV.pos[1],1200);  //cannot take more than 1.2 seconds
  wheel_refresh();
  DUMP_COMMAND = UP;

  // -- continue move while dumping till Y_POS_ONEDUMP to finish dump
  // -- in the mean time keep the move voltage to push fence
  move_by(Y_POS_ONEDUMP-DRV.pos[1],400);    // cannot take more than 0.4 second
  wheel_refresh();
  if (IS_ALIGN_FENCE == 0)   {
    move(VOLT_PUSH_FENCE_0,0);
  }
  else {
    move(VOLT_PUSH_FENCE_1,0);
  }
  one_dump(arm_down_angle, 2500, 750);         // will resume move after dump is lower than arm_down_angle
  // wheel_reset(600,720,-900); //We know our position because we hit the wall
  move_stop();
}

void move_s_fwd(int dir, int dx, int d0, int ang0)
{
  // this is a fancy fence to wall movement with lateral shift
  //    1. you start at around [x, 660, -900]
  //    2. move fwd by d0
  //    3. rotate by ang0 -- adjusted by error to -900  (actual rotation is larger than ang0 due to inertia)
  //    4. move fwd by some dist (till desired lateral position)
  //    5. now rotate back by ang0
  //    6. move fwd till claw line up with the end of wall
  //
  // dir: = 1 left to right lateral shift (from RED SIDE)
  // dx:      is the amount of lateral shift (150 or 300 etc) -- be positive
  // d0 should be something like 120 (12")
  // ang0 should be something like 450-750
  //
  //   to shift 15" laterally a guess is move_s_fwd(1, 150, 120, 450)
  //   to shift -30" laterally a guess is move_s_fwd(-1, 300, 120, 750)

  // find xtarget
  wheel_refresh();    // DRV.pos ~= [x,65, -90]
  int xtarget = DRV.pos[0]+dir*fabs(dx);
  // move fwd by d0
  dbgMsg("m_s_fwd 0");
  move_fwd();    move_by(d0,1000);wheel_refresh();

  // now rotate left (auto correct error to -900, which is -900-DRV.pos[2])
  // if  you start -1000, need to rotate extra 10 deg
  dbgMsg("m_s_fwd 1");
  rotate(-SIDE*dir*127);    // rotate left (angle increases)
  // dir*ang0 should be adjust by SIDE*delta where delta =  ang_side(-900)-DRV.pos[2]
  // if doesn't work switch back to your original stuff
 // rotate_by(dir*SIDE*ang0, 1000)
  rotate_by(dir*ang0+SIDE*(ang_side(-900)-DRV.pos[2]), 1000);

  // move_fwd till xtarget is hit
  dbgMsg("m_s_fwd 2");
  wheel_refresh();   move_fwd();
  int tnow0 = time1[T1];
  dx = fabs(xtarget-DRV.pos[0]);   // update dx to x-distance to target
  int X0 = DRV.pos[0];
  while (true) {
    if (is_timed_out(tnow0 + 2000) == 1) { return; }
    if (fabs(DRV.pos[0]-X0)>=dx) { break;};
    wait1Msec(2);
  }

  // now rotate right -- assuming the inertial is same, you will be back to -900
  wheel_refresh();  rotate(SIDE*dir*127);    // rotate right
  rotate_by(ang0,1000);

  wheel_refresh();  move_fwd();
  // -- this movement apparently has error in it and didn't stop until timed out at
  // 1.5 seconds -- we cannot get the stuff correct w/o sensor measurements
  // let us reduce the timeout time for now; from the video the position at wall
  // is more like 260, let us use 300 to allow inertial to take care of the stuff
  // from video 800 msec appears to be sufficient, let use 900 then
  move_by(DRV.pos[1]-Y_POS_WALL, 900);  // drive fwd so that the claw is 2" away from wall -- might need to adjust
  move_stop();
}

/*void move_s_bkwd_dump(int dir, int dx, int arm_down_angle, int d0, int ang0)
{
  // it is the reverse of move_s_fwd, but we will start DUMP UP 120" from fence
  // arm_down_angle refer to one dump
  //   to shift -24" and dump a guess is move_s_kbwd_dump(-1, 240, arm_down_angle, 120, 600)
  // find xtarget
  wheel_refresh();    // DRV.pos ~= [x,65, -90]
  int X0 = DRV.pos[0];

  // move bkwd by d0
  dbgMsg("m_s_bkwd 0");
  wheel_refresh(); move_bkwd();    move_by(d0,1000);

  // now rotate right (auto correct error to -900 -- note the sign is different, if you already -1000, will rotate 10 deg less)
  dbgMsg("m_s_bkwd 1");
  wheel_refresh(); rotate(SIDE*dir*127);    // rotate left (angle increases)
  // rotate_by(ang_side(-900)-DRV.pos[2]+dir*SIDE*ang0, 1000);   // correct error to -90 here
  rotate_by(ang0, 1000);   // correct error to -90 here

  // move_bkwd till xtarget is hit
  dbgMsg("m_s_bkwd 2");
  wheel_refresh(); move_bkwd();
  int tnow0 = time1[T1];
  // don't know why the logics never got hit -- need sensor data
  // in the mean time, let us reduce time out and reduce dx
  // it cannot be more than 900 msec
  while (true) {
    if (is_timed_out(tnow0 + 900) == 1) { break; }
    if (fabs(DRV.pos[0]-X0)>=dx) { break;};
    wait1Msec(2);
  }

  // now rotate left -- assuming the inertial is same, you will be back to -900
  dbgMsg("m_s_bkwd 3");
  wheel_refresh();rotate(-SIDE*dir*127);    // rotate right
  rotate_by(ang0,1000);

  // we will dump around Y=500
  dbgMsg("m_s_bkwd 4");
  wheel_refresh(); move_bkwd();
  if (DRV.pos[1]<Y_POS_DUMP) {move_by(Y_POS_DUMP-DRV.pos[1],1500);}
   // now start dump

  dbgMsg("m_s_bkwd 5");
  wheel_refresh();
  DUMP_COMMAND = UP;
  move_by(600-DRV.pos[1],1200);    // drive to Y=60 check on dump

  // set voltage to push wall
  dbgMsg("m_s_bkwd 6");
  wheel_refresh();
  if (IS_ALIGN_FENCE == 0)   {
    move(VOLT_PUSH_FENCE_0,0);
  }
  else {
    move(VOLT_PUSH_FENCE_1,0);
  }
  one_dump(arm_down_angle, 1000, 750);         // will resume move after dump is lower
  move_stop();
}
*/

/*
auto_mv1 is called by both auto_A and auot_b that does
the grab rotate and dump of the first cube
*/

void auto_mv1(){
  // wheel_reset(-SIDE*380, 80,900-SIDE*350);
  wheel_reset(-SIDE*380, 100,900-SIDE*350);

  claw(OPEN);
  DUMP_COMMAND = HOLD;
  wait1Msec(500);
  int tnow = time1[T1];
  while(DUMP_VAL > DUMP_MIN && time1[T1] - tnow < 1000){
    wait1Msec(5);
  }

  wheel_refresh();

  //Move 1.1: Curved motion + Grab cube
  dbgMsg("Curved motion to grab cube");
  move_curve(SIDE*6);
  move_by(240,2000);
  claw(CLOSE);
  // wait1Msec(50);    // need this??

  //*** CHANGE1 -- this part code is not good
  //    it takes > 0.4 seconds to lift the dumper
  //    which moves more than 5" -- there fore how far it moves
  //    fwd become random
  // instead, i lift it to about 32 deg -- inertia will carry it to about 35
  // ad use carry high to keep it in 40-60 deg
  // and then we let it travel about 14" during the process
  wheel_refresh();   move_fwd();
  DUMP_COMMAND = UP;
  while(DUMP_VAL < 320){  wait1Msec(50);   }
  // DUMP_COMMAND = STOP;
  DUMP_COMMAND = CARRY;  // we want to keep the cube high so it won't rub the stars
  move_by(140, 900);          //**** 140 is a parameter that you may need to adjust

  //Move 2: Rotate we know it is about 1200, go 900 fast first
  //        and go 300 small (slow to be better accurate)
  //        alternately we can use rotate_brake(-1200, correct angle....)
 // current position should be 1250 from right (SIDE=-1) and 550 from left
 // we need to rotate to about 2700 from right (SIDE=-1) and -900 from left
 // *** CHANGE2 -- this is accurate but slow and not necessay for auton
 //            let us use inertia to our advantage and adjust the one parameter
 //            to roughly control the angle to be correct (AUTO_DELTA_ANG)
  dbgMsg("rotate with cube");
  wheel_refresh(); rotate(SIDE*127);
  // rotate_by was incorrect too -- were lucky due to timeout logics
  // for SIDE=-1, right, it is 2700-1250-300
  // for SIDE=+1, left , it is abs(-900-550)-300
  rotate_by(fabs(ang_side(-900)-DRV.pos[2])-AUTO_DELTA_ANG+AUTON_MV1_ANG,1000);    // control which way it throws the first cube
  // -- for the last 30 deg, use rotate_small
  // there were error in rotate small -- it was never triggered
  // -- we are not use rotate_small anymore but use AUTO_DELTA_ANG to get it about right
  // rotate_small(-SIDE*300);    // the last 30 degree more accurate, -300 go right side

  //Move 3: Move backwards and dump
  // about 30" from the wall; takes 1 seconds or so, let us go half way and start to dump
  // s-move takes long, therefore we can exist slight early at arm = 120 deg
  DUMP_COMMAND = STOP;           // let it drop first -- so we can use rubber band rebound to throw farther ...
  wheel_refresh();   move_bkwd_dump(600);

  // let us not reset here -- no need to be so accurate let us trust sensor here
}

void auto_A() {
  //start auto A, we need to wait a while till .5 msec
  // for the claw to drop and afterwards, wait till
  // claw has been hold to center degree
  dbgMsg("Start auto_a");
  WHEEL_MONITOR = START;          // start time
  DUMP_MONITOR = START;          // start time
  // do auto mv1 to dump the first cube
  AUTON_MV1_ANG = 0;                  // throw straight
  auto_mv1();

  // 4.1 now the fancy s move to grab
  dbgMsg("S move lateral 15 inch to grab");
  DUMP_COMMAND = HOLD;    // hold dumper to ground
  // appaent lots of rotation inertia here ; 20 deg -> 45 degree
  //move_s_fwd(1, 20, 0, 100);   move_stop();
  move_fwd_and_grab();
  // we need to wait a bit till the arm raised before move back
  // it takes about 300 msec to raise to 25 deg; hold 250 msec

  // 4.2 now move bkwd and dump
  // s-move takes long, therefore we can exist slight early at arm = 120 deg
  wheel_refresh();   move_bkwd_dump(1200);   //
  move_stop();

  // 5.1 now the fancy s move to grab, want x pos to be -240
  dbgMsg("S move lateral -30 inch to grab");
  wheel_refresh();
  DUMP_COMMAND = HOLD;    // hold dumper to ground
  //move_s_fwd(1, fabs(-240-DRV.pos[0]), 120, 750);   move_stop();
  // **** I think you are not rotating enough in the beginning, there fore missed the stuff
  //      toward the end -- let us increase the angle
  // move_s_fwd(-1, 150, 0, 150); move_stop();
  move_s_fwd(-1, 150, 0, 200); move_stop();
  // we need to wait a bit till the arm raised before move back
  // it takes about 300 msec to raise to 25 deg; hold 250 msec
  claw(CLOSE); DUMP_COMMAND = CARRY;  wait1Msec(250);


  // 5.2 the fancy s bkwd and dump (240" lateral) -- xpos to be about -480
  // no need to lower arm down, since power is cut at the end and
  // we want it up there for defence, get out at 120 deg
  wheel_refresh();
  //move_s_bkwd_dump(-1, fabs(-480-DRV.pos[0]),1200, 120, 600);
  DUMP_COMMAND = CARRY;
  // move_s_bkwd_dump(-1, 2400, 1200, 120, 450);
  //move_s_bkwd_dump(-1, 100, 1200, 60, 200);
  move_bkwd_dump(1200);
  DUMP_COMMAND = STOP;
  move_stop();

  // finish up
  wait1Msec(100);
  DUMP_MONITOR = STOP;
  WHEEL_MONITOR = STOP;
}

/*
  AUTO B: Complement autonomous
*/

void auto_B() {
  //auto_B is a back up tp auto_A, in case alliance will go after the mid stars
  // drop the star in the corner
  wait1Msec(0);         // modify the waitime to avoid collision with alliance

  dbgMsg("Start auto_B");
  WHEEL_MONITOR = START;          // start time
  DUMP_MONITOR = START;          // start time
  wheel_reset(-SIDE*380, 80,900-SIDE*350);

  claw(OPEN);
  DUMP_COMMAND = HOLD;
  wait1Msec(500);
  int tnow = time1[T1];
  while(DUMP_VAL > DUMP_MIN && time1[T1] - tnow < 1000){
    wait1Msec(5);
  }

  wheel_output_pos(1);
  wheel_refresh();

  //Move 1.1: Curved motion + Grab cube
  dbgMsg("Curved motion to grab cube");
  move_curve(SIDE*6);
  move_by(240,2000);
  // move_stop();
  claw(CLOSE);
  // wait1Msec(50);    // need this??

  // Move 1.2 continue fwd (you don't want caught by
  // fence leg in dump later)
  dbgMsg("continue fwd, CARRY");
  wheel_refresh();   move_fwd();
  DUMP_COMMAND = UP;
  while(DUMP_VAL < 350){
    wait1Msec(50);
  }
  DUMP_COMMAND = STOP;
  move_by(100, 1200);
  dbgMsg("rotate with cube");
  wheel_refresh(); rotate(SIDE*127);
  rotate_by(fabs(ang_side(-900)-DRV.pos[2])-300,1000);
  rotate_small(-SIDE*300);    // the last 30 degree more accurate, -300 go right side
  wheel_output_pos(3);
  wheel_refresh();   move_bkwd_dump(600);

  // now mv fwd a bit rotate
  // right by 90 deg
  // mv fwd by 36"
  // rotate left by 90
  // mv fwd by xx
  // back dump
    // we need move fwd, keep arm up first and then bkwd and lower arm
  // -- move fwd a bit -- may pick up some extra stars
  DUMP_COMMAND = HOLD;
  wheel_refresh(); move_fwd(); move_by(120,1000);
  DUMP_COMMAND = HOLD;  // now lower the dumper
  // -- 900 degree right rotation is done in 60/30
  wheel_refresh();  rotate(SIDE*127);  rotate_by(600,700);  // rotate left for 60 deg first
  wheel_refresh(); rotate_small(-SIDE*300);                 // use rotate small for another 300 deg
  // -- move fwd by 36"
  wheel_refresh(); move_fwd(); move_by(360,1500);  move_stop();
  // -- rotate left by 90
  wheel_refresh();  rotate(-SIDE*127);  rotate_by(600,700);  // rotate left for 60 deg first
  wheel_refresh(); rotate_small(SIDE*300);                 // use rotate small for another 300 deg

  // -- move fwd till x pos = 0 to grab and carry
  wheel_refresh(); move_fwd(); move_by(Y_POS_WALL-DRV.pos[0],1000); move_stop();
  claw(CLOSE);   DUMP_COMMAND=CARRY;  wait1Msec(50);
  // -- left rotate 90 degree
  wheel_refresh(); rotate(-SIDE*127); rotate_by(600, 1000);
  wheel_refresh(); rotate_small(SIDE*300);                 // use rotate small for another 300 deg
  // -- move back and dump  - out at arm angle = 50 deg
  // wheel_reset(DRV.pos[0],DRV.pos[1],ang_side(-900));
  wheel_refresh(); move_bkwd_dump(500); move_stop();
  wheel_reset(DRV.pos[0],Y_POS_FENCE,ang_side(-900));

  // move backward and dump
  wheel_refresh();   move_bkwd_dump(1200);   //
  move_stop();


  DUMP_MONITOR = STOP;
  WHEEL_MONITOR = STOP;
}


/*
  AUTO C: Safe autonomous
*/

void auto_C(){
   float dy = 0.;
   float dx = 0.;
   int dr = 0;
  //start auto A, we need to wait a while till .5 msec
  // for the claw to drop and afterwards, wait till
  // claw has been hold to center degree
  dbgMsg("Start auto_a");
  WHEEL_MONITOR = START;          // start time
  DUMP_MONITOR = START;          // start time
  // do auto mv1 to dump the first cube
  AUTON_MV1_ANG = 100;                  // throw at an extra 10 deg arngle to avoid the first cube
  auto_mv1();

  // try to make first grab target is x=SIDE*120, calculate angle adjustment based on math
  dbgMsg("S move lateral 15 inch to grab");
  // -- predict where the claw will end up with and rotate to the correct trajectory
  dy = (DRV.pos[1]-Y_POS_WALL);
  dx = DRV.pos[0]+dy*cos(DRV.pos[2]/DRV.pi2d)-SIDE*120;
  dr = 1800/3.14*dx/dy;     // if positive need left rotation to correct;
                            // if negative need right rotation to correct, side independent
  wheel_refresh();  rotate_slow_by(dr, 45, 500);
  // -- move fwd and grab and move backward and dump
  wheel_refresh();  move_fwd_and_grab();
  wheel_refresh();  move_bkwd_dump(1200);     move_stop();

  // try to make second grab target is x=-SIDE*120
  wheel_refresh();   DUMP_COMMAND = HOLD;    // hold dumper to ground
  // -- predict where the claw will end up with and rotate to the correct trajectory
  dy = (DRV.pos[1]-Y_POS_WALL);
  dx = DRV.pos[0]+dy*cos(DRV.pos[2]/DRV.pi2d)+SIDE*120;
  dr = 1800/3.14*dx/dy;     // if positive need left rotation to correct;
                            // if negative need right rotation to correct, side independent
  wheel_refresh();  rotate_slow_by(dr, 45, 500);
  // -- move fwd and grab and move backward and dump
  wheel_refresh();  move_fwd_and_grab();

  // -- rotate to about 15 deg off vertical and dump
  // wheel_refresh();
  // if (SIDE==1) rotate_slow_by(-900+100-DRV.pos[2], 500);
  // if (SIDE==-1) rotate_slow_by(2700-100-DRV.pos[2], 500);

  // -- rotate to about 45 deg off vertical and dump, move backward, and rotate back and dump
  // -- a fancy s motion
  wheel_refresh(); rotate(-SIDE*127);
  if (SIDE==1) rotate_by(-900+300-DRV.pos[2], 500);
  if (SIDE==-1) rotate_by(2700-300-DRV.pos[2], 500);
  wheel_refresh();  move_bkwd();  move_by(2500, 2000);
  wheel_refresh(); rotate(SIDE*127); rotate_by(300, 500);

  move_bkwd_dump(1200);
  DUMP_COMMAND = STOP;
  move_stop();

  // finish up
  wait1Msec(100);
  DUMP_MONITOR = STOP;
  WHEEL_MONITOR = STOP;
}


/*
  AUTO D: Safe autonomous
*/


/*
  PROGRAMMING SKILLS
*/

void prg_skills() {
  WHEEL_MONITOR = START;
  DUMP_MONITOR = START;
  //int tstart = time1[T1];

  // setup startup position and time
  wheel_reset(-SIDE*360, 200, ang_side(-900));

  // initial movement - 4 stars 2 cube
  prg_mv1();
  prg_mv2();
  prg_mv3();
  prg_mv4();
  prg_mv5();
  //prg_mv6();
  go_lift();

  // get the two side stars (front and corner)
  // wheel_reset(0, Y_POS_FENCE, -900);
  /*wheel_reset(DRV.pos[0], Y_POS_FENCE, ang_side(-900));
  prg_mv4();

  // auto align for lift
  wheel_reset(DRV.pos[0], Y_POS_FENCE, ang_side(-900));
  // wheel_reset(450, Y_POS_FENCE, -900);
  prg_mv5();
  // add if lift is aligned code here
  go_lift();
  //add lift up code here
  // also add lift stop logics
  wait1Msec(100);*/
  DUMP_MONITOR = STOP;
  WHEEL_MONITOR = STOP;
}


void prg_mv1() {
  // move bkwd a bit first to avoid hit the wall and open claw
  // then continue move a bit more
  // initial position
  wheel_reset(-SIDE*360, 200, ang_side(-900));

  move_bkwd(); move_by(60, 1000);                  // move bkwd a bit first
  claw(OPEN); DUMP_COMMAND = HOLD;                 // now open claw and push claw down
  wheel_refresh(); move_by(140, 1000); move_stop();// move more to where we want
  wait1Msec(400);                                  // wait some time for dump down and ...
  // DO THE fwd and dump 3 time
  // -- 1st time 4 stars
  // -- 2nd time cube
  // -- 3rd time cube
  IS_ALIGN_FENCE = 0;               // default is not to force realign
  for ( unsigned  int irounds=0;irounds<3;irounds++) {
    // -- mv fwd and grab --
    if (BAILOUT==1) return;
    wheel_refresh();  move_fwd_and_grab();
    if (BAILOUT==1) return;

    // -- move bkwd and dump --
    if (irounds==2) IS_ALIGN_FENCE = 1;      // force realign for the third dump
    wheel_refresh(  ); move_bkwd_dump(1200); DUMP_COMMAND = HOLD;  move_stop();
    if (BAILOUT==1) return;
    if (irounds<=1) wait1Msec(200);     // for the first two times, wait some time for arm to go down

    // -- reset position --
    // if (irounds==2) {            // force realign
       // move(VOLT_PUSH_FENCE_1,0);
       // wait1Msec(500);          // no sure if it is necessary, take it out
     // }

    wheel_reset(DRV.pos[0],Y_POS_FENCE,ang_side(-900));
  }
  IS_ALIGN_FENCE = 0;
}

void prg_mv2() {
  // try to grab and score cube 3
  // -- move dist don't have to be too accurate - will have extra drift but ok
  wheel_reset(DRV.pos[0],Y_POS_FENCE,ang_side(-900));

  IS_ALIGN_FENCE = 1;

  wheel_refresh(); DUMP_COMMAND = HOLD;
  claw(OPEN);
  // move_fwd(); move_by(240,1500); move_stop();
  // move_fwd(); move_by(240,1500); // take out move_stop() to make it faster
  move_fwd(); move_by(160,1500); // take out move_stop() to make it faster
                                 // also forward a little less to avoid rubber the 2 cube

  // -- 900 degree left rotation needs to be accurate -- let us do error correction so it will end as -900
  wheel_refresh();  rotate(-SIDE*127);
  /*if (SIDE== RIGHT) rotate_by(-350-DRV.pos[2],1050); // more tricker handling rotation from wall
  if (SIDE== LEFT) rotate_by(2150-DRV.pos[2],1050);
  // rotate_by(ang_side(-300)-DRV.pos[2],700);  // rotate to -300
  // wheel_refresh(); rotate_small(SIDE*320);                 // use rotate small for another 300 deg
  // may consider move fwd a little further, rotate a little more if needed
  wheel_refresh(); rotate_small(SIDE*fabs(ang_side(-1800)-DRV.pos[2]));                 // rotate to exactly -1800 deg
    if (BAILOUT==1) return;*/
  rotate_by(800, 1000);
  rotate_small(SIDE * 100);

  // -- move fwd till x pos = -100 to grab and carry
  wheel_refresh(); move_fwd(); move_by(SIDE*(180),1500);
    if (BAILOUT==1) return;
  claw(CLOSE);   DUMP_COMMAND=CARRY_HIGH; wait1Msec(50);
  // -- move fwd till x pos = 360 (with 2 inch inertia) (change to move_brake if need to be more accurate)
  // wheel_refresh(); move_fwd(); move_by(SIDE*(500),2000); move_stop();
  wheel_refresh(); move_fwd(); move_by(SIDE*(450),2500); move_stop();
    if (BAILOUT==1) return;
  // -- 900 degree right rotation needs to be accurate

  // wheel_refresh();  rotate(SIDE*127);  rotate_by(ang_side(-600)-DRV.pos[2],700);  // rotate to -600
  // fixed the error that causes extra rotation
  wheel_refresh();  rotate(SIDE*127);
    if (BAILOUT==1) return;
  if (SIDE== RIGHT) rotate_by(-600-DRV.pos[2],700);  // rotate to -600
    if (BAILOUT==1) return;
  if (SIDE== LEFT) rotate_by(2400-DRV.pos[2],700);  // rotate to -600
    if (BAILOUT==1) return;
  wheel_refresh(); rotate_small(-SIDE*300);                 // use rotate small for another 300 deg
    if (BAILOUT==1) return;
  // -- move back and dump C3 - out at arm angle = 50 deg
  wheel_refresh(); move_bkwd_dump(500); move_stop();
    if (BAILOUT==1) return;
  // wheel_reset(DRV.pos[0],DRV.pos[1],ang_side(-900));
  wheel_reset(DRV.pos[0],Y_POS_FENCE,ang_side(-900));      // don't force realign is good idea

  // try to grab and score cube 4
  // -- we may need to check x and y position with some fine adjustment ???
  // -- move fwd  and grab  cube
  DUMP_COMMAND = HOLD;  wheel_refresh();
  move_fwd_and_grab();
    if (BAILOUT==1) return;

  //--  move bkwd and dump -- arm out asap since we need arm up first for next more
  // wheel_refresh(); move_bkwd_dump(1250); move_stop();
  wheel_refresh(); move_bkwd_dump(1250);
    if (BAILOUT==1) return;

  DUMP_COMMAND = STOP;

  // force push fence
  move(VOLT_PUSH_FENCE_1,0);
    if (BAILOUT==1) return;
  wait1Msec(500);
  move_stop();

  // wheel_reset(DRV.pos[0],DRV.pos[1],ang_side(-900));
  wheel_reset(DRV.pos[0],Y_POS_FENCE,ang_side(-900));
}

void prg_mv3() {
  // try to grab and score the center 3 stars
  // we need move fwd, keep arm up first and then bkwd and lower arm
  // 2/1 reduce mv fwd by 7 " and
  wheel_reset(DRV.pos[0],Y_POS_FENCE,ang_side(-900));
  dbgMsg("go after front mid 3 stars");
  // -- move fwd a bit
  // wheel_refresh(); move_fwd(); move_by(120,1000);
  // wheel_refresh(); move_fwd(); move_by(50,1000);
  // wheel_refresh(); move_fwd(); move_by(40,1000);
  wheel_refresh(); move_fwd(); move_by(80,1000);
  DUMP_COMMAND = STOP;  // now lower dumper
  // -- 900 degree right rotation is done in 60/30
  wheel_refresh();  rotate(SIDE*127);  rotate_by(300,600);  // rotate left for 60 deg first
  wheel_refresh(); rotate_by(300, 600);
  wheel_refresh(); rotate_small(-SIDE*300);                 // use rotate small for another 300 deg
  DUMP_COMMAND = HOLD;
  // wheel_refresh(); move_bkwd(); move_by(50,1000);  move_stop();
  wheel_refresh(); move_bkwd(); move_by(60,1000);  move_stop();
  wait1Msec (100);    // some extra time to let the claw fall down
  wheel_refresh(); move_fwd(); move_by(470,2500);        // -- let us not move_stop to make it faster
  // wheel_refresh(); move_fwd(); move_by(450,2500);        // -- let us not move_stop to make it faster
  claw(CLOSE);   DUMP_COMMAND=CARRY;  wait1Msec(50);
  // -- left rotate 90 degree
  // correct 2 errors: should be to -60 not -30; 0.7 second is not long enough when lots of stars are picked up
  // wheel_refresh(); rotate(-SIDE*127); rotate_by(ang_side(-600)-DRV.pos[2],1050);
  wheel_refresh(); rotate(-SIDE*127);
  if (SIDE== RIGHT)  rotate_by(-1200-DRV.pos[2],1050);
  if (SIDE== LEFT) rotate_by(3000-DRV.pos[2],1050);
  // wheel_refresh(); rotate(-SIDE*127); rotate_by(600,700);
  wheel_refresh(); rotate_small(SIDE*300);                 // use rotate small for another 300 deg
  // -- move back and dump  - out at arm angle = 50 deg
  // wheel_reset(DRV.pos[0],DRV.pos[1],ang_side(-900));
  wheel_refresh(); move_bkwd(); move_by(50, 500);
  wheel_refresh(); move_bkwd_dump(500); move_stop();
  wheel_reset(DRV.pos[0],Y_POS_FENCE,ang_side(-900));

  // try to grab and score the read mid 3 stars
  // -- move fwd till y=240  and grab  cube
  DUMP_COMMAND = HOLD;  wheel_refresh();
  move_fwd_and_grab();
  wait1Msec(500);
  //--  move bkwd and dump , arm out at arm=140 degree since next move takes time
  // wheel_refresh(); move_bkwd_dump(1400); move_stop();
  wheel_refresh(); move_bkwd_dump(1400);
    // force push fence
  move(VOLT_PUSH_FENCE_1,0);
  wait1Msec(500);
  move_stop();

  wheel_reset(DRV.pos[0],Y_POS_FENCE,ang_side(-900));
}

void prg_mv4()
{
  // try to score corner stars
  // we need arm up first and then lower
  dbgMsg("go after corner star");
  wheel_refresh(); DUMP_COMMAND = HOLD;
    // -- move fwd a bit
  // actually went 11" went too far, reduce to 20 inch
  // wheel_refresh(); move_fwd(); move_by(25,1000);
  wheel_refresh(); move_fwd(); move_by(35,1000);
  // -- 900 degree
  // -- left rotate 90 degree
  wheel_refresh(); rotate(-SIDE*127);
  if (SIDE== RIGHT) rotate_by(-300-DRV.pos[2],1050); // more tricker handling rotation from wall
  if (SIDE== LEFT) rotate_by(2100-DRV.pos[2],1050);
  // wheel_refresh(); rotate_small(SIDE*300);                 // use rotate small for another 300 deg
  wheel_refresh(); rotate_small(SIDE*350);                 // use rotate small for another 300 deg
  // -- move fwd till xpos equals 540 grab the first one
  wheel_refresh(); DUMP_COMMAND = HOLD;
  move_fwd(); move_by(400,2000); move_stop();   // we increase mv fwd distance just in case
  claw(CLOSE); wait1Msec(100); //DUMP_COMMAND = CARRY; wait1Msec(150);
  // -- move back a bit and right this is 45 degree attack angle
  wheel_refresh(); move_bkwd(); move_by(25,1000);
  // wheel_refresh(); move_bkwd(); move_by(50,500);
  // increase rotate angle a bit
  wheel_refresh(); rotate(SIDE*127);
  if (SIDE== RIGHT) rotate_by(-150-50-DRV.pos[2],1050); // more tricker handling rotation from wall
  if (SIDE== LEFT) rotate_by(1950+50-DRV.pos[2],1050);
  wheel_refresh(); rotate_small(-SIDE*300); move_stop();                // use rotate small for another 300 deg
  // wheel_refresh(); DUMP_COMMAND = HOLD; claw(OPEN); wait1Msec(200);
  wheel_refresh(); claw(OPEN); wait1Msec(200);
  // -- move fwd till Ypos is about 240 (1/cos(60)=1.1)
  // wheel_refresh(); move_fwd(); move_by((DRV.pos[1]-240)*11/10,2000); move_stop();
  // wheel_refresh(); move_fwd(); move_by(240,1000); move_stop();
  wheel_refresh(); move_fwd(); move_by(320,2000); move_stop();
  claw(CLOSE); DUMP_COMMAND = CARRY; wait1Msec(100);
  // -- move bkwd and dump
  // wheel_refresh(); move_bkwd(); move_by(180,1000);
  wheel_refresh(); move_bkwd(); move_by(60,1000);
  wheel_refresh(); rotate(SIDE * 127);
  if (SIDE==RIGHT && DRV.pos[2]>-600) rotate_by(-600-DRV.pos[2],1050); // more tricker handling rotation from wall
  if (SIDE== LEFT && DRV.pos[2]<2400) rotate_by(2400-DRV.pos[2],1050);
  wheel_refresh(); rotate_small(-SIDE*fabs(ang_side(-900)-DRV.pos[2])); move_stop();                // use rotate small
  // move_bkwd_dump(1400); move_stop();
  move_bkwd_dump(1400);
      // force push fence
  move(VOLT_PUSH_FENCE_1,0);
  wait1Msec(500);
  move_stop();

  // wheel_reset(DRV.pos[0],DRV.pos[1],ang_side(-900));
  wheel_reset(DRV.pos[0],Y_POS_FENCE,ang_side(-900));
}

#define DUMP_RELEASE_LIFT 900    // need adjustment
void prg_mv5()
{
  // modify it to hit walls twice to realign for lift
  // auto alignment feature
  dbgMsg(" ** prg_mv5");
  wheel_reset(DRV.pos[0],Y_POS_FENCE,ang_side(-900));

  // switch transmission, which auto wiggles
  transmission(CLOSE);

  // move fwd first till hit the wall -- total dist is about 54", let us move 46" (reach 30"/second)
  // switch to lower power and hold for certain time
  dbgMsg("align for lift");
  // wheel_refresh(); move_fwd(); move_by(DRV.pos[1]-450,1200); move_stop();
  wheel_refresh(); move_fwd(); move_by(460,1800);
  move(60,0); wait1Msec(600); //use small voltage and hold .6 second   to force alignment
  wheel_reset(DRV.pos[0], 0, ang_side(-900));     // reset position to force Y value to be zero

  //move slightly bkwd and rotate 90 deg to either 180 or 0 degree
  // this 90 deg rotation needs to be accurate
  // rotation of 90 deg was ok byt then
  wheel_refresh(); move_back_slow_by(60, 500);          // slight move back 4"   ypos is now 5" due to inertia
   wheel_refresh(); rotate(-SIDE*127);
  //wheel_refresh();
  rotate_by(300, 500);
  //if (SIDE==1) rotate_by(-300-DRV.pos[2], 1000);
  //if (SIDE==-1) rotate_by(2100-DRV.pos[2], 1000);
  wheel_refresh(); rotate_small(SIDE*300);                 // use rotate small for another 300 deg
  wheel_refresh(); rotate_slow_by(SIDE*300, 45, 1500);                 // use rotate small for another 300 deg
  // need to add move stop -- otherwise small voltage braking cause twisting in angle
  move_stop();
  //let us wiggnel one more time if needed
  if(DUMP_VAL < DUMP_RELEASE_LIFT) DUMP_COMMAND=WIGGLE;

  // also popup the lift early here
  // -- the LIFT VAL in lift up may not be correct -- check it up
  lift_up();


  // move fwd and to auto align on the wall -- the timing
  // wheel_refresh(); move_fwd(); move_by(720-DRV.pos[0], 1200);
  wheel_refresh(); move_fwd(); move_by(240, 1200);      // may need adjustment
  move(60,0); wait1Msec(500); //use small voltage and hold .5 second
  // wheel_reset(0, DRV.pos[1], ang_side(0));          // reset position again
  // error should be -180/180 deg not 0 or 360
  wheel_reset(0, DRV.pos[1], ang_side(-1800));          // reset position again

  // we may consider a strategy of slightly off and hit with an angle
  //move slightly bkwd and rotate 135 deg to either 450 or 1350
  // wheel_refresh(); move_back_slow_by(50, 500);          // slight move back 2"   ypos is now 2"
  // wheel_refresh(); move_back_slow_by(DRV.pos[1]+30, 500);          // slight move back 2"   ypos is now 2"
  wheel_refresh(); move_back_slow_by(DRV.pos[1]+10, 1000);          // slight move back 2"   ypos is now 2"
  // -- we want X=Y, some how move back and rotate like this X is too small, add 2" here
  // need to rotate 1350 degree; let us do 60 deg fast, 30 degree small and 450 deg slow
  // wheel_refresh(); rotate(-SIDE*127);
  // rotate_by(1050, 1500);
  // wheel_refresh(); rotate_small(SIDE*300);                 // use rotate small for another 300 deg
  wheel_refresh();
  //rotate_slow_by(SIDE*fabs(450-DRV.pos[2]),45,1000);                // rotate very slow with 45 vcmd
  wheel_refresh(); rotate(-SIDE*127);
 //wheel_refresh();
 rotate_by(750, 1000);
 //if (SIDE==1) rotate_by(-300-DRV.pos[2], 1000);
 //if (SIDE==-1) rotate_by(2100-DRV.pos[2], 1000);
 wheel_refresh(); rotate_small(SIDE*300);                 // use rotate small for another 300 deg
 wheel_refresh(); rotate_slow_by(SIDE*fabs(450-DRV.pos[2]), 45, 1500);
  // We should expect DRV.pos to be x=y, r=-1350/2250

  //let us wiggle one more time if needed
  if(DUMP_VAL < DUMP_RELEASE_LIFT) DUMP_COMMAND=WIGGLE;

  // move bkwd and align -- first mv bkwd by 9"
  wheel_refresh(); move_bkwd(); move_by(90, 800);      // may need adjustment
  // we could us the X,Y,R at the point to fine adjustment
  move(-60,0); wait1Msec(600); //use small voltage and hold .5 second
  DUMP_COMMAND=STOP;
  move_stop();

  // now ready to lift
  lift_down();
}

void prg_mv6() {
  // try go all the way back and grab two stars far corner; do one dump in the middle
  wheel_reset(DRV.pos[0],Y_POS_FENCE,ang_side(-900));
  // -- move fwd a bit
  DUMP_COMMAND = STOP;  // now lower dumper
  // wheel_refresh(); move_fwd(); move_by(75,1000);
  wheel_refresh(); move_fwd(); move_by(60,1000);
  // -- 900 degree right rotation is done in 60/30
  wheel_refresh();  rotate(SIDE*127);  rotate_by(300,600);  // rotate left for 60 deg first
  DUMP_COMMAND = HOLD;
  wheel_refresh(); rotate_by(300, 600);
  wheel_refresh(); rotate_small(-SIDE*300);                 // use rotate small for another 300 deg
  // -- move bkwd till x pos = - to grab and carry  500 might not be accurate, just do 12"
  wheel_refresh(); move_fwd(); move_by(480,2500);  move_stop();
  claw(CLOSE);   DUMP_COMMAND=CARRY;  wait1Msec(50);
  wheel_refresh(); rotate(-SIDE*127); rotate_by(ang_side(-600)-DRV.pos[2],1050);
  wheel_refresh(); rotate_small(SIDE*300);                 // use rotate small for another 300 deg
  wheel_refresh(); move_bkwd_dump(600); move_stop();
  wheel_reset(DRV.pos[0],Y_POS_FENCE,ang_side(-900));

  // another fwd/bkwd  wheel_reset(DRV.pos[0],Y_POS_FENCE,ang_side(-900));

  // try to grab and score mid  stars
  DUMP_COMMAND = HOLD;  wheel_refresh();
  move_fwd_and_grab();
  //--  move bkwd and dump , arm out at arm=140 degree since next move takes time
  wheel_refresh(); move_bkwd_dump(1400); move_stop();
  wheel_reset(DRV.pos[0],Y_POS_FENCE,ang_side(-900));

  // try the same thing again
  DUMP_COMMAND = STOP;  // now lower dumper
  wheel_refresh(); move_fwd(); move_by(50,1000);
  // -- 900 degree right rotation is done in 60/30
  wheel_refresh();  rotate(SIDE*127);  rotate_by(300,600);  // rotate left for 60 deg first
  DUMP_COMMAND = HOLD;
  wheel_refresh(); rotate_by(300, 600);
  wheel_refresh(); rotate_small(-SIDE*300);                 // use rotate small for another 300 deg
  // -- move bkwd till x pos = - to grab and carry  500 might not be accurate, just do 12"
  wheel_refresh(); move_fwd(); move_by(360,2000);  move_stop();
  claw(CLOSE);   DUMP_COMMAND=CARRY;  wait1Msec(150);

  wheel_refresh(); move_bkwd(); move_by(25,500);
  // wheel_refresh(); move_bkwd(); move_by(50,500);
  // increase rotate angle a bit
  wheel_refresh(); rotate(-SIDE*127);
  if (SIDE== 1) rotate_by(-1050+50-DRV.pos[2],1050); // more tricker handling rotation from wall
  if (SIDE==-1) rotate_by(2850-50-DRV.pos[2],1050);
  wheel_refresh(); rotate_small(SIDE*300); move_stop();                // use rotate small for another 300 deg
  wheel_refresh(); DUMP_COMMAND = HOLD; claw(OPEN); wait1Msec(200);
  // -- move fwd till Ypos is about 240 (1/cos(60)=1.1)
  // wheel_refresh(); move_fwd(); move_by((DRV.pos[1]-240)*11/10,2000); move_stop();
  // wheel_refresh(); move_fwd(); move_by(240,1000); move_stop();
  wheel_refresh(); move_fwd(); move_by(320,1600); move_stop();
  claw(CLOSE); DUMP_COMMAND = CARRY; wait1Msec(100);
  // -- move bkwd and dump
  // wheel_refresh(); move_bkwd(); move_by(180,1000);
  wheel_refresh(); move_bkwd(); move_by(60,1000);
  wheel_refresh(); rotate(-SIDE * 127);
  if (SIDE== 1 && DRV.pos[2]<-1100) rotate_by(-1100-DRV.pos[2],1050); // more tricker handling rotation from wall
  if (SIDE==-1 && DRV.pos[2]>2900) rotate_by(2900-DRV.pos[2],1050);
  wheel_refresh(); rotate_small(-SIDE*fabs(ang_side(-900)-DRV.pos[2])); move_stop();                // use rotate small
  move_bkwd_dump(1400); move_stop();
  // wheel_reset(DRV.pos[0],DRV.pos[1],ang_side(-900));
  wheel_reset(DRV.pos[0],Y_POS_FENCE,ang_side(-900));

}

#endif //SS_AUTON
