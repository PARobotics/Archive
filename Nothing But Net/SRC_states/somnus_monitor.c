#ifndef SOMNUS_MONITOR_C
#define SOMNUS_MONITOR_C


task somnus_monitor()
{

#if DEBUG == YES
    writeDebugStreamLine("%%    startTask somnus_monitor");
#endif
  int tcur, Bat_dbg_cnt, Feeder_dbg_cnt, Wheel_dbg_cnt;
  //int Intaker_dbg_cnt;

  while (true) {
    tcur = time1[T1];
    ////////////////////////////////////////////////////////////////////////////
    // BAT
    ////////////////////////////////////////////////////////////////////////////
    // -- START monitoring bat
    if (BAT_MONITOR==START )
    {
      getBatVal();                    // got Bat_val (in 1/10-deg)
      Bat_t_last    = tcur;           // initialization of Monitoring
      Bat_val_last = Bat_val;
      Bat_rpm      = 0;
      Bat_rpm_last = 0;
      Bat_dbg_cnt   = 0;              // reset debug count
      BAT_MONITOR = MONITOR ;         // now set the status to MONITOR
      #if DEBUG_BAT > 0
        Bat_t0        = tcur;
        // writeDebugStreamLine("B %3d %3d %4d %4d", 0, Bat_volt, Bat_val, Bat_rpm);
        writeDebugStreamLine("B %3d %3d %4d %4d", 0, (volt1()*6+volt2()*4)/100, Bat_volt, Bat_val);
      #endif
    }                                 // if (BAT_CMD==START )

    // -- Monitor bat (after  time interval BAT_DT has passed)
    else if (BAT_MONITOR==MONITOR && tcur>= Bat_t_last + BAT_DT)
    {
      getBatVal();
      Bat_rpm_last = Bat_rpm;
      Bat_rpm = 60000/360*(Bat_val-Bat_val_last)/(tcur-Bat_t_last);
      Bat_t_last    = tcur;           // now move the frame (current becomes last)
      Bat_val_last = Bat_val;
      Bat_dbg_cnt ++;
      // print out monitoring debug message once every DEBUG_BAT count
      if (DEBUG_BAT > 0 && DEBUG_BAT == Bat_dbg_cnt) {
        Bat_dbg_cnt = 0;
        writeDebugStreamLine("B %3d %3d %4d %4d", tcur-Bat_t0, (volt1()*6+volt2()*4)/10, Bat_volt, Bat_val);
        // writeDebugStreamLine("B %3d %3d %4d %4d", tcur-Bat_t0, Bat_volt, Bat_val, Bat_rpm);
      }
    }                                 //if (BAT_CMD==BAT_MONITOR && tcur>= Bat_t_last + BAT_DT)
    else if (BAT_MONITOR==STOP )      // set speed to zero
    {
      Bat_rpm = 0;
    }


    ////////////////////////////////////////////////////////////////////////////
    // FEEDER
    ////////////////////////////////////////////////////////////////////////////
    // -- START monitoring
    if (FEEDER_MONITOR==START )
    {
      getFeederVal();                    // got Feeder_val1 & Feeder_Val2
      Feeder_t_last    = tcur;           // initialization of Monitoring
      Feeder_val_last  = Feeder_val;
      Feeder_rpm       = 0;
      Feeder_dbg_cnt   = 0;              // reset debug count
      FEEDER_MONITOR = MONITOR ;         // now set the status to MONITOR
#if DEBUG_FEEDER > 0
      Feeder_t0        = tcur;
      writeDebugStreamLine("FS %3d %3d %4d %4d", tcur, Feeder_volt, Feeder_val/10, Feeder_rpm/10);
#endif
    }                                     // if (FEEDER_MONITOR==START )

    // -- Monitor  (after certain time interval FEEDER_DT has passed)
    else if (FEEDER_MONITOR==MONITOR && tcur>= Feeder_t_last + FEEDER_DT)
    {
      getFeederVal();                    // got Feeder_val(in degree units)
      Feeder_rpm = 60000/360*(Feeder_val-Feeder_val_last)/(tcur-Feeder_t_last);
                                         // calculate rpm (rpm)
      Feeder_t_last    = tcur;           // now move the frame (current becomes last)
      Feeder_val_last = Feeder_val;
      Feeder_dbg_cnt ++;
      // print out monitoring debug message once every DEBUG_FEEDER count
      if (DEBUG_FEEDER > 0 && DEBUG_FEEDER == Feeder_dbg_cnt) {
        Feeder_dbg_cnt = 0;
        writeDebugStreamLine("F %3d %3d %4d %4d", tcur-Feeder_t0, Feeder_volt, Feeder_val/10, Feeder_rpm/10);
      }
    }  //if (FEEDER_MONITOR==MONITOR && tcur>= Feeder_t_last + FEEDER_DT)
    else if (FEEDER_MONITOR==STOP )      // set speed to zero
    {
      Feeder_rpm = 0;
    }

    ////////////////////////////////////////////////////////////////////////////
    // WHEEL
    ////////////////////////////////////////////////////////////////////////////
    // -- START monitoring
    if (WHEEL_MONITOR==START )
    {
      getWheelVal();                    // got Wheel_val1 & Wheel_Val2
      Wheel_t_last      = tcur;           // initialization of Monitoring
      Wheel_val_L_last  = Wheel_val_L;
      Wheel_val_R_last  = Wheel_val_R;
      Wheel_val_last    = (Wheel_val_L+Wheel_val_R)/2;
      Gyro_val_last     = Gyro_val;
      Wheel_speed       = 0;
      Wheel_speed_L     = 0;
      Wheel_speed_R     = 0;
      Gyro_speed        = 0;
      Wheel_dbg_cnt     = 0;              // reset debug count
      WHEEL_MONITOR = MONITOR ;         // now set the status to MONITOR
#if DEBUG_WHEEL > 0
      Wheel_t0        = tcur;
        writeDebugStreamLine("L %3d %3d %4d %4d", tcur-Wheel_t0, Wheel_volt, Wheel_val_L/10, Wheel_speed_L);
        writeDebugStreamLine("R %3d %3d %4d %4d", tcur-Wheel_t0, Wheel_volt, Wheel_val_R/10, Wheel_speed_R);
        writeDebugStreamLine("G %3d %3d %4d %4d", tcur-Wheel_t0, Wheel_volt, Gyro_val, Gyro_speed);
#endif
    }                                     // if (WHEEL_MONITOR==START )

    // -- Monitor  (after certain time interval WHEEL_DT has passed)
    else if (WHEEL_MONITOR==MONITOR && tcur>= Wheel_t_last + WHEEL_DT)
    {
      getWheelVal();                    // got Wheel_val (in 0.01 inch)
      // Wheel_speed_L    = 100*(Wheel_val_L-Wheel_val_L_last)/(tcur-Wheel_t_last);   // in 0.1 inch/second
      // Wheel_speed_R    = 100*(Wheel_val_R-Wheel_val_R_last)/(tcur-Wheel_t_last);   // in 0.1 inch/second
      Wheel_speed    = 100*(Wheel_val-Wheel_val_last)/(tcur-Wheel_t_last);   // in 0.1 inch/second
      // Wheel_speed      = (Wheel_speed_L+Wheel_speed_R)/2;   // in 0.1 inch/second
      Gyro_speed       = 100*(Gyro_val -Gyro_val_last)/(tcur-Wheel_t_last);    // in deg/second
      Wheel_t_last     = tcur;           // now move the frame (current becomes last)
      // Wheel_val_L_last = Wheel_val_L;
      // Wheel_val_R_last = Wheel_val_R;
      Wheel_val_last = Wheel_val;
      // Wheel_val        = (Wheel_val_L+Wheel_val_R)/2;
      Gyro_val_last    = Gyro_val;
      Wheel_dbg_cnt ++;
      // print out monitoring debug message once every DEBUG_FEEDER count
      if (DEBUG_WHEEL > 0 && DEBUG_WHEEL == Wheel_dbg_cnt) {
        Wheel_dbg_cnt = 0;
        // writeDebugStreamLine("L %3d %3d %4d %4d", tcur-Wheel_t0, Wheel_volt, Wheel_val_L/10, Wheel_speed_L);
        // writeDebugStreamLine("R %3d %3d %4d %4d", tcur-Wheel_t0, Wheel_volt, Wheel_val_R/10, Wheel_speed_R);
        writeDebugStreamLine("W %3d %3d %4d %4d", tcur-Wheel_t0, Wheel_volt, Wheel_val/10, Wheel_speed);
        writeDebugStreamLine("G %3d %3d %4d %4d", tcur-Wheel_t0, Wheel_volt, Gyro_val, Gyro_speed);
      }
    }                                     //if (WHEEL_MONITOR==MONITOR && tcur>= Wheel_t_last + WHEEL_DT)
    else if (WHEEL_MONITOR==STOP )      // set speed to zero
    {
      Wheel_speed_L = 0;
      Wheel_speed_R = 0;
      Wheel_speed   = 0;
      Gyro_speed    = 0;
    }


    wait1Msec(1);                         // avoid jamming the system with inf loop
  }                                       // end of the infinite while loop
}


#endif //SOMNUS_MONITOR_C
