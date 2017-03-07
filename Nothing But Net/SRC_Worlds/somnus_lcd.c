#ifndef SOMNUS_LCD_C
#define SOMNUS_LCD_C
void waitForPress()
{
  while(nLCDButtons == 0){}
  wait1Msec(5);
}

void waitForRelease()
{
  while(nLCDButtons != 0){}
  wait1Msec(5);
}


void lcd_selection()
{

  // clear the LCD and show some junk first
	clearLCDLine(0);
	clearLCDLine(1);
	// lcd_message();
  sprintf(lcdStr1, "8900 %4.1fv %4.1fv", volt1()/1000.0,volt2()/1000.);
	lcdStr2 =  "starting ...";
	displayLCDString(0,0, lcdStr1);
  displayLCDString(1,0, lcdStr2);

  // now wait till center button has been pushed -- adjust display to current selection accordingly
  while(nLCDButtons != CENTERBUTTON) {

		if (SIDE == BLUESIDE) {lcdStr2 = "BLUE <       Red";}
		if (SIDE == REDSIDE ) {lcdStr2 = "Blue      >  RED";}
		displayLCDCenteredString(1, lcdStr2);
    #if DEBUG_LCD>=1
        writeDebugStreamLine("in loop, waiting for press");
    #endif
		waitForPress();
		if(nLCDButtons == LEFTBUTTON) {
			waitForRelease();
			SIDE = BLUESIDE;
      #if DEBUG_LCD>=1
          writeDebugStreamLine("L: left pushed");
      #endif
		}
		if(nLCDButtons == RIGHTBUTTON) {
			SIDE = REDSIDE;
      waitForRelease();
      #if DEBUG_LCD>=1
          writeDebugStreamLine("L: right pushed");
      #endif
		}
  }

	if (SIDE == BLUESIDE) {lcdStrSide = "B"; }
  if (SIDE == REDSIDE ) {lcdStrSide = "R"; }
  lcdStr2 = "wait for MODE selection ...";
  displayLCDString(1,0, lcdStr2);
  wait1Msec(250);

  // MODE = TEST_A;       // default is testing 1
  MODE = AUTO_A;       // default is testing 1
  lcd_mode_selection();
}

void lcd_mode_selection()
{
  int t_pushed;
  #if DEBUG_LCD>=1
      writeDebugStreamLine("L: in lcd_mode_selection");
  #endif
  // now select auton/program skills mode -- update display accordingly
	while(nLCDButtons != CENTERBUTTON)
	{
    // bound the MODE and update mode string
    if (MODE<0) {MODE=7;}    // LEFT and RIGHT arrow circles 0->5
    if (MODE>7) {MODE=0;}
    if (MODE==AUTO_A)    {lcdStrMode = "Auto_A    ";}
    if (MODE==AUTO_B)    {lcdStrMode = "Auto_B    ";}
    if (MODE==AUTO_C)    {lcdStrMode = "Auto_C    ";}
    if (MODE==AUTO_D)    {lcdStrMode = "Auto_D    ";}
    if (MODE==PRG_SKILL) {lcdStrMode = "Prg_Skills";}
    if (MODE==RBT_SKILL) {lcdStrMode = "Rbt_Skills";}
    if (MODE==TEST_A)    {lcdStrMode = "Test-A    ";}
    if (MODE==TEST_B)    {lcdStrMode = "Test-B    ";}
    sprintf(lcdStr2, "<<%s %s>>",lcdStrSide,lcdStrMode);

    // monitor t_push center time
    t_push_center = time1[T1];     // this has to be before and after waitForPress()

    // allow switching between modes
    displayLCDCenteredString(1, lcdStr2);
    waitForPress();
    if(nLCDButtons == LEFTBUTTON) {
      waitForRelease();
      MODE--;
    }
    if(nLCDButtons == RIGHTBUTTON) {
      waitForRelease();
      MODE++;
    }

    t_push_center = time1[T1];
  }

  // long press center button to reset; otherwise center acts as ok
  while(nLCDButtons == CENTERBUTTON)
	{
    // writeDebugStreamLine("center pushed %6d",t_push_center );
    waitForRelease();
    t_pushed = time1[T1]-t_push_center;

    #if DEBUG_LCD>=1
        writeDebugStreamLine("L: t_push=%6d",t_pushed);
    #endif
    if (t_pushed>2500) {
        sprintf(lcdStr1, "Resetting ...   ");
        displayLCDString(0,0, lcdStr1);
        displayLCDString(1,0, lcdStr2);
        sensor_reset();
        global_variable_reset();
        // lcd_selection();
        sprintf(lcdStr1, "Reset, start over");
        wait1Msec(1000);
        lcd_selection();
        return;
    }
    wait1Msec(5);
  }
  // lcd_msg_init();
  lcd_bat_adj();
}

void lcd_bat_adj()
{
  int t_pushed;
  #if DEBUG_LCD>=1
      writeDebugStreamLine("L: in lcd_bat_adj");
  #endif
  // now select auton/program skills mode -- update display accordingly
	while(nLCDButtons != CENTERBUTTON)
	{
    sprintf(lcdStr2, "<<-   %3d    +>>",BAT_LONG_PULL_ANG_MAX_BASE);

    // monitor t_push center time
    t_push_center = time1[T1];     // this has to be before and after waitForPress()

    // allow switching between modes
    displayLCDCenteredString(1, lcdStr2);
    waitForPress();
    if(nLCDButtons == LEFTBUTTON) {
      waitForRelease();
      BAT_LONG_PULL_ANG_MAX_BASE -= 8;
      BAT_SKILLS_HIT_V_BASE -= 4;
      // BAT_LONG_PULL_ANG2 -= 5;
      // BAT_LONG_PULL_ANG2 -= 8;
    }
    if(nLCDButtons == RIGHTBUTTON) {
      waitForRelease();
      BAT_LONG_PULL_ANG_MAX_BASE += 8;
      BAT_SKILLS_HIT_V_BASE += 4;
      // BAT_LONG_PULL_ANG2 += 8;
    }
    t_push_center = time1[T1];
  }

  // long press center button to reset; otherwise center acts as ok
  while(nLCDButtons == CENTERBUTTON)
	{
    // writeDebugStreamLine("center pushed %6d",t_push_center );
    waitForRelease();
    t_pushed = time1[T1]-t_push_center;

    BAT_LONG_PULL_ANG_MAX = BAT_LONG_PULL_ANG_MAX_BASE  ; // update the new value
    BAT_SKILLS_HIT_V  = BAT_SKILLS_HIT_V_BASE;

    #if DEBUG_LCD>=1
        writeDebugStreamLine("L: t_push=%6d",t_pushed);
    #endif
    if (t_pushed>2500) {
        sprintf(lcdStr1, "Resetting ...   ");
        displayLCDString(0,0, lcdStr1);
        displayLCDString(1,0, lcdStr2);
        sensor_reset();
        global_variable_reset();
        sprintf(lcdStr1, "Reset, start over");
        wait1Msec(1000);
        lcd_selection();
        return;
    }
    wait1Msec(5);
  }
  lcd_msg_init();
}

void lcd_msg_init()
{
  #if DEBUG==YES
    writeDebugStreamLine("%%Initializing ...",time1[T1]);
    writeDebugStreamLine("%% batt1   = %3.2fv", volt1()/1000.);
    writeDebugStreamLine("%% batt2   = %3.2fv",  volt2()/1000.);
    writeDebugStreamLine("%% mode    = %1d   ", MODE);
    writeDebugStreamLine("%% time    = %4d   ", time1[T1]);
    writeDebugStreamLine("%% Wheel_L = %4d   ", Wheel_val_L);
    writeDebugStreamLine("%% Wheel_R = %4d   ", Wheel_val_R);
    writeDebugStreamLine("%% Bat     = %4d   ", Bat_val);
    writeDebugStreamLine("%% Intaker = %4d   ", Intaker_val);
    writeDebugStreamLine("%% Gryo    = %4d   ", Gyro_val);
  #endif
  sprintf(lcdStr1, "8900PX %4d %4d", volt1(),volt2());
  sprintf(lcdStr2, "M%1d %s  %3d",MODE,lcdStrMode,BAT_LONG_PULL_ANG_MAX);
  // displayLCDString(0,0, lcdStr1);
  // displayLCDString(1,0, lcdStr2);
}



void lcd_message()
{
  getWheelVal() ;
  getFeederVal();
  getBatVal();
  getUS0Val() ;
  getUS1Val() ;
  getFeederVal();
  getIntakerVal();
  getBatVal();

  // for mid and short we displace only relative volt change
  // for long we acutally display BAT_LONG_HIT_V_2
  if (DIST==SHORT) sprintf(lcdStr1, "%1d %2d %2d %2d ",DIST-100,volt1()/100,volt2()/100, BAT_SHORT_HIT_V);
  if (DIST==MEDIUM) sprintf(lcdStr1, "%1d %2d %2d %2d ",DIST-100,volt1()/100,volt2()/100, BAT_MEDIUM_HIT_V);
  if (DIST==LONG) sprintf(lcdStr1, "%1d %2d %2d %2d ",DIST-100,volt1()/100,volt2()/100, BAT_LONG_PULL_ANG_MAX);
  // sprintf(lcdStr2, "%3d %3d %3d %3d",Gyro_val/10,Wheel_val/10,Feeder_val,Intaker_val);
  sprintf(lcdStr2, "%3d %3d %3d %3d",Gyro_val/10,Wheel_val_L/10,Wheel_val_R/10,Bat_val);
  //sprintf(lcdStr2, "%3d %3d %3d %3d",Gyro_val/10,Wheel_val/10,Feeder_val,Bat_val);

  // displayLCDString(0,0, lcdStr1);
  // displayLCDString(1,0, lcdStr2);

}

#endif  // SOMNUS_LCD_C
