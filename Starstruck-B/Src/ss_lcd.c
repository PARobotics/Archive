#ifndef SS_LCD_C
#define SS_LCD_C

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

void lcd_selection(){
  // clear the LCD and display filler text
	clearLCDLine(0);
	clearLCDLine(1);

  sprintf(lcdStr1, "8900 %4.1fv %4.1fv", volt1()/1000.0, volt2()/1000.0);
	lcdStr2 =  "starting ...";
	displayLCDString(0,0, lcdStr1);
  displayLCDString(1,0, lcdStr2);

  //Wait for center to be pushed
  while(nLCDButtons != CENTERBUTTON) {
		if (SIDE == BLUESIDE) {lcdStr2 = "BLUE <       Red";}
		if (SIDE == REDSIDE ) {lcdStr2 = "Blue      >  RED";}
		displayLCDCenteredString(1, lcdStr2);

		waitForPress();
		if(nLCDButtons == LEFTBUTTON) {
			waitForRelease();
			SIDE = BLUESIDE;
		}
		if(nLCDButtons == RIGHTBUTTON) {
			SIDE = REDSIDE;
      waitForRelease();
		}
  }

  if (SIDE == BLUESIDE) lcdStrSide = "B";
  if (SIDE == REDSIDE ) lcdStrSide = "R";
  lcdStr2 = "wait for MODE selection ...";
  displayLCDString(1,0, lcdStr2);
  wait1Msec(250);

  MODE = AUTO_A;
  lcd_mode_selection();
}

void lcd_mode_selection(){
  int t_pushed;

  //While not selected:
	while(nLCDButtons != CENTERBUTTON){
    //Cycle:
    if (MODE<0) {MODE=4;}
    if (MODE>4) {MODE=0;}
    if (MODE==AUTO_A) lcdStrMode = "Auto_A";
    if (MODE==AUTO_B) lcdStrMode = "Auto_B";
    if (MODE==AUTO_C) lcdStrMode = "Auto_C";
    if (MODE==PRG_SKILL) lcdStrMode = "Prg_Skills";
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

  lcd_msg_init();
}



void lcd_msg_init()
{
  sprintf(lcdStr1, "8900B %4.1fv %4.1fv", volt1()/1000.0, volt2()/1000.0);
  sprintf(lcdStr2, "M%1d %s ",MODE,lcdStrMode);
}

void lcd_message()
{
 	sprintf(lcdStr1, "8900B %4.1fv %4.1fv", volt1()/1000.0, volt2()/1000.0);
  sprintf(lcdStr2, "D: %4d", DUMP_VAL);
}


#endif  // SS_LCD_C
