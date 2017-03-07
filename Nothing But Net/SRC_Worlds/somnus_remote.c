#ifndef SOMNUS_REMOTE_C
#define SOMNUS_REMOTE_C

// 1. monitor all the 7x and 8x buttons on second remote and 7U and 8U buttons on main remote
// 2. monitor the press and release of the 7U button on the main remote (as shift)
// 3. also update LCD string automatically




task somnus_remote()
{
  // local variables
  int BUTTON_PUSH[10]   = {0,0,0,0,0,0,0,0,0,0};

  #if DEBUG == YES
    writeDebugStreamLine("%%    startTask somnus_remote");
  #endif

  while(1) {

    // monitor press and release of L/U/R/D buttons on both remotes
    if(vexRT[Btn7LXmtr2] == 1) { BUTTON_PUSH[0] = 1;  }
    if(vexRT[Btn7UXmtr2] == 1) { BUTTON_PUSH[1] = 1;  }
    if(vexRT[Btn7RXmtr2] == 1) { BUTTON_PUSH[2] = 1;  }
    if(vexRT[Btn7DXmtr2] == 1) { BUTTON_PUSH[3] = 1;  }
    if(vexRT[Btn5DXmtr2] == 1) { BUTTON_PUSH[4] = 1;  } //Left
    if(vexRT[Btn8UXmtr2] == 1) { BUTTON_PUSH[5] = 1;  }
    if(vexRT[Btn6DXmtr2] == 1) { BUTTON_PUSH[6] = 1;  } //Right
    if(vexRT[Btn8DXmtr2] == 1) { BUTTON_PUSH[7] = 1;  }
    if(vexRT[Btn7U] == 1)      { BUTTON_PUSH[8] = 1;  }
    if(vexRT[Btn8U] == 1)      { BUTTON_PUSH[9] = 1;  }

    if(vexRT[Btn7LXmtr2] == 0 && BUTTON_PUSH[0] == 1) { PUSH_RELEASE[0] = 1; BUTTON_PUSH[0] = 0;  }
    if(vexRT[Btn7UXmtr2] == 0 && BUTTON_PUSH[1] == 1) { PUSH_RELEASE[1] = 1; BUTTON_PUSH[1] = 0;  }
    if(vexRT[Btn7RXmtr2] == 0 && BUTTON_PUSH[2] == 1) { PUSH_RELEASE[2] = 1; BUTTON_PUSH[2] = 0;  }
    if(vexRT[Btn7DXmtr2] == 0 && BUTTON_PUSH[3] == 1) { PUSH_RELEASE[3] = 1; BUTTON_PUSH[3] = 0;  }
    if(vexRT[Btn5DXmtr2] == 0 && BUTTON_PUSH[4] == 1) { PUSH_RELEASE[4] = 1; BUTTON_PUSH[4] = 0;  }
    if(vexRT[Btn8UXmtr2] == 0 && BUTTON_PUSH[5] == 1) { PUSH_RELEASE[5] = 1; BUTTON_PUSH[5] = 0;  }
    if(vexRT[Btn6DXmtr2] == 0 && BUTTON_PUSH[6] == 1) { PUSH_RELEASE[6] = 1; BUTTON_PUSH[6] = 0;  }
    if(vexRT[Btn8DXmtr2] == 0 && BUTTON_PUSH[7] == 1) { PUSH_RELEASE[7] = 1; BUTTON_PUSH[7] = 0;  }
    if(vexRT[Btn7U] == 0      && BUTTON_PUSH[8] == 1) { PUSH_RELEASE[8] = 1; BUTTON_PUSH[8] = 0;  }
    if(vexRT[Btn8U] == 0      && BUTTON_PUSH[9] == 1) { PUSH_RELEASE[9] = 1; BUTTON_PUSH[9] = 0;  }

    led_ctrl(LED1,ON);
    is_loaded();
    is_queued();
    if (Ball0==1 && Ball1==1) led_ctrl(LED2,FAST);
    if (Ball0==1 && Ball1==0) led_ctrl(LED2,SLOW);
    if (Ball0==0 && Ball1==1) led_ctrl(LED2,ON);
    if (Ball0==0 && Ball1==0) led_ctrl(LED2,OFF);

    // update lcd strings
    displayLCDString(0,0, lcdStr1);
    displayLCDString(1,0, lcdStr2);
    wait1Msec(10); //Maybe go a little longer???
  }
}

void led_ctrl(tSensors theLed, int option)
{
  int dt, on_off_time=0;
  if (option==ON)   {SensorValue[theLed] = false; return;}
  if (option==OFF)  {SensorValue[theLed] = true; return;}
  if (option==SLOW) on_off_time=LED_SLOW_DT;
  if (option==FAST) on_off_time=LED_FAST_DT;

  if (theLed==LED1) dt = time1[T1]-LAST_LED_1;
  if (theLed==LED2) dt = time1[T1]-LAST_LED_2;

  if (dt>2*on_off_time) {
    if (theLed==LED1) LAST_LED_1 = time1[T1];
    if (theLed==LED2) LAST_LED_2 = time1[T1];
    SensorValue[theLed] = true;
  }
  else if (dt>on_off_time) {
    SensorValue[theLed] = false;
  }
  else {
    SensorValue[theLed] = true;
  }
}
#endif //SOMNUS_REMOTE_C
