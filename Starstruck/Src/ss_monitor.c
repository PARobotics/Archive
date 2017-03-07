#ifndef SS_MONITOR_C
#define SS_MONITOR_C
#define DUMP_MIN_REAL             2550     //In 1/10 degree
#define DUMP_DT              20

#define LIFT_DT          20
#define LIFT_MIN_REAL    3640
// #define WHEEL_DT            20     // wheel update every 0.05 seconds
#define WHEEL_DT            10     // wheel update every 0.01 second
#define DEBUG_DUMP          2     // flag; turn ON/OFF debug message for LCD every 40 msec
#define DEBUG_DRV           4     // flag; turn ON/OFF debug message for DRV every 40 msec
#define DEBUG_LIFT          1     // flag; turn ON/OFF debug message for LIFT

// handle lift -> get a value from 0 to 1000 with 0 down 1000 up
#define LIFT_DWN_POT   2273      // was 3200
#define LIFT_UP_POT    0         // was 1000
// need to create lift value curve and update the above parameters


int Wheel_t0 = time1[T1];

// define all the sensor value functions
void getDumpVal() {
  DUMP_VAL = ((2560 * (DUMP_MIN_REAL - SensorValue(DUMP_ANGLE))) / 4096);// in 1/10 degree
}

void getWheelVal() {
  //360 ticks = 4 (diameter) * pi
  DRV.raw[0] = -SensorValue(WHEEL_L);
  DRV.raw[1] = SensorValue(WHEEL_R);
  DRV.raw[2] = SensorValue(G_SENSOR);
  // no more than 360 deg per move
  if (DRV.raw[2] - DRV.raw_last[2] > 2700)  DRV.raw[2] = DRV.raw[2] - 3600;
  if (DRV.raw[2] - DRV.raw_last[2] < -2700) DRV.raw[2] = DRV.raw[2] + 3600;
}

void getLiftVal() {
  // LIFT_VAL = LIFT_MIN_REAL - SensorValue(LIFT_POT);
  LIFT_VAL = 1000*(SensorValue(LIFT_POT)-LIFT_DWN_POT)/(LIFT_UP_POT-LIFT_DWN_POT) ;
}

int getAccelerometerVal(){
  return sqrt(pow(SensorValue(ACC_X), 2) + pow(SensorValue(ACC_Y), 2) + pow(SensorValue(ACC_Z), 2));
}

void hasCollided(){
  if(getAccelerometerVal() >= 15) COLLIDED = YES;
  else COLLIDED = NO;
}

void wheel_init() {
  for (unsigned int i = 0; i < 3; i++) {
    DRV.raw      [i] = 0;
    //DRV.raw_pre  [i] = 0;
    DRV.raw_last [i] = 0;
    DRV.pos      [i] = 0.0;
    DRV.pos_pre  [i] = 0.0;
    DRV.pos_delta[i] = 0.0;
    DRV.move     [i] = 0.0;
  }
  for (unsigned  int i=0; i<2; i++) {
    DRV.speed    [i] = 0;
  }

  DRV.method        = 1;                  // method for getting detla_r   1: use gyro, 2: use wheels
  DRV.pi2d          = 572.9578;           // .1 deg per pi
  DRV.tick2inch     = 4 * 3.14159 / 36;   // 0.1 inch per tick
  DRV.t_last        = 0;
  DRV.dbg_cnt       = 0;
  DRV.DT            = WHEEL_DT;
  DRV.DX            = 130;               // 13" between the wheel
}


void wheel_refresh() {
      // writeDebugStreamLine("wheel_refresh");
  for (unsigned  int i=0; i<3; i++)  DRV.pos_pre[i]  = DRV.pos[i];
  for (unsigned  int i=0; i<3; i++)  DRV.move   [i]  = 0.;
}

void wheel_reset(float X, float Y, float R) {
  DRV.pos[0] = X;
  DRV.pos[1] = Y;
  DRV.pos[2] = R;
}

void wheel_output_pos(int lmk_id) {
  #if DEBUG_MSG == YES
    // writeDebugStreamLine("LMK%d",lmk_id);
    writeDebugStreamLine("K%d %4d %4d %4d %4d", lmk_id, time1[T1] - Wheel_t0, DRV.pos[0],DRV.pos[1],DRV.pos[2]);
  #endif
}

void dbgMsg(const char* msg) {
  #if DEBUG_MSG == YES
    writeDebugStreamLine("%% -- %s",msg);
  #endif
}

// define the ss_monitor tasks
task ss_monitor()
{
  //int tcur, Wheel_dbg_cnt;
  int tnow; int dump_t0; int lift_last_monitor = 0;
  // int Wheel_t0; -- need Wheel_t0 to be globa
  int lift_t0;
  float dw_l,dw_r, drot, dd;     // delta distance travelled
  int dump_dbg_cnt = 0;
  int lift_dbg_cnt = 0;
  while (true) {
    tnow = time1[T1];

    /*
      LIFT
    */

    if (LIFT_MONITOR == START && tnow >= lift_last_monitor + LIFT_DT) {
      getLiftVal();
      lift_last_monitor = tnow;
      LIFT_LAST_VAL = LIFT_VAL;
      LIFT_LAST_V = 0;
      LIFT_V = 0;
      LIFT_MONITOR = MONITOR;
      lift_t0 = tnow;
      lift_dbg_cnt = 0;
    }
    else if(LIFT_MONITOR == MONITOR && tnow >= lift_last_monitor + LIFT_DT){
      getLiftVal();
      LIFT_LAST_V = LIFT_V;
      LIFT_V = abs(1000 *  (LIFT_VAL - LIFT_LAST_VAL) / (tnow - lift_last_monitor));
      lift_last_monitor = tnow;
      LIFT_LAST_VAL = LIFT_VAL;

      lift_dbg_cnt ++;
      if (DEBUG_LIFT > 0 && DEBUG_LIFT == lift_dbg_cnt) {
        lift_dbg_cnt = 0;
        writeDebugStreamLine("L %4d %4d %4d", tnow-lift_t0, LIFT_VAL, LIFT_V);
      }
    }
    else if(LIFT_MONITOR == STOP){
      LIFT_V = 0;
    }

    /*
      DUMPER
    */

    if (DUMP_MONITOR == START) {
      writeDebugStreamLine("DUMP_MONITOR=START");
      getDumpVal();
      dump_last_monitor = tnow;
      DUMP_LAST_VAL = DUMP_VAL;
      DUMP_LAST_OMEGA = 0;
      DUMP_OMEGA = 0;
      DUMP_MONITOR = MONITOR;
      writeDebugStreamLine("DUMP_MONITOR=MONITOR");
      dump_t0 = tnow;
      dump_dbg_cnt = 0;
      //dump_t0 = Wheel_t0;    // to sync with wheel movement
    }
    else if (DUMP_MONITOR == MONITOR && tnow >= dump_last_monitor + DUMP_DT) {
      getDumpVal();
      DUMP_LAST_OMEGA = DUMP_OMEGA;
      DUMP_OMEGA = (DUMP_VAL - DUMP_LAST_VAL) * 1000 * 60 / 360 / (tnow - dump_last_monitor); // .1 rpm
      dump_last_monitor = tnow;
      DUMP_LAST_VAL = DUMP_VAL;

      dump_dbg_cnt ++;
      if (DEBUG_DUMP > 0 && DEBUG_DUMP == dump_dbg_cnt) {
        dump_dbg_cnt = 0;
        writeDebugStreamLine("D %4d %3d %4d %4d", tnow - dump_t0, motor[M_DUMP_L1], DUMP_VAL, DUMP_OMEGA);
      }
    }
    else if (DUMP_MONITOR == STOP) {
      DUMP_OMEGA = 0;
    }

    /*
      WHEEL
    */
    if (WHEEL_MONITOR == START){
      writeDebugStreamLine("WHEEL_MONITOR=MONITOR");
      getWheelVal();

      //initialize values
      Wheel_t0    = tnow;
      DRV.t_last  = tnow;
      for (unsigned  int i=0; i<3; i++) {
        DRV.raw_last [i] = DRV.raw[i];
        DRV.pos_pre  [i] = DRV.pos[i];
        DRV.move     [i] = 0.0;
      }

      DRV.dbg_cnt  = 0;        // reset debug count
      WHEEL_MONITOR = MONITOR ;         // now set the status to MONITOR
    }
    //Monitor every WHEEL_DT seconds
    else if (WHEEL_MONITOR == MONITOR && tnow >= DRV.t_last + DRV.DT){
      // -- get raw values
      getWheelVal();
      // convert to .1 inch, .1 deg of increment
      dw_l = (DRV.raw[0]-DRV.raw_last[0])*DRV.tick2inch;
      dw_r = (DRV.raw[1]-DRV.raw_last[1])*DRV.tick2inch;

      // -- calculate speed and pos (update at DT)
      dd     = (dw_l+dw_r)/2.;

      DRV.pos[0] += dd*cos(DRV.pos[2]/DRV.pi2d);
      DRV.pos[1] += dd*sin(DRV.pos[2]/DRV.pi2d);
      DRV.speed[0] = dd*1000/(tnow - DRV.t_last);

      if (DRV.method==2) {
        drot = (dw_l-dw_r)*DRV.pi2d/DRV.DX;
        // DRV.pos[2] += (dw_l-dw_r)*DRV.pi2d/DRV.DX;   // method 1
      }
      else if (DRV.method==1) {
        drot = DRV.raw[2]-DRV.raw_last[2];
        // DRV.pos[2] += DRV.raw[2]-DRV.raw_last[2];    // method 2
      }
      DRV.pos[2]   += drot;
      DRV.speed[1] = drot*1000/(tnow - DRV.t_last);

      // hash lateral (for control purpuse to move straight)
      DRV.move[0] += dd;    // total fwd/bkwd movement
      DRV.move[1] += dw_l-dw_r;        // total lateral movement
      DRV.move[2] += drot;

      // move frame fwd
      DRV.t_last = tnow;
      for (unsigned  int i=0; i<3; i++) DRV.raw_last[i] = DRV.raw[i];
      DRV.dbg_cnt ++;
      if (DEBUG_DRV > 0 && DEBUG_DRV == DRV.dbg_cnt) {
        DRV.dbg_cnt = 0;
        writeDebugStreamLine("W %4d %4d %4d %4d %d", tnow - Wheel_t0, DRV.pos[0],DRV.pos[1],DRV.pos[2], motor[0]);
      }

      if(ACC_MONITOR == START){
        hasCollided();
      }
    }
    else if (WHEEL_MONITOR == STOP){
      // writeDebugStreamLine("WHEEL_MONITOR=STOP");
      wheel_refresh();
      DRV.speed[0] = 0;
      DRV.speed[1] = 0;
      //Y_vel = 0;
      //R_vel = 0;
    }

    wait1Msec(1);                         // avoid jamming the system with inf loop
  }                                       // end of the infinite while loop
}


#endif //SS_MONITOR_C
