#ifndef SS_LCD_C
#define SS_LCD_C

void waitForPress()
{
  while (nLCDButtons == 0) {}
  wait1Msec(5);
}

void waitForRelease()
{
  while (nLCDButtons != 0) {}
  wait1Msec(5);
}

void lcd_selection()
{
  // clear the LCD and display filler text
  clearLCDLine(0);
  clearLCDLine(1);

  sprintf(lcdStr1, "8900 %4.1fv", volt1() / 1000.0);
  lcdStr2 =  "starting ...";
  displayLCDString(0, 0, lcdStr1);
  displayLCDString(1, 0, lcdStr2);

  //Wait for center to be pushed
  while (nLCDButtons != CENTERBUTTON) {
    if (SIDE == BLUESIDE) {lcdStr2 = "RIGHT  <    Left";}
    if (SIDE == REDSIDE ) {lcdStr2 = "right  >    LEFT";}
    displayLCDCenteredString(1, lcdStr2);

    waitForPress();
    if (nLCDButtons == LEFTBUTTON) {
      waitForRelease();
      SIDE = BLUESIDE;
      #if DEBUG_LCD>=1
        writeDebugStreamLine("L: left pushed");
      #endif
    }
    if (nLCDButtons == RIGHTBUTTON) {
      SIDE = REDSIDE;
      waitForRelease();
      #if DEBUG_LCD>=1
        writeDebugStreamLine("L: right pushed");
      #endif
    }
  }

  if (SIDE == BLUESIDE) lcdStrSide = "R";
  if (SIDE == REDSIDE ) lcdStrSide = "L";
  // if (SIDE == BLUESIDE) lcdStrSide = "B";
  // if (SIDE == REDSIDE ) lcdStrSide = "R";
  lcdStr2 = "wait for MODE selection ...";
  displayLCDString(1, 0, lcdStr2);
  wait1Msec(250);

  MODE = AUTO_A;
  lcd_mode_selection();
}

void lcd_mode_selection()
{
  int t_pushed;

  //While not selected:
  while (nLCDButtons != CENTERBUTTON) {
    //Cycle:
    if (MODE < 0) {MODE = 6;}
    if (MODE > 6) {MODE = 0;}
    if (MODE == AUTO_A) lcdStrMode = "Auto_A";
    if (MODE == AUTO_B) lcdStrMode = "Auto_B";
    if (MODE == AUTO_C) lcdStrMode = "Auto_C";
    if (MODE == RBT_SKILL) lcdStrMode = "Drv_Skills";
    if (MODE == PRG_SKILL) lcdStrMode = "Prg_Skills";
    if (MODE == TESTING) lcdStrMode = "Testing";
    sprintf(lcdStr2, "<<%s %s>>", lcdStrSide, lcdStrMode);

    // monitor t_push center time
    t_push_center = time1[T1];     // this has to be before and after waitForPress()

    // allow switching between modes
    displayLCDCenteredString(1, lcdStr2);
    waitForPress();
    if (nLCDButtons == LEFTBUTTON) {
      waitForRelease();
      MODE--;
    }
    if (nLCDButtons == RIGHTBUTTON) {
      waitForRelease();
      MODE++;
    }

    t_push_center = time1[T1];
  }

  // long press center button to reset
  while (nLCDButtons == CENTERBUTTON) {
    waitForRelease();
    t_pushed = time1[T1] - t_push_center;

    #if DEBUG_LCD>=1
        writeDebugStreamLine("L: t_push=%6d", t_pushed);
    #endif

    if (t_pushed > 2500) {
      sprintf(lcdStr1, "Resetting ...   ");
      displayLCDString(0, 0, lcdStr1);
      displayLCDString(1, 0, lcdStr2);
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

  lcd_msg_init();
}



void lcd_msg_init()
{
  sprintf(lcdStr1, "8900PX %4d", volt1());
  sprintf(lcdStr2, "M%1d %s ", MODE, lcdStrMode);
}

void lcd_message()
{
  getLiftVal();
  getDumpVal();

  sprintf(lcdStr1, "8900 %4.1fV %4.1fV", volt1() / 1000.0, volt2() / 1000.0);
  sprintf(lcdStr2, "D: %4d L: %4d", DUMP_VAL, LIFT_VAL);

  displayLCDString(0, 0, lcdStr1);
  displayLCDString(1, 0, lcdStr2);
}


#endif  // SS_LCD_C
