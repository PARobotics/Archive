// w/o hold; push back v 66
// w   hold, push back v 85
//  get rid of pheonix
//  get rid of continuous mode
//  get rid of auto-adjust
//  get rid of lift
//
#ifndef SOMNUS_BAT_C
#define SOMNUS_BAT_C

task somnus_bat()
{
  #if DEBUG_BAT>0
     writeDebugStreamLine("%%    startTask somnus_bat");
  #endif
	while (true)
  {                            // don't hold forever for LONG
    if (BAT_CMD==STOP)          {
      if (time1[T1]>BAT_HOLD1_TLAST+BAT_HOLD1_TMAX) bat(STOP);
      wait1Msec(BAT_DT);
    } // do nothing

    //Single long shot
    if (BAT_CMD==LONG)      {
      getWheelVal();
      writeDebugStreamLine("SSS vs= %4.2f  t= %4d",VOLT_SCALE, BAT_LONG_PULL_ANG_MAX);
      BAT_MONITOR = START;
      bat_long();
      BAT_COUNT_TOTAL ++ ;
      BAT_CMD = STOP;  BAT_MONITOR = STOP;
      // bat_hold(BAT_HOLD1_V, 300); // need some logics after this to hold bat in place
    }

    //Skills Shot -- with logics to hold fire if ball is not loaded for a couple of seconds
    // skills shooting might worth check if loaded first before pulling???
    if (BAT_CMD==SKILLS)    {
     writeDebugStreamLine("SSS vs= %4.2f  t= %4d",VOLT_SCALE, BAT_SKILLS_HIT_V);
      if (is_loaded()==1) {         // option to not pull till it has been loaded
        BAT_WAIT_TIME = 0;          // option to not pull till it has been loaded
        BAT_MONITOR = START;
        bat_skills();
        BAT_COUNT_TOTAL ++ ;
        bat(STOP); BAT_CMD = STOP;  BAT_MONITOR = STOP;
      }else                          // option to not pull till it has been loaded
      {                              // option to not pull till it has been loaded
      	wait1Msec(5);                // option to not pull till it has been loaded
        BAT_WAIT_TIME += 5;          // option to not pull till it has been loaded
        if (BAT_WAIT_TIME>BAT_WAIT_TIME_MAX)      // option to not pull till it has been loaded
        {BAT_CMD = STOP; BAT_WAIT_TIME=0;}   // option to not pull till it has been loaded
       }                           // option to not pull till it has been loaded
    }

    //Medium and short shots -- with logics to hold fire if ball is not loaded for a couple of seconds
    if (BAT_CMD==MEDIUM || BAT_CMD==SHORT)    {
     writeDebugStreamLine("SSS vm= %4.2f  t= %4d",VOLT_SCALE, BAT_MEDIUM_HIT_V);
      if (is_loaded()==1) {
        BAT_WAIT_TIME = 0;
        BAT_MONITOR = START;
        if (BAT_CMD==MEDIUM) bat_medium();
        if (BAT_CMD==SHORT)  bat_short();
        BAT_COUNT_TOTAL ++ ;
        bat(STOP); BAT_CMD = STOP;  BAT_MONITOR = STOP;
      }else
      {
      	wait1Msec(10);
        BAT_WAIT_TIME += 10;
        if (BAT_WAIT_TIME>BAT_WAIT_TIME_MAX)     // if it has been waitinf for more than 2 seconds, stop shotting mode
        {BAT_CMD = STOP; BAT_WAIT_TIME=0;}
    	}
    }
  }
}


/////////////////////////////////////////////////////////////////////////////////////////////
void bat_short()
{
  volt_adj();
  #if DEBUG_BAT>0
    writeDebugStreamLine("%%P VOLT1   = %d",volt1());
    writeDebugStreamLine("%%P VOLT2   = %d",volt2());
    writeDebugStreamLine("%%P S2_SC   = %5.2f",VOLT_SCALE);
    writeDebugStreamLine("%%P S_HIT_V = %d",BAT_SHORT_HIT_V);
  #endif
  bat_pull(BAT_SHORT_PULL_V*VOLT_SCALE,BAT_HOLD1_V*VOLT_SCALE, BAT_SHORT_PULL_ANG1,BAT_SHORT_PULL_ANG2);
  // bat_hit(BAT_SHORT_HIT_V*VOLT_SCALE,BAT_HIT_TMAX);
  bat_hit(BAT_SHORT_HIT_V,BAT_HIT_TMAX);
  bat_pullback(BAT_SHORT_PB_V*VOLT_SCALE, BAT_SHORT_PB_DT, BAT_SHORT_PB_DT_2);
}

/////////////////////////////////////////////////////////////////////////////////////////////
void bat_medium()
{
  volt_adj();
  #if DEBUG_BAT>0
    writeDebugStreamLine("%%P VOLT1   = %d",volt1());
    writeDebugStreamLine("%%P VOLT2   = %d",volt2());
    writeDebugStreamLine("%%P S2_SC   = %5.2f",VOLT_SCALE);
    writeDebugStreamLine("%%P M_HIT_V = %d",BAT_MEDIUM_HIT_V);
  #endif

  bat_pull(BAT_MEDIUM_PULL_V*VOLT_SCALE,BAT_HOLD1_V*VOLT_SCALE, BAT_MEDIUM_PULL_ANG1,BAT_MEDIUM_PULL_ANG2);
  // bat_hit(BAT_MEDIUM_HIT_V*VOLT_SCALE,BAT_HIT_TMAX);
  bat_hit(BAT_MEDIUM_HIT_V,BAT_HIT_TMAX);
  bat_pullback(BAT_MEDIUM_PB_V*VOLT_SCALE, BAT_MEDIUM_PB_DT, BAT_MEDIUM_PB_DT_2);

}

/////////////////////////////////////////////////////////////////////////////////////////////
int bat_skills()
{
  volt_adj();
  // WHEEL_MONITOR=START;
  #if DEBUG_BAT>0
    writeDebugStreamLine("%%P VOLT1   = %d",volt1());
    writeDebugStreamLine("%%P VOLT2   = %d",volt2());
    writeDebugStreamLine("%%P S2_SC   = %5.2f",VOLT_SCALE);
    writeDebugStreamLine("%%P M_HIT_V = %d",BAT_SKILLS_HIT_V);
    // writeDebugStreamLine("S %4.2f  %4d  %4d  %4d",VOLT_SCALE, Gyro_val, Wheel_val_L, Wheel_val_R);
  #endif

  bat_pull(BAT_SKILLS_PULL_V*VOLT_SCALE,BAT_HOLD1_V*VOLT_SCALE, BAT_SKILLS_PULL_ANG1,BAT_SKILLS_PULL_ANG2);
    // don't hit until see the ball
  int tnow = time1[T1];
  while (is_loaded()==0) {
    bat_hold(BAT_HOLD2_V*VOLT_SCALE,BAT_HOLD2_TWAIT);
    // if you got timed out, reset the bat don't hit
    if (is_bailed_out()==1 || is_timed_out(tnow+BAT_HOLD2_TMAX )==1) {
       bat_reset();
       return 0;
    }
  }
  // bat_hit(BAT_SKILLS_HIT_V*VOLT_SCALE,BAT_HIT_TMAX);
  bat_hit(BAT_SKILLS_HIT_V,BAT_HIT_TMAX);
  bat_pullback(BAT_SKILLS_PB_V*VOLT_SCALE, BAT_SKILLS_PB_DT, BAT_SKILLS_PB_DT_2);
  return 1;
}


/////////////////////////////////////////////////////////////////////////////////////////////
// at the end of bat_long; bat is at the top of reverse swing with RPM<=0 and moving back
// with HOLD1_V
int bat_long()
{
  volt_adj();

  #if DEBUG_BAT>0
    writeDebugStreamLine("%%P VOLT1   = %d",volt1());
    writeDebugStreamLine("%%P VOLT2   = %d",volt2());
    writeDebugStreamLine("%%P S2_SC   = %5.2f",VOLT_SCALE);
    writeDebugStreamLine("%%P L_ANG_MAX = %d",BAT_LONG_PULL_ANG_MAX);
  #endif

  bat_pull_long();

  // don't hit until see the ball
  int tnow = time1[T1];
  while (is_loaded()==0) {
    bat_hold(BAT_HOLD2_V*VOLT_SCALE,BAT_HOLD2_TWAIT);
    // if you got timed out, reset the bat don't hit
    if (is_bailed_out()==1 || is_timed_out(tnow+BAT_HOLD2_TMAX )==1) {
       bat_reset();
       return 0;
    }
  }
  // hit now
  bat_hit(BAT_LONG_HIT_V,BAT_HIT_TMAX);

  // pull back after hit
  bat_pullback(BAT_LONG_PB_V*VOLT_SCALE, BAT_LONG_PB_DT, BAT_LONG_PB_DT_2);
  return 1;

}

///////////////////////////////////////////////////////////////////////////////////////////////
// pull at voltage v1 up to angle  and break
// notice
//   * the residual speed will carry it a little further
//   * optimizaed via post-processed RPM and ANG time plots
//
void bat_pull(int v1, int v2, int ang1, int ang2 )
{
  int v;
  int tnow = time1[T1];
  bat(PULL*v1);
  while (true) {
    // don't set v = 0; leave it later ...
    if (is_bailed_out()==1 || is_timed_out(tnow+BAT_PULL_TMAX )==1) {
      #if DEBUG_BAT>0
        writeDebugStreamLine("pull bail out");
      #endif
      break ;
    }

    if (Bat_val<=ang1)
    {
      bat(PULL*v1);
    }
    // phase 2 -- reduce vol linearly
    else if (Bat_val<=ang2)
    {
      v = v1-(Bat_val-ang1)*(v1-v2)/(ang2-ang1);
      bat(PULL*v);
    }
    else {
      #if DEBUG_BAT>0
        writeDebugStreamLine("pull ends %d %d",Bat_val,ang2);
      #endif
      break;
    }
    getBatVal();
    // wait1Msec(2);
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////
// pull in 3 phases
// phase 1: full voltage to ANG1
// phase 2: reduce linearly from ANG1 to ANG2
// phase 3: reduce linearly from ANG2 to ANG3
// notice
//   * the residual speed will carry it a little further after phase 3
//   * optimizaed via post-processed RPM and ANG time plots
//
void bat_pull_long()
{
  int tnow = time1[T1];
  int degPerSec, dtMore;

  float NScale  = 1.0;
  int ang1 = BAT_LONG_PULL_ANG1;
  int ang2 = BAT_LONG_PULL_ANG2;
  int ang3 = BAT_LONG_PULL_ANG_MAX-BAT_LONG_PULL_DANG3;
  int rpm1 = BAT_LONG_PULL_RPM1;
  int rpm2 = BAT_LONG_PULL_RPM2;
  int rpm3 = BAT_LONG_PULL_RPM3;
  int vcmd1 = BAT_LONG_PULL_VCMD1*NScale;
  int vcmd2 = BAT_LONG_PULL_VCMD2*NScale;
  int vcmd3 = BAT_LONG_PULL_VCMD3*NScale;
  int vcmd4 = BAT_LONG_PULL_VCMD4*NScale;
  int vcmd2_min = BAT_LONG_PULL_VCMD2_MIN*NScale;
  int vcmd3_min = BAT_LONG_PULL_VCMD3_MIN*NScale;
  int vcmd4_min = BAT_LONG_PULL_VCMD4_MIN*NScale;
  int vcmd2_max = BAT_LONG_PULL_VCMD2_MAX*NScale;
  int vcmd3_max = BAT_LONG_PULL_VCMD3_MAX*NScale;
  int vcmd4_max = BAT_LONG_PULL_VCMD4_MAX*NScale;
  float vcmd2_k = BAT_LONG_PULL_VCMD2_K*NScale;
  float vcmd3_k = BAT_LONG_PULL_VCMD3_K*NScale;
  float vcmd4_k = BAT_LONG_PULL_VCMD4_K*NScale;

  int rpm,vcmd,rpm0;
  while (true) {
    // don't set v = 0; leave it later ...
    if (is_bailed_out()==1 || is_timed_out(tnow+BAT_PULL_TMAX )==1) {
      #if DEBUG_BAT>0
        writeDebugStreamLine("pull bail out");
      #endif
      break ;
    }

    // phase 1 -- full vol ahead
    if (Bat_val<=ang1)
    {
      vcmd = vcmd1;
      bat(PULL*vcmd*VOLT_SCALE);
    }
    // phase 2 -- reduce vol linearly
    else if (Bat_val<=ang2)
    {
      rpm0 = (Bat_rpm+Bat_rpm_last)/2;
      vcmd = vcmd2+vcmd2_k*(rpm1-rpm0);
      vcmd = bound(vcmd,vcmd2_min,vcmd2_max);
      bat(PULL*vcmd*VOLT_SCALE);
    }
    // phase 3 --
    else if (Bat_val<=ang3)
    {
      rpm0 = (Bat_rpm+Bat_rpm_last)/2;
      rpm = rpm2+(Bat_val-ang2)*(rpm3-rpm2)/(ang3-ang2);
      vcmd = vcmd3+vcmd3_k*(rpm-rpm0);
      vcmd = bound(vcmd,vcmd3_min,vcmd3_max);

      bat(PULL*vcmd*VOLT_SCALE);
    }
    else
    {
      rpm0 = (Bat_rpm+Bat_rpm_last)/2;
      degPerSec = rpm0*6;

      vcmd = vcmd4+vcmd4_k*(rpm3-rpm);
      vcmd = bound(vcmd,vcmd4_min,vcmd4_max);
      bat(PULL*vcmd*VOLT_SCALE);
      getBatVal();                   // now update velocity and ang
      if (Bat_val>=BAT_LONG_PULL_ANG_MAX)
      {
        #if DEBUG_BAT>0
          writeDebugStreamLine("pull out %d %d",Bat_val,BAT_LONG_PULL_ANG_MAX);
        #endif
        break;
      }

      dtMore = (BAT_LONG_PULL_ANG_MAX-Bat_val)*1000/degPerSec-BAT_LONG_PULL_REV_DT;
      dtMore = bound(dtMore,0, BAT_LONG_PULL_REV_HOLDTMAX);                // if stuck, I don't think takes more than extran  120 msec
               writeDebugStreamLine("*** dt = %d , A = %d , rpm = %d ",dtMore,Bat_val, degPerSec/6);

      wait1Msec(dtMore);             // hold extra time at the voltage based on how much has moved etc
      getBatVal();                   // now update velocity and ang
      #if DEBUG_BAT>0
         writeDebugStreamLine("pull out %d",Bat_val);
       #endif
       break;
    }
    getBatVal();
    wait1Msec(2);
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////
// apply a small voltage of v for specified time dt (to hold rubber band in place)
//
void bat_hold(int v, int dt)
{
  int tnow = time1[T1];
  volt_adj();
  while (time1[T1]<tnow+dt) {
    if (is_bailed_out()==1)  break;
    bat(PULL*v*VOLT_SCALE);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// push at voltage v up to hit limit; timeout period is dt
// notice
//   * the v is the main control/adjustable parameters that affects shooting distance!!
void bat_hit(int v, int dt)
{
  int tnow = time1[T1];
  getBatVal();

  int last_v = Bat_val;
  int v_hit=v;
  int is_reversed=0;
  while (true) {
    if (is_bailed_out()==1 || is_timed_out(tnow+dt )==1) {
      bat(STOP);
      writeDebugStreamLine("push bail out ");
      break ;
    }


    if (Bat_val<=BAT_HIT_LIMIT) {
      bat(STOP);
      writeDebugStreamLine("push %d %d",Bat_val,BAT_HIT_LIMIT);
      writeDebugStreamLine("push out at target");
      break;
      }
    else
    {
      bat(PUSH*v_hit*VOLT_SCALE);
    }
    wait1Msec(1);
    getBatVal();

  }
}


///////////////////////////////////////////////////////////////////////////////////////////////
// pull back logics goes like this
// 1. apply a constant voltage for dt
// 2. use pid (d) control that apply a braking voltage proportional to RPM shifted by drpm
// 3. exit when RPM becomes positive (back swing reachs peak) and hold it for dt2 to let
//    bat falls back a bit

void bat_pullback(int v0, int dt, int dt2)
{
  // now try to brake the bat and slowly put it in correct position
  int tnow = time1[T1];
  int v;
  bat(PULL*v0);
  wait1Msec(dt);

  while (true) // need at least 80 msec for the process
  {
    // time out logics
    if (is_bailed_out()==1 || is_timed_out(tnow+BAT_PB_TMAX )==1) {
#if DEBUG_BAT>0
      writeDebugStreamLine("pb bail out ");
#endif
      bat(STOP);
      break ;
    }

    // exit logics (dt2 msec after rpm become +)
    if (Bat_rpm>=0) {
      BAT_HOLD1_TLAST = time1[T1];              // we don't hold it forever
      bat(BAT_HOLD1_V*VOLT_SCALE);             // apply a small hold_v at the end
      wait1Msec(dt2);
#if DEBUG_BAT>0
      writeDebugStreamLine("pb end ");
#endif
      break ;
    }

   // pid adjustable control
    v = -BAT_PB_KV*(Bat_rpm+BAT_PB_DRPM)*VOLT_SCALE;
    v = bound(v,0, v0);
    bat(PULL*v);
  }
}



///////////////////////////////////////////////////////////////////////////////////////////////
// when pulled all the way but ball is not loaded, reset
// applying a very small voltage that will allows the bat to move back
// exits when bat back to 0 position

void bat_reset()
{
  // now try to brake the bat and slowly put it in correct position
  int tnow = time1[T1];
  bat(PUSH*BAT_RESET_V);   //assuming the value was set correctly at 8V

  while (true) // need at least 80 msec for the process
  {
    // time out logics
    if (is_bailed_out()==1 || is_timed_out(tnow+BAT_RESET_TMAX )==1) {
#if DEBUG_BAT>0
      writeDebugStreamLine("reset  bail out ");
#endif
      bat(STOP);
      break ;
    }

    // exit logics (dt2 msec after rpm become +)
    if (Bat_rpm<=BAT_RESET_ANG) {
#if DEBUG_BAT>0
      writeDebugStreamLine("reset done ");
#endif
      bat(STOP);
      break ;
    }

  }
}


// the following function accounts for battery voltage variation -- needs fine tuning
void volt_adj()
{
  // have 3 on cortex and 2 on power expander; scaling accordingly
  // VOLT_SCALE = 8000.0/volt2();       // expected range is 7.6 to 8.4
  VOLT_SCALE = 4800.0/volt1()+3200.0/volt2();

                                      // float number
}
#endif SOMNUS_BAT_C
