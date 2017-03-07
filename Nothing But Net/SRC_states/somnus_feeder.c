#ifndef SOMNUS_FEEDER_C
#define SOMNUS_FEEDER_C
//

//


task somnus_feeder()
{
	#if DEBUG == YES
	    writeDebugStreamLine("%%    startTask somnus_feeder");
	#endif

  int t_last_reset = time1[T1];
	while (true)   // a task requires an infinite loop till stop
  {

    // smartly does somthing here -- ONLY when BAT_MODE == SINGLE
    if (FEEDER_CMD == STOP && BAT_MODE == SINGLE)
    {
      // automatically starts feeder
      is_loaded();
      is_queued();
      if( FEEDER_ON == ON && Ball0==0 && Ball1==1 && is_bat_to_feed()==1)
      {
        FEEDER_CMD=FEED;
      }
      // reset feeder to position (once every 1 second)
      else if(( time1[T1]-t_last_reset>1000) && LIFT_MODE == OFF)
      {
        feeder_reset();
        t_last_reset = time1[T1];
      }
    }

    //
    if (FEEDER_CMD==FEED)
    {
      FEEDER_MONITOR = START;               // start monitoring and calculate rpm
      feed_1ball();
      feeder(STOP);
      FEEDER_CMD = STOP;
      FEEDER_MONITOR = STOP;
    }

    wait1Msec(FEEDER_DT);  //avoid jamming the system with inf loop
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// 1. feeder go up at full voltage until it hits braker point
// 2. using PID control to slow down the feeder (while still going up)
//    there is no breaking logics -- it has to be timed out
// 3. same with going down -- full voltage till brake point and PID till time out
//  ** parameters fine tuned from post-processed RPM and ANG plots
//  **
void feed_1ball()
{
  int tcur, v;
  tcur = time1[T1];
  // -- fast up to range
  while (true)
  {
    feeder(FEEDER_V_MAX);
    if (is_bailed_out()==1 || is_timed_out(tcur+FEEDER_TMAX_UP )==1) { feeder(0); break;}
    if (Feeder_val>=FEEDER_UP_BRAKE) { feeder(0); break; } // stop when it hits FEEDER_UP_BRAKE
    wait1Msec(FEEDER_DT);
  }

  // -- slow down -- only exit when time is up
  while (true)
  {
    if (is_bailed_out()==1 || is_timed_out(tcur+FEEDER_TMAX_UP )==1) { feeder(0); break;}
    v = FEEDER_KP_UP*(FEEDER_UP_LIMIT-Feeder_val)-FEEDER_KV_UP*Feeder_rpm;
    v = bound(v,-FEEDER_V_MAX,FEEDER_V_MAX);
    if (abs(v)<FEEDER_V_MIN) v=0;
    feeder(v);
    wait1Msec(FEEDER_DT);
  }
  #if DEBUG_FEEDER>0
    writeDebugStreamLine("Feeder up end");
  #endif

  /////////GO DOWN ///////////////////////////////////
  tcur = time1[T1];
  while (true)
  {
    feeder(-FEEDER_V_MAX);
    if (is_bailed_out()==1 || is_timed_out(tcur+FEEDER_TMAX_DOWN )==1) { feeder(0); break;}
    if (Feeder_val<=FEEDER_DOWN_BRAKE) { feeder(0); break; }  // stop when below brake limit
    wait1Msec(FEEDER_DT);
  }

  // -- slow down --  only brake when time is up
  while (true)
  {
    if (is_bailed_out()==1 || is_timed_out(tcur+FEEDER_TMAX_DOWN )==1) { feeder(0); break;}
    v = FEEDER_KP_DOWN*(FEEDER_DOWN_LIMIT-Feeder_val)-FEEDER_KV_DOWN*Feeder_rpm;
    v = bound(v,-FEEDER_V_MAX,FEEDER_V_MAX);
    if (abs(v)<FEEDER_V_MIN) v=0;
    feeder(v);
    wait1Msec(FEEDER_DT);
  }
  // wait some extra time before next feeder to let the feed stop completely???
  wait1Msec(50);
  #if DEBUG_FEEDER>0
    writeDebugStreamLine("Feeder down end");
  #endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// WHEN FEEDER is in STOP
// will check every once a while (20*DT) to see if the feed position is too far off
// if it is, it will "self correct" by apply a contact voltage and hold it
// for a period of time protionaly to the angle off
void feeder_reset()
{
  // -- fast up to range
  getFeederVal();
  int dang = FEEDER_DOWN_LIMIT-Feeder_val;
  if (dang>=10)
  {
    feeder(FEEDER_RESET_V);
    wait1Msec(dang*FEEDER_RESET_KP);
    feeder(STOP);
  }
  else if(dang<=-10)
  {
    feeder(-FEEDER_RESET_V);
    wait1Msec(-dang*FEEDER_RESET_KP);
    feeder(STOP);
  }
  else
  {
    feeder(STOP);
  }

}

// bat is read to feed if
// Bat_rpm >=0 (must include 0, because when BAT_CMD==STOP, Bat_rpm=0)
// & Bat_val>-25 degree
// this allows to cover stop case as well as pulling and pulling back from top cases
int is_bat_to_feed()
{
  getBatVal();
  // if (Bat_rpm>=0 && Bat_val>0) {return 1;}
  // if (Bat_rpm>=0 && Bat_val>-25) {return 1;}
  if (Bat_rpm>=0 && Bat_val>-15) {return 1;}
  return 0;

}
#endif //SOMNUS_FEEDER_C
