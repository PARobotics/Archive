#ifndef SS_FUNC_C
#define SS_FUNC_C
#include "ss.h"

/*
	FUNCTION.C
	For general functions
*/



// int is_bailed_out()
// {
  // if (vexRT[Btn7D] == 1 || vexRT[Btn7DXmtr2] ==  1){
    // return 1;
  // }
  // return 0;
// }

int is_timed_out(int tstop)
{
  if (time1[T1]>=tstop)
  {
    return 1;
  }
  return 0;
}

//Sensor Functions

int volt1() { //Returns voltage of main battery in millivolts
  return nImmediateBatteryLevel;
}

int volt2() {
  return SensorValue(Power2)*1000/286;
}

void getDumpVal() {
  //DUMP_VAL = ((450 * (DUMP_MIN_REAL - SensorValue[FORK_ANGLE]))/512);  // in 1/10 degree
	DUMP_VAL = (450 * (DUMP_MIN_REAL - SensorValue[FORK_ANGLE]))/512;
}

void getClawVal() {
  CLAW_VAL_L = SensorValue(CLAW_L);
  CLAW_VAL_R = SensorValue(CLAW_R);
}

void getWheelVal() {
  // 392 tick = pi*3.25 gives 0.026" per tick
  // it was then x 1sqrt(2) to get the displacement from either x or y disp
  X_disp =  (nMotorEncoder[M_WHEEL_FR]-nMotorEncoder[M_WHEEL_FL]) * 1.84*1.6/ 2;  // x is lateral
  Y_disp = - (nMotorEncoder[M_WHEEL_FR]+nMotorEncoder[M_WHEEL_FL]) *1.84 *1.6/ 2;  // y is frontal
  //R_disp  = -SensorValue(G_SENSOR);                                      // gyro in 1/10 deg

  //writeDebugStreamLine("%% %5d %5d", nMotorEncoder[M_WHEEL_FL], nMotorEncoder[M_WHEEL_FR]);
  //writeDebugStreamLine("%% %5d %5d %5d", X_disp, Y_disp, R_disp);
}

//Motion control/Motor functions

void move(int V,int H, int X)
{
	if(abs(V) > abs(X)){
		X = 0;
	}
	else if(abs(V) < abs(X)){
		V = 0;
	}

	motor[M_WHEEL_FL] = V + H - X;
	motor[M_WHEEL_BL] = V + H + X;
	motor[M_WHEEL_FR] = V - H + X;
	motor[M_WHEEL_BR] = V - H - X;
}

//Control Drive
void move_y(int vol){
// by pass slew rate
	/*motorReq[M_WHEEL_FL]  =  vol;
	motorReq[M_WHEEL_BL]  =  vol;
	motorReq[M_WHEEL_FR]  =  vol;
	motorReq[M_WHEEL_BR]  =  vol;
	motor[M_WHEEL_FL]  =  vol;
	motor[M_WHEEL_BL]  =  vol;
	motor[M_WHEEL_FR]  =  vol;
	motor[M_WHEEL_BR]  =  vol;*/
}

void move_x( int vol){
// by pass slew rate
	/*motorReq[M_WHEEL_FL]  =   vol;
	motorReq[M_WHEEL_BL]  =  -vol;
	motorReq[M_WHEEL_FR]  =  -vol;
	motorReq[M_WHEEL_BR]  =   vol;
	motor[M_WHEEL_FL]  =   vol;
	motor[M_WHEEL_BL]  =  -vol;
	motor[M_WHEEL_FR]  =  -vol;
	motor[M_WHEEL_BR]  =   vol;*/
}

void rotate(int vol){ // Left = -1, Right = 1
	/*motorReq[M_WHEEL_FL] = -vol;
	motorReq[M_WHEEL_BL] = -vol;
	motorReq[M_WHEEL_FR] =  vol;
	motorReq[M_WHEEL_BR] =  vol;
	motor[M_WHEEL_FL] = -vol;
	motor[M_WHEEL_BL] = -vol;
	motor[M_WHEEL_FR] =  vol;
	motor[M_WHEEL_BR] =  vol;*/
}

void move_x_for(int vol, int tlimit){
  move_x(vol);
  int tnow = time1[T1];
  while(true){
    if(is_timed_out(tnow + tlimit)){
      move_x(STOP);
      break;
    }
    wait1Msec(20);
  }
}

void move_y_for(int vol, int tlimit){
  move_y(vol);
  int tnow = time1[T1];
  while(true){
    if(is_timed_out(tnow + tlimit)){
      move_y(STOP);
      break;
    }
    wait1Msec(20);
  }
}

void rotate_for(int vol, int tlimit){
  rotate(vol);
  int tnow = time1[T1];
  while(true){
    if(is_timed_out(tnow + tlimit)){
      rotate(STOP);
      break;
    }
    wait1Msec(20);
  }
}



int move_x_by(int dist, int tlimit){ //Strafe by a certain distance
  //Dist is in 0.01 inch
  getWheelVal();
  WHEEL_MONITOR = START;

  int dir = SIGN(dist);
  int target = X_disp+dist;
  int tnow0 = time1[T1];
  int to_target;
  //writeDebugStreamLine("%%Starting move");
  //writeDebugStreamLine("%%Target = %4d", target);
  while(true){
    if ( is_timed_out(tnow0+tlimit)==1){
      writeDebugStreamLine("MOVE_END");
      move_x(STOP);
      WHEEL_MONITOR = STOP;
      return 0;
    }

    to_target = abs(target - X_disp);
    //writeDebugStreamLine("%% Y: %4d %4d %4d", target, to_target, X_disp);

    if(to_target > MOVEX_DIST1){
      MOVEX_VCMD = MOVEX_V1;
      move_x(dir * MOVEX_VCMD);
      //writeDebugStreamLine("%%VCMD = %3d", MOVEY_VCMD);
    }
    else if(to_target > MOVEX_DIST0){
      MOVEX_VCMD = dir * vcmd_ctrl_linear_band(
       to_target, X_vel,
       MOVEX_DIST1, MOVEX_DIST0, MOVEX_Y1, MOVEX_Y2, MOVEX_DY, MOVEX_KV,
       MOVEX_V_DEF, MOVEX_V_MIN, MOVEX_V_MAX);
       move_x(MOVEX_VCMD);
       //writeDebugStreamLine("%%VCMD2 = %3d", MOVEY_VCMD);
    }
    else{
      MOVEX_VCMD = MOVEX_BRAKE_V * dir;
      move_x(MOVEX_VCMD);
      wait1Msec(MOVEX_DT);
      move_x(STOP);
      WHEEL_MONITOR = STOP;
      writeDebugStreamLine("%%WHEEL_MONITOR_END");
      return 1;
    }
    wait1Msec(2);
  }
  return 1;
}

int move_y_by(int dist, int tlimit){ //Move fwd/backwards by a certain distance
  //Dist is in 0.01 inch
  getWheelVal();
  WHEEL_MONITOR = START;

  // why negative SIGN?????
  int dir = -SIGN(dist);
  int target = Y_disp+dist;
  int tnow0 = time1[T1];
  int to_target;
  //writeDebugStreamLine("%%Starting move");
  //writeDebugStreamLine("%%Target = %4d", target);
  while(true){
    if (is_timed_out(tnow0+tlimit)==1){
      writeDebugStreamLine("MOVE_END");
      move_y(STOP);
      WHEEL_MONITOR = STOP;
      return 0;
    }

    to_target = abs(target - Y_disp);
    //writeDebugStreamLine("%% Y: %4d %4d %4d", target, to_target, Y_disp);

    if(to_target > MOVEY_DIST1){
      MOVEY_VCMD = MOVEY_V1;
      move_y(dir * MOVEY_VCMD);
      //writeDebugStreamLine("%%VCMD = %3d", MOVEY_VCMD);
    }
    else if(to_target > MOVEY_DIST0){
      MOVEY_VCMD = dir * vcmd_ctrl_linear_band(
       to_target, Y_vel,
       MOVEY_DIST1, MOVEY_DIST0, MOVEY_Y1, MOVEY_Y2, MOVEY_DY, MOVEY_KV,
       MOVEY_V_DEF, MOVEY_V_MIN, MOVEY_V_MAX);
       move_y(MOVEY_VCMD);
       //writeDebugStreamLine("%%VCMD2 = %3d", MOVEY_VCMD);
    }
    else{
      MOVEY_VCMD = MOVEY_BRAKE_V * dir;
      move_y(MOVEY_VCMD);
      wait1Msec(MOVEY_DT);
      move_y(STOP);
      WHEEL_MONITOR = STOP;
      writeDebugStreamLine("%%WHEEL_MONITOR_END");
      return 1;
    }
    wait1Msec(2);
  }
  return 1;
}


int rotate_by(int angle, int tlimit){ //Rotate a certain amount of degrees (Counterclockwise is positive)
  //Angle in 1/10 degree
  getWheelVal();
  WHEEL_MONITOR = START;
  int dir = SIGN(angle);
  int target = R_disp+angle;
  int tnow0 = time1[T1];
  int to_target;

  while (true){
    if (is_timed_out(tnow0+tlimit)==1){
      writeDebugStreamLine("MOVE_END");
      rotate(STOP);
      return 0;
    }

    to_target = abs(target - R_disp);

    if(to_target > ROTATE_ANG1){
      ROTATE_VCMD = ROTATE_V1 * dir;
      rotate(ROTATE_VCMD);
    }
    else if(to_target > ROTATE_ANG0){
      ROTATE_VCMD = dir * vcmd_ctrl_linear_band(
       to_target, R_vel,
       ROTATE_ANG1, ROTATE_ANG0, ROTATE_Y1, ROTATE_Y2, ROTATE_DY, ROTATE_KV,
       ROTATE_V_DEF, ROTATE_V_MIN, ROTATE_V_MAX);
       rotate(ROTATE_VCMD);
    }
    else{
      ROTATE_VCMD = ROTATE_BRAKE_V * dir;
      rotate(ROTATE_VCMD);
      wait1Msec(ROTATE_BRAKE_DT);
      rotate(STOP);
      WHEEL_MONITOR = STOP;
      writeDebugStreamLine("%%WHEEL_MONITOR_END");
      return 1;
    }
    wait1Msec(2);
  }
  return 1;

}

int move_x_by_fast(int dist, int tlimit){ //Move fwd/backwards by a certain distance without braking
  //Dist is in 0.01 inch
  getWheelVal();
  WHEEL_MONITOR = START;

  int dir = SIGN(dist);
  int target = X_disp+dist;
  int tnow0 = time1[T1];
  int to_target;

  while(true){
    if (is_timed_out(tnow0+tlimit)==1){
      writeDebugStreamLine("MOVE_END");
      move_x(STOP);
      WHEEL_MONITOR = STOP;
      return 0;
    }

    to_target = abs(target - X_disp);

    if(to_target > 0){
      move_x(dir * MOVEX_V1);
    }
    else{
      move_x(STOP);
      return 1;
    }
    wait1Msec(2);
  }
  return 1;
}

int move_y_by_fast(int dist, int tlimit){ //Move fwd/backwards by a certain distance without braking
  //Dist is in 0.01 inch
  getWheelVal();
  WHEEL_MONITOR = START;

  int dir = -SIGN(dist);
  int target = Y_disp+dist;
  int tnow0 = time1[T1];
  int to_target;

  while(true){
    if ( is_timed_out(tnow0+tlimit)==1){
      writeDebugStreamLine("MOVE_END");
      move_y(STOP);
      WHEEL_MONITOR = STOP;
      return 0;
    }

    to_target = abs(target - Y_disp);

    if(to_target > 0){
      move_y(dir * MOVEY_V1);
    }
    else{
      move_y(STOP);
      return 1;
    }
    wait1Msec(2);
  }
  return 1;
}

int rotate_by_fast(int dist, int tlimit){ //Move fwd/backwards by a certain distance without braking
  //Dist is in 0.1 degrees
  getWheelVal();
  WHEEL_MONITOR = START;

  int dir = SIGN(dist);
  int target = R_disp+dist;
  int tnow0 = time1[T1];
  int to_target;

  while(true){
    if (is_timed_out(tnow0+tlimit)==1){
      writeDebugStreamLine("MOVE_END");
      rotate(STOP);
      WHEEL_MONITOR = STOP;
      return 0;
    }

    to_target = abs(target - R_disp);

    if(to_target > 0){
      rotate(dir * MOVEY_V1);
    }
    else{
      rotate(STOP);
      return 1;
    }
    wait1Msec(2);
  }
  return 1;
}

#endif //SS_FUNC_C
