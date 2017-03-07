#ifndef SS_MOVE_C
#define SS_MOVE_C
//#include "ss.h"

#define DEFAULT_ROT_VCMD 127
#define ROTATE_BRAKE 20
#define ROTATE_BRAKE_TIME 60


//Motion control/Motor functions

void move(int V, int H) {
  //Basic movement in all directions
  //V = Voltage of F/B
  //H = Voltage of L/R Turn
  int v1 = BOUND(V + H, -127, 127);
  int v2 = BOUND(V - H, -127, 127);
  motorReq[M_DRV_FL]  = v1;
  motorReq[M_DRV_BL]  = v1;
  motorReq[M_DRV_FR]  = v2;
  motorReq[M_DRV_BR]  = v2;
}

void move_force(int V, int H) {
  //Basic movement in all directions
  //V = Voltage of F/B
  //H = Voltage of L/R Turn
  // +H; rotate right

  int v1 = BOUND(V + H, -127, 127);
  int v2 = BOUND(V - H, -127, 127);
  motorReq[M_DRV_FL]  = v1;
  motorReq[M_DRV_BL]  = v1;
  motorReq[M_DRV_FR]  = v2;
  motorReq[M_DRV_BR]  = v2;
  motor[M_DRV_FL]  = v1;
  motor[M_DRV_BL]  = v1;
  motor[M_DRV_FR]  = v2;
  motor[M_DRV_BR]  = v2;
}

void move_stop() {
   move(0,0); //Tell slew to stop and then manually stop
   motor[M_DRV_FL] = 0;
   motor[M_DRV_BL] = 0;
   motor[M_DRV_FR] = 0;
   motor[M_DRV_BR] = 0;
}

void move_fwd(){ move(127,0);}
void move_bkwd(){ move(-127,0); }
void rotate(int vol){ move(0, vol); }

//Control Drive
void move_back_slow_by(int dist, int tlimit) { //
  move_force(-60,0);
  int tnow0 = time1[T1];
  while (true) {
    if (BAILOUT==1) return;
    if (is_timed_out(tnow0 + tlimit) == 1) {
      break;
    }
    if (fabs(DRV.move[0])>=fabs(dist) ) {
      break;
    }
    wait1Msec(2);
  }
  move_force(20,0);
  wait1Msec(30);
  move_stop();
}

void move_curve(int level){
  if (level==0) move(127,0);
  else if (level==1) move(127,15);
  else if (level==2) move(127,25);
  else if (level==3) move(127,35);
  else if (level==4) move(127,45);
  else if (level==5) move(127,55);
  else if (level==6) move(127,65);
  else if (level==7) move(127,75);
  else if (level==8) move(127,85);
  else if (level==9) move(127,95);
  else if (level==10) move(127,127);
  else if (level==-1) move(127,-15);
  else if (level==-2) move(127,-25);
  else if (level==-3) move(127,-35);
  else if (level==-4) move(127,-45);
  else if (level==-5) move(127,-55);
  else if (level==-6) move(127,-65);
  else if (level==-7) move(127,-75);
  else if (level==-8) move(127,-85);
  else if (level==-9) move(127,-95);
  else if (level==-10) move(127,-127);
}

void move_curve_bkwd(int level){
  if (level==0) move(-127,0);
  else if (level==1) move(-127,15);
  else if (level==2) move(-127,25);
  else if (level==3) move(-127,35);
  else if (level==4) move(-127,45);
  else if (level==5) move(-127,55);
  else if (level==6) move(-127,65);
  else if (level==7) move(-127,75);
  else if (level==8) move(-127,85);
  else if (level==9) move(-127,95);
  else if (level==10) move(-127,127);
  else if (level==-1) move(-127,-15);
  else if (level==-2) move(-127,-25);
  else if (level==-3) move(-127,-35);
  else if (level==-4) move(-127,-45);
  else if (level==-5) move(-127,-55);
  else if (level==-6) move(-127,-65);
  else if (level==-7) move(-127,-75);
  else if (level==-8) move(-127,-85);
  else if (level==-9) move(-127,-95);
  else if (level==-10) move(-127,-127);
}

//Control Drive
void move_by(int dist, int tlimit) { //
  int tnow0 = time1[T1];
  while (true) {
    if (BAILOUT==1) return;

    if (is_timed_out(tnow0 + tlimit) == 1) {
      return;
    }

    if (fabs(DRV.move[0])>=fabs(dist) || (ACC_MONITOR == START && COLLIDED == YES)) {
      return;
    }
    wait1Msec(2);
  }
}

//Control Drive
void rotate_by(int ang, int tlimit) { //
  // wheel_refresh();    // DRV_move -> 0
  int tnow0 = time1[T1];
  while (true) {
    if (BAILOUT==1) return;

    if (is_timed_out(tnow0 + tlimit) == 1) {
      // writeDebugStream("rotate_by out");
      // move_stop();
      return;
    }
    if (fabs(DRV.move[2])>=fabs(ang)) {
      // move_stop();
      return;
    }
    wait1Msec(2);
  }
}

//Control Drive
void rotate_slow_by(int ang, int vol, int tlimit) { //
  if (fabs(ang)<=30) return;     // ignore if smaller than 3 degree error
  rotate(-SIGN(ang)*vol); // if positive left rotation
                            // if negative need right rotation to correct, side independent
  // wheel_refresh();    // DRV_move -> 0
  int tnow0 = time1[T1];
  while (true) {
    if (is_timed_out(tnow0 + tlimit) == 1) {
      // writeDebugStream("rotate_by out");
      move_stop();
      return;
    }
    if (fabs(DRV.move[2])>=fabs(ang)) {
      move_force(0,SIGN(ang)*ROTATE_BRAKE);                  // brake at -20
      wait1Msec(ROTATE_BRAKE_TIME);
      move_stop();
      return;
    }
    wait1Msec(2);
  }
}

void rotate_small (int angle) {
  // rotate_small(-300) rotate right for 300 degree
  // used to make small angle adjustment < 20 degree

  wheel_refresh();
  // get the rotating direction set
  int dir = -SIGN(angle);
  // angle = dir*angle;
  angle = fabs(angle);    // abs value of angle

  int vcmd, rpm;
  int tnow0 = time1[T1];
  while (true) {
    if (is_timed_out(tnow0 + 600) == 1) {   // cannot take more than 0.5 sec
      move_stop();
      return;
    }

    rpm = fabs(DRV.speed[1]);      // assuming it takes 80 msec to brake rpm*.04 is about how
                                   // much it will rotate after the brake is started
    if (fabs(DRV.move[2])>angle-rpm/25 ) {  // brake take 0.08 second
      move_force(0,-dir*ROTATE_BRAKE);                  // brake at -20
      wait1Msec(ROTATE_BRAKE_TIME);
      return;
    }

    if (rpm>100) vcmd = -ROTATE_BRAKE;         // apply some break if speed too faster ...
    // if (rpm>1000) vcmd = -ROTATE_BRAKE;         // error in rotate_small

    if (rpm>600) vcmd = 0;         // limit to 60 deg/second
    else vcmd = 40+(600-rpm)*14/100;  // vcmd from 40 to 127 when speed is below 60dps
    move_force(0,dir*vcmd);
    wait1Msec(2);
  }
}


/*
void rotate_brake(int angle, int tlimit) {
//Rotate a certain amount of degrees (Counterclockwise is positive)
  wheel_refresh();
  int tnow0 = time1[T1];
  float R_vel, to_target;
  int dir = -SIGN(angle);
  while (true) {
    if (is_timed_out(tnow0 + tlimit) == 1) {
      // writeDebugStreamLine("Rotate_brake");
      move_stop();
      return;
    }

    R_vel = fabs(DRV.speed[1]);
    to_target = fabs(angle - DRV.move[2]);

    if (to_target > ROTATE_ANG1) {
      ROTATE_VCMD = ROTATE_V1 * dir;
      move_force(0,ROTATE_VCMD);
    }
    else if (to_target > ROTATE_ANG0) {
      ROTATE_VCMD = dir * vcmd_ctrl_linear_band(
                    to_target, R_vel,
                    ROTATE_ANG1, ROTATE_ANG0, ROTATE_Y1, ROTATE_Y2, ROTATE_DY, ROTATE_KV,
                    ROTATE_V_DEF, ROTATE_V_MIN, ROTATE_V_MAX);
      move_force(0,ROTATE_VCMD);
    }
    else {
      ROTATE_VCMD = ROTATE_BRAKE_V * dir;
      move_force(0,ROTATE_VCMD);
      wait1Msec(ROTATE_BRAKE_DT);
      rotate(STOP);
      return;
    }
    wait1Msec(2);
  }
}
*/


void move_brake(int dist, int tlimit) { //Move fwd/backwards by a certain distance

  int dir = SIGN(dist);
  int tnow0 = time1[T1];
  float Y_vel, to_target;

  while (true) {
    if (is_timed_out(tnow0 + tlimit) == 1) {
      move_stop();
      return ;
    }
    to_target = fabs(dist - DRV.move[0]);
    Y_vel = fabs(DRV.speed[0]);

    if (to_target > MOVEY_DIST1) {
      MOVEY_VCMD = MOVEY_V1;
      move_force(dir * MOVEY_VCMD,0);
      //writeDebugStreamLine("%%VCMD = %3d", MOVEY_VCMD);
    }
    else if (to_target > MOVEY_DIST0) {
      MOVEY_VCMD = dir * vcmd_ctrl_linear_band(
                     to_target, Y_vel,
                     MOVEY_DIST1, MOVEY_DIST0, MOVEY_Y1, MOVEY_Y2, MOVEY_DY, MOVEY_KV,
                     MOVEY_V_DEF, MOVEY_V_MIN, MOVEY_V_MAX);
      move_force(MOVEY_VCMD,0);
      //writeDebugStreamLine("%%VCMD2 = %3d", MOVEY_VCMD);
    }
    else {
      MOVEY_VCMD = MOVEY_BRAKE_V * dir;
      move_force(MOVEY_VCMD,0);
      wait1Msec(MOVEY_DT);
      move_stop();
      return;
    }
    wait1Msec(2);
  }
  return;
}
#endif //SS_MOVE_C
