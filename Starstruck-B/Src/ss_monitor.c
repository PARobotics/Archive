#ifndef SS_MONITOR_C
#define SS_MONITOR_C

int CLAW_DT = 20;

task ss_monitor()
{
  //int tcur, Wheel_dbg_cnt;
  int tnow; int claw_t0, dump_t0; int lift_last_monitor = 0; int wheel_t0;
  int dump_dbg_cnt = 0;
  int wheel_dbg_cnt = 0;
  int claw_dbg_cnt = 0;
  while (true) {
    tnow = time1[T1];

    /*
      DUMPER
    */

  	if(DUMP_MONITOR == START){
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
  	}
    else if(DUMP_MONITOR == MONITOR && tnow >= dump_last_monitor + DUMP_DT){
      getDumpVal();
      DUMP_LAST_OMEGA = DUMP_OMEGA;
      DUMP_OMEGA = (DUMP_VAL - DUMP_LAST_VAL)*1000*60/360/(tnow - dump_last_monitor);  // .1 rpm
      dump_last_monitor = tnow;
      DUMP_LAST_VAL = DUMP_VAL;
      dump_dbg_cnt ++;
      #if DEBUG_DUMP > 0
      if (DEBUG_DUMP == dump_dbg_cnt) {
        dump_dbg_cnt = 0;
        writeDebugStreamLine("D %4d %3d %4d %4d", tnow - dump_t0, motor[M_DUMPER_ML], DUMP_VAL, DUMP_OMEGA);
      }
      #endif
    }
    else if(DUMP_MONITOR == STOP){
      DUMP_OMEGA = 0;
    }

    /*
    	WHEEL
    */
    if (WHEEL_MONITOR==START )
    {
      writeDebugStreamLine("WHEEL_MONITOR=MONITOR");
      getWheelVal();
      Wheel_t_last      = tnow;           // initialization of Monitoring
      X_disp_last       = X_disp;
      Y_disp_last       = Y_disp;
      R_disp_last       = R_disp;
      X_vel             = 0;
      Y_vel             = 0;
      R_vel             = 0;
      wheel_dbg_cnt     = 0;              // reset debug count
      wheel_t0 = tnow;
      WHEEL_MONITOR = MONITOR ;         // now set the status to MONITOR
    }
    //Monitor after WHEEL_DT has passed
    else if (WHEEL_MONITOR==MONITOR && tnow>= Wheel_t_last + WHEEL_DT)
    {
      getWheelVal();                    // got Wheel_val (in 0.01 inch)
      if(R_disp - R_disp_last > 2700) R_disp = R_disp - 3600;
      if(R_disp - R_disp_last < -2700) R_disp = R_disp + 3600;
      X_vel  = 100*(X_disp-X_disp_last)/(tnow-Wheel_t_last);   // in 0.1 inch/second
      Y_vel  = 100*(Y_disp-Y_disp_last)/(tnow-Wheel_t_last);   // in 0.1 inch/second
      R_vel  = 100*(R_disp-R_disp_last)/(tnow-Wheel_t_last);   // in deg/second
      Wheel_t_last = tnow;           // now move the frame (current becomes last)
      X_disp_last  = X_disp;
      Y_disp_last  = Y_disp;
      R_disp_last  = R_disp;
      wheel_dbg_cnt ++;
      #if DEBUG_WHEEL > 0
        if (DEBUG_WHEEL == wheel_dbg_cnt) {
          wheel_dbg_cnt = 0;
          writeDebugStreamLine("W %4d %4d %4d %4d %d", tnow - wheel_t0, X_disp,Y_disp,R_disp, motor[M_WHEEL_FL]);
        }
      #endif

    }
    else if (WHEEL_MONITOR==STOP )
    {

      X_vel = 0;
      Y_vel = 0;
      R_vel = 0;
    }

    wait1Msec(1);                         // avoid jamming the system with inf loop
  }

  /*
  CLAW
  */
  if(DUMP_MONITOR == START){
      writeDebugStreamLine("DUMP_MONITOR=START");
  		getClawVal();
      claw_last_monitor = tnow;
      CLAW_LAST_VAL_L = CLAW_VAL_L;
      CLAW_LAST_OMEGA_L = 0;
      CLAW_OMEGA_R = 0;
      CLAW_LAST_VAL_R = CLAW_VAL_R;
      CLAW_LAST_OMEGA_R = 0;
      CLAW_OMEGA_L = 0;
      CLAW_MONITOR = MONITOR;
      writeDebugStreamLine("CLAW_MONITOR=MONITOR");
      claw_t0 = tnow;
      claw_dbg_cnt = 0;
  	}
    else if(CLAW_MONITOR == MONITOR && tnow >= claw_last_monitor + CLAW_DT){
      getDumpVal();
      CLAW_LAST_OMEGA_L = CLAW_OMEGA_L;
      CLAW_OMEGA_L = (CLAW_VAL_L - CLAW_LAST_VAL_L)*1000*60/360/(tnow - claw_last_monitor);  // .1 rpm
      CLAW_LAST_VAL_L = CLAW_VAL_L;
      CLAW_LAST_OMEGA_R = CLAW_OMEGA_R;
      CLAW_OMEGA_R = (CLAW_VAL_R - CLAW_LAST_VAL_R)*1000*60/360/(tnow - claw_last_monitor);  // .1 rpm
      CLAW_LAST_VAL_R= CLAW_VAL_R;
      claw_last_monitor = tnow;

      #if DEBUG_CLAW > 0
      claw_dbg_cnt ++;
      if (DEBUG_CLAW == claw_dbg_cnt) {
        claw_dbg_cnt = 0;
        writeDebugStreamLine("C %4d %3d %4d %4d", tnow - claw_t0, CLAW_VAL_L, CLAW_VAL_R, motor[M_CLAW_L], motor[M_CLAW_R]);
      }
      #endif
    }
    else if(CLAW_MONITOR == STOP){
      CLAW_OMEGA_L = 0;
      CLAW_OMEGA_R = 0;
    }
  // end of the infinite while loop
}


#endif //SS_MONITOR_C
