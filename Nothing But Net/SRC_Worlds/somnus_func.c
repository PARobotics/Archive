#ifndef SOMNUS_FUNC_C
#define SOMNUS_FUNC_C

///////////////////////////////////////////////////////////////////////////////
// Utilities functions
///////////////////////////////////////////////////////////////////////////////

int sign (int x) {          // sign function implementation
  if (x > 0) return 1;
  if (x < 0) return -1;
  return 0;
}

int bound (int a, int amin, int amax) {          // bound a in [amin, amax]
  if (a<=amin) {a = amin;}
  if (a>=amax) {a = amax;}
  return a;
}

int is_bailed_out()
{
  // button 7D is the magic bail out button
  if (vexRT[Btn7D] == 1)
  {
    return 1;
  }
  return 0;
}

int is_timed_out(int tstop)
{
  if (time1[T1]>=tstop)
  {
    return 1;
  }
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Sensor value functions
///////////////////////////////////////////////////////////////////////////////
// -- voltage of second battery in mv
int volt1() {
  return nImmediateBatteryLevel;
}
int volt2() {
  return SensorValue(Power2)*1000/286;
  // return SensorValue(Power2)*1000/280;
}
//void getGyroVal() {
// Gyro_val  = SensorValue(G_SENSOR);
//}
void getWheelVal() {
  // Wheel_val = -nMotorEncoder[M_WHEEL_L_1]*2; // 0.005 inch per tick, in .01 inch
  Wheel_val_L = -nMotorEncoder[M_WHEEL_L]*3.2; // 0.005 inch per tick, in .01 inch, scaling by 1.6 since overclock
  // Wheel_val_L = nMotorEncoder[M_WHEEL_L_1]*3.2; // 0.005 inch per tick, in .01 inch, scaling by 1.6 since overclock
  Wheel_val_R = -nMotorEncoder[M_WHEEL_R]*3.2; // 0.005 inch per tick, in .01 inch, scaling by 1.6 since overclock
  Wheel_val = (Wheel_val_L+Wheel_val_R)/2;
  Gyro_val  = -SensorValue(G_SENSOR);
  // Gyro_val  = SensorValue(G_SENSOR);
}
void getIntakerVal() {
  // Intaker_val  = nMotorEncoder[M_INTAKER_L]*360/RPM_393_HS; // in deg
  Intaker_val  = nMotorEncoder[M_INTAKER_L]*5/4; // in .01 inch
}
void getFeederVal() {
  Feeder_val = -nMotorEncoder[M_FEEDER]*360/RPM_393_HS; // in deg
}
void getBatVal() {
  // Bat_val = SensorValue(SE_BAT)*10;          // in 1/10 degree
  Bat_val = SensorValue(SE_BAT);          // in deg
                                          //*10 cause trouble in memory if called too ast
}
void getUS0Val() {
  US0_val = SensorValue(US_BALL_0);          // in mm
}
void getUS1Val() {
  US1_val = SensorValue(US_BALL_1);          // in mm
}

///////////////////////////////////////////////////////////////////////////////
// check 2 ball positions
///////////////////////////////////////////////////////////////////////////////

int is_loaded() {
  Ball0 = 0;
  getUS0Val();
  if(US0_val <= US0_DMAX && US0_val >= US0_DMIN) {
    // #if DEBUG_BALL == YES
      // writeDebugStreamLine("BALL loaded %d",time1[T1]);
    // #endif
    Ball0 = 1;
  }
  return Ball0;
}

int is_queued() {
  Ball1 = 0;
  getUS1Val();
  if(US1_val <= US1_DMAX && US1_val >= US1_DMIN) {
    // #if DEBUG_BALL == YES
      // writeDebugStreamLine("BALL queued %d",time1[T1]);
    // #endif
    Ball1 = 1;
  }
  return Ball1;
}



///////////////////////////////////////////////////////////////////////////////
// basic motion control/motor functions
///////////////////////////////////////////////////////////////////////////////

//move_x
// ** we are not putting anything that may correct the drifing effects
void move_x(int vol){
  int vol_left  = bound(vol+MOVE_X_DV,-127,127);
  int vol_right = bound(vol-MOVE_X_DV,-127,127);
  motor [ M_WHEEL_L ] =  vol_left;
  // motor [ M_WHEEL_L_2 ] =  vol;
  motor [ M_WHEEL_R ] =  vol_right;
  // motor [ M_WHEEL_R_2 ] =  vol;
  Wheel_volt = vol;
}


//rotate
void rotate(int  vol){ // LEFT = -1 RIGHT = 1
  motor [ M_WHEEL_L ] =  vol;
  // motor [ M_WHEEL_L_2 ] =  vol;
  motor [ M_WHEEL_R ] =  -vol;
  // motor [ M_WHEEL_R_2 ] =  -vol;
  Wheel_volt = vol;
}


void bat(int vol) {
  motor[M_BAT_1] = vol;
  motor[M_BAT_2] = vol;
  motor[M_BAT_3] = vol;
  motor[M_BAT_4] = vol;
  motor[M_BAT_5] = vol;
  Bat_volt = vol;
}

void intaker(int vol){
  motor [ M_INTAKER_L ] =  vol;
  motor [ M_INTAKER_R ] =  vol;
  Intaker_volt = vol;
}

void feeder(int vol){
  motor [ M_FEEDER ] =  vol;
  Feeder_volt = vol;
}




///////////////////////////////////////////////////////////////////////////////
// ADVANCED MOVEMENT LOGICS
///////////////////////////////////////////////////////////////////////////////

int move_x_by_slow(int d, int tlimit)
{
  // move x slowly by distance d in time limit tlinmit

  getWheelVal();
  int dir = sign(d);
  int target = Wheel_val+d;
  int tnow0 = time1[T1];
  move_x(dir*MOVE_X_SLOW_VOLT);
  while ( true)
  {
    if (is_bailed_out()==1 || is_timed_out(tnow0+tlimit)==1)          {
      #if DEBUG_BAT>0
       writeDebugStreamLine("%%MOVE_X_BY_SLOW_BAILOUT");
      #endif
    return 0;
    }

    getWheelVal();
    if (sign(d)*(target - Wheel_val)<=0) {
      move_x(STOP);
      break;
    }
  }
  return 1;
}

int move_x_by(int d, int tlimit)
{
  // this function is used to move distance > 12"
  // -- full speed ahead until some distance away from the taget
  // -- now brake down according to speed deceleration curve (using PID)
  // -- until it hit some threshold of margin of error and stop
  // -- single run w/o iteration

    // automatically switch to move_x_by_slow if the distance to travel is less than 12
  if (abs(d)<=1200)                //
  {
    int status = move_x_by_slow(d, tlimit);
    return status;
  }


  getWheelVal();
  int target = Wheel_val+d;
  int v;

  writeDebugStreamLine("t %4d+%4d=%4d",Wheel_val,d,target);

  int tnow0 = time1[T1];
  int dir = sign(d);
  int toTarget;
   // WHEEL_MONITOR = START;
  while ( true)
  {
    if (is_bailed_out()==1 || is_timed_out(tnow0+tlimit)==1)
    {
      writeDebugStreamLine("MOVE_END");
      return 0;
    }

    getWheelVal();
    toTarget = target - Wheel_val;
    // up to DX1 full speed
    if  ((dir==1 && Wheel_val<target-MOVE_X_DX1) || (dir==-1 && Wheel_val>target+MOVE_X_DX1))
    {
      move_x(dir*MOVE_X_VOLT1);
    }
    // up to DX2 linearly reduce the speed
    else if  ((dir==1 && Wheel_val<target-MOVE_X_DX2) || (dir==-1 && Wheel_val>target+MOVE_X_DX2))
    {
      v = MOVE_X_VOLT2+(abs(toTarget)-MOVE_X_DX2)*(MOVE_X_VOLT1-MOVE_X_VOLT2)/(MOVE_X_DX1-MOVE_X_DX2);
      move_x(dir*v);
    }
    // up to DX3 try to control the cruise speed to ...
    else if ((dir==1 && Wheel_val<target-MOVE_X_DX3) || (dir==-1 && Wheel_val>target+MOVE_X_DX3))
    {
      v = -MOVE_X_KV*(Wheel_speed - dir*MOVE_X_V_SLOW);
      if (dir==1) v = bound(v,0,MOVE_X_VOLT_MAX);
      if (dir==-1) v = bound(v,-MOVE_X_VOLT_MAX,0);
      move_x(v);
    }
    else
    {
      move_x( dir*MOVE_X_VOLT_BRAKE);
      wait1Msec(50+abs(Wheel_speed)*.25);
      move_x(STOP);
      // wait1Msec(500);
      // writeDebugStreamLine("MOVE_END");
      return 1;
    }

    wait1Msec(2);
  }
  return 1;
   // WHEEL_MONITOR = STOP;
}

/////////////////////////////////////////
// MOVE_X by certain distance at full speed and no braking logics afterwards
int move_x_by_nostop(int d, int tlimit)
{
  getWheelVal();
  int target = Wheel_val+d;

  writeDebugStreamLine("t %4d+%4d=%4d",Wheel_val,d,target);

  int tnow0 = time1[T1];
  int dir = sign(d);
   // WHEEL_MONITOR = START;
  while ( true)
  {
    if (is_bailed_out()==1 || is_timed_out(tnow0+tlimit)==1)
    {
      writeDebugStreamLine("MOVE_END");
      return 0;
    }

    getWheelVal();
    move_x(dir*MOVE_X_VOLT1);

    // up to DX1 full speed
    if  ((dir==1 && Wheel_val<target) || (dir==-1 && Wheel_val>target))
    {
      move_x(dir*MOVE_X_VOLT1);
    }

    else
    {
      move_x(STOP);
      return 1;
    }

    wait1Msec(1);
  }
  return 1;
   // WHEEL_MONITOR = STOP;
}


int rotate_by_slow(int angle, int tlimit)
{
  getWheelVal();
  int dir = sign(angle);
  int target = Gyro_val+angle;
  int tnow0 = time1[T1];

  rotate(dir*ROTATE_SLOW_VOLT);
  while ( true)
  {
    if (is_bailed_out()==1 || is_timed_out(tnow0+tlimit)==1)          {
      return 0;
    }

    getWheelVal();
    if (dir*(target - Gyro_val)<=0) {
      rotate(dir*ROTATE_SLOW_BRAKE_VOLT);
      wait1Msec(40);
      break;
    }
  }
  return 0;
}

int rotate_by(int angle, int tlimit)
{
  // automatically switch to rotate_by_slow if the angle to rotate is small
  if (abs(angle)<=300)
  {
    int status = rotate_by_slow(angle, tlimit);
    return status;
  }
  getWheelVal();
  int dir = sign(angle);
  int target = Gyro_val+angle;
  int tnow0 = time1[T1];
  int vTarget, deltaV, toTarget, brake_volt;

  while ( true)
  {
    if (is_bailed_out()==1 || is_timed_out(tnow0+tlimit)==1)          {
      // #if DEBUG_BAT>0
       // writeDebugStreamLine("%%ROTATE_BY_BAILOUT");
      // #endif
      writeDebugStreamLine("MOVE_END");
      return 0;
    }
    getWheelVal();
    toTarget = target-Gyro_val;
    // far away from target, full speed ahead/or back
    if (abs(toTarget)>ROTATE_DA1)
    {
      rotate(dir*127);
    }
    // in the braking zone, use PID to control the velocity deceleration
    else if (abs(toTarget)>ROTATE_DA2)
    {
      vTarget = dir*(ROTATE_BRAKE_V2+abs(toTarget)*(ROTATE_BRAKE_V1-ROTATE_BRAKE_V2)/(ROTATE_DA1-ROTATE_DA2));
      deltaV = Gyro_speed-vTarget;
      // -- out of upper bound, (too fast right, too slow left)
      if (deltaV>ROTATE_BRAKE_DV)
      {
        brake_volt = dir*ROTATE_BRAKE_VOLT-(deltaV-ROTATE_BRAKE_DV)*ROTATE_BRAKE_KV;
      }
      // -- out of lower bound, too slow for right, too fast for left
      else if (deltaV<-ROTATE_BRAKE_DV)
      {
        brake_volt = dir*ROTATE_BRAKE_VOLT-(deltaV+ROTATE_BRAKE_DV)*ROTATE_BRAKE_KV;
      }
      // -- in bound, keep default volt
      else
      {
        brake_volt = dir*ROTATE_BRAKE_VOLT;
      }
      // output_debug_stream_dt();   // output to steam at dt interval
      // writeDebugStreamLine("%%-%3d %4.1f %4.1f %4d", tnow, xnow-xnow0, xsmt_v,-dir*brake_p);
      brake_volt = bound(brake_volt,-ROTATE_BRAKE_VMAX,ROTATE_BRAKE_VMAX);
      rotate(brake_volt);
    }

    // when about to reach the target turn the motor to hold it in the direction of movement
    else
    {
      rotate( dir*ROTATE_BRAKE_VOLT);
      wait1Msec(ROTATE_BRAKE_DT);
      // wait1Msec(50+abs(Gyro_speed)*.25);
      rotate(STOP);
      // wait1Msec(400);
      // move_x_hold(sign(toTarget));
      // #if DEBUG_BAT>0
        // writeDebugStreamLine("%%ROTATE_BY_STARTS");
      // #endif
      // writeDebugStreamLine("MOVE_END");
      return 1;
    }
    wait1Msec(2);
  }
  return 1;
   // WHEEL_MONITOR = STOP;
}



#endif //SOMNUS_FUNC_C
