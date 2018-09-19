#ifndef CONFIG_C
#define CONFIG_C


//Configure preauton
void preAutonProcedure(){
  //This code runs during preauton
}

void userControlProcedure(){
  //This code executes inside the user control while loop
}


//Set up push release buttons
#define USE_PR_BUTTON  1
#define NUM_PR_BUTTONS  7 //Only include if using remote

#define Btn7R_Main 0
#define Btn5U_Main 1
#define Btn5D_Main 2
#define Btn7L_Main 3
#define Btn8R_Main 4
#define Btn8U_Main 5
#define Btn8D_Main 6


void setUpButtons(){ //Only include if using remote
  addPrButton(Btn7R_Main, Btn7R);
  addPrButton(Btn5D_Main, Btn5D);
  addPrButton(Btn5U_Main, Btn5U);
  addPrButton(Btn7L_Main, Btn7L);
  addPrButton(Btn8R_Main, Btn8R);
  addPrButton(Btn8U_Main, Btn8U);
  addPrButton(Btn8D_Main, Btn8D);
}

//Configure Sensors
#define USE_SECOND_BATTERY 1

#define USE_LCD 1

void lcdGenerateMessage(){
  sprintf(lcdStr1, "8900 %4.1fV %4.1fV", getMainBatteryVoltage() / 1000.0, getSecondBatteryVoltage() / 1000.0);
  sprintf(lcdStr2, "%4d", liftSensor.val);
}

/*
  MOTOR CONTROL
*/

//Slew Rate
#define USE_SLEW  1 //Disable if slew interferes with move functions or slows robot down
int MOTOR_SLEW[MOTOR_NUM] = {255, 40, 40, 40, 40, 40, 40, 255, 255, 255};

#define USE_MOVE 1

/*
  DEBUGGING TOGGLES
*/

#define DEBUG_SLEW  0
#define DEBUG_MOVE 0
#define DEBUG_REMOTE 0

#define BAILOUT_BUTTON Btn8L
void bailOut(){
	CONE_LIFT_COMMAND = STOP;
	SWING_ARM_COMMAND = STOP;
  clawIsClosed = 0;
  clawIsOpened = 0;
}

void move(int V, int H, int X){
  motorReq[M_WHEEL_L1] = BOUND(V + H + X, -127, 127);
  motorReq[M_WHEEL_L2] = BOUND(V + H, -127, 127);
  motorReq[M_WHEEL_L3] = BOUND(V + H - X, -127, 127);
  motorReq[M_WHEEL_R1] = BOUND(V - H - X, -127, 127);
  motorReq[M_WHEEL_R2] = BOUND(V - H, -127, 127);
  motorReq[M_WHEEL_R3] = BOUND(V - H + X, -127, 127);
}


#endif
