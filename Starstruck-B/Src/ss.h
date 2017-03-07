#ifndef SS_H
#define SS_H

/*
	EASY NAMES
	For better code readability
*/

#define YES             1
#define NO              0
#define START					  1
#define STOP            0
#define LEFT           -1
#define RIGHT           1
#define FWD             1
#define BKWD           -1
#define UP              1
#define DOWN           -1
#define IN              1
#define MONITOR         2
#define RESET           1
#define OUT            -1
#define BLUESIDE  	   -1
#define REDSIDE 	      1
#define POLETILE       -1
#define NORMTILE        1
#define GRAY            0
#define COLORED         1
#define OPEN						1
#define CLOSE          -1
#define HOLD_OPEN       2
#define HOLD_CLOSE      3
#define AUTO_A      0
#define AUTO_B      1
#define AUTO_C      2
#define AUTO_D      3
#define PRG_SKILL   4
#define RBT_SKILL   5
#define TESTING      6
#define CARRY         5
#define UP_MANUAL     3
#define DOWN_MANUAL   4

/*
	DEBUG
*/

#define DEBUG               YES   // flag; turn ON/OFF debug message
#define DEBUG_LCD           1     // flag; turn ON/OFF debug message for LCD
#define DEBUG_DUMP          2     // flag; turn ON/OFF debug message for LCD
#define DEBUG_CLAW          2     // flag; turn ON/OFF debug message for LCD
#define DEBUG_WHEEL         4     // flag; turn ON/OFF debug message for LCD
#define WHEEL_DT            20     // wheel update every 0.05 seconds

/*
	MAIN FLOW
	Initialize, LCD, Remote related
*/

void sensor_reset();

// LCD
int  MODE = AUTO_A;
int  TILE = NORMTILE;
int  SIDE = BLUESIDE;        // Blue or Red side that is selected
int  t_push_center;          // Time used to track long press mid button

string  lcdStr1;                // hold the 16 character string for LCD line 1
string  lcdStr2;                // hold the 16 character string for LCD line 2
string  lcdStrSide;             // 'B' or 'R' indicates the field selection
string  lcdStrMode;             // lcdStr for Mode

#define LEFTBUTTON     1
#define CENTERBUTTON   2
#define RIGHTBUTTON    4

//Wheel monitor
int WHEEL_MONITOR = 0;
int  Wheel_t_last      = 0;
int  X_disp            = 0;    // in 0.01 inch
int  Y_disp            = 0;    // in 0.01 inch
int  R_disp            = 0;    // in 1/10 degree
int  X_disp_last       = 0;    // in 0.01 inch
int  Y_disp_last       = 0;    // in 0.01 inch
int  R_disp_last       = 0;    // in 1/10 degree
int  X_vel             = 0;    // in in/sec
int  Y_vel             = 0;    // in in/sec
int  R_vel             = 0;    // in deg/sec
int  GYRO_VAL          = 0;

#define MOTION_DRV_THRESHOLD  10      // threshold of joystick sensitivity
#define  DRIVE_VOLT 127                 // don't want full speed in drive

//MOVE X
#define MOVEX_DT          100
#define MOVEX_BRAKE_V     -20
#define MOVEX_DIST1       300
#define MOVEX_DIST0       100
#define MOVEX_V1          127
#define MOVEX_Y1          160
#define MOVEX_Y2          20
#define MOVEX_DY          10
#define MOVEX_KV          0.1
#define MOVEX_V_DEF       40
#define MOVEX_V_MIN       -50
#define MOVEX_V_MAX       127

int MOVEX_VCMD = 0;

//MOVE Y
#define MOVEY_DT          100
#define MOVEY_BRAKE_V     -20
#define MOVEY_DIST1       300
#define MOVEY_DIST0       100
#define MOVEY_V1          127
#define MOVEY_Y1          160
#define MOVEY_Y2          20
#define MOVEY_DY          10
#define MOVEY_KV          0.1
#define MOVEY_V_DEF       40
#define MOVEY_V_MIN       -50
#define MOVEY_V_MAX       127

int MOVEY_VCMD = 0;

//Smart rotate
int ROTATE_V1 = 127;
#define ROTATE_ANG1       450
#define ROTATE_ANG0       50
#define ROTATE_BRAKE_V    -20
#define ROTATE_BRAKE_DT   50
#define ROTATE_Y1         160
#define ROTATE_Y2         20
#define ROTATE_DY         10
#define ROTATE_KV         0.2
#define ROTATE_V_DEF      40
#define ROTATE_V_MIN      -50
#define ROTATE_V_MAX      127
int ROTATE_VCMD = 0;

// Dump monitor
#define DUMP_MIN_REAL             3100     //In ticks

int DUMP_MONITOR = 0;
int DUMP_VAL;
int DUMP_OMEGA = 0;
int dump_last_monitor;
int DUMP_LAST_VAL = 0;
int DUMP_LAST_OMEGA = 0;
int DUMP_VCMD;
int DUMP_COMMAND = STOP;

int CLAW_MONITOR = 0;
int CLAW_VAL_L;
int CLAW_LAST_VAL_L = 0;
int CLAW_LAST_OMEGA_L = 0;
int CLAW_OMEGA_L = 0;
int CLAW_VAL_R;
int CLAW_LAST_VAL_R = 0;
int CLAW_LAST_OMEGA_R = 0;
int CLAW_OMEGA_R = 0;
int claw_last_monitor;
int CLAW_COMMAND = STOP;

/*
	CONTROL, SENSORS
*/

// Constants
#define RPM_393_HS    392      // # ticks per revolution
#define RPM_393       627.2
#define RPM_SE        360

/*
    Autonomous parameters
*/

#define WIGGLE_V            127
#define WIGGLE_DT           100

//Programming skills
#define PRG_MOVE1          -1000
#define PRG_MOVE2           1500
#define PRG_ROTATE1        -900
#define PRG_MOVE3           2400
#define PRG_MOVE4           1000
#define PRG_MOVE5           6000
#define PRG_ROTATE2        -1800
#define PRG_MOVE6           2400
#define PRG_MOVE7           5500
#define PRG_MOVE8          -7000
#define PRG_ROTATE3         900
#define PRG_MOVE9           4000
#define PRG_ROTATE4        -900
#define PRG_ROTATE5        -450
#define PRG_MOVE10          2000
#define PRG_MOVE11         -1000
#define PRG_MOVE12         -700
#define PRG_MOVE13          6000
#define PRG_MOVE14         -1000
#define PRG_MOVE15          2500
#define PRG_MOVE16          1000
#define PRG_MOVE17         -7000

/*
	Function + Tasks Wrappers
*/

//LCD
void waitForPress();
void waitForRelease();
void lcd_selection();          // select side and then call lcd_mode_selection// to modify
void lcd_mode_selection();     // select MODE // to modify
//void lcd_message();            // display message ON LCD
void lcd_msg_init();            // display message INIT

//Utilities
int is_timed_out(int tstop);               // time out at tstop = time1[T1]
//int is_bailed_out();                       // bailout

//Sensors
int  volt1();                            // main battery in mV
// int  volt2();                            // second battery in mV (scale factor adjusted to 286)
void getWheelVal();                      // Wheel_val in 0.01 inch
void getDumpVal();
//Movement
void move(int V, int H, int X);
void move_x(int vol);
void move_y(int vol);
void move_x_for(int vol, int tlimit);
void move_y_for(int vol, int tlimit);
void rotate_for(int vol, int tlimit);
void rotate(int vol);
int rotate_by(int angle, int tlimit);
int move_x_by(int dist, int tlimit);
int move_y_by(int dist, int tlimit);
int move_x_by_fast(int dist, int tlimit);
int move_y_by_fast(int dist, int tlimit);
int rotate_by_fast(int dist, int tlimit);

//Dumper
void dumper(int vol);
void dump_down();
void dump_dump();
void dump_down_hold();
void dump_shoot();
void dump_carry();
//int vcmd_ctrl_xv(int x, int v, int x_target, int v_target, double kx, double kv, int v_def, int v_min, int v_max);
//int vcmd_ctrl_linear_band(int x, int y, int x1, int x2, int y1, int y2, int dy, double kv, int v_def, int v_min, int v_max);
void one_dump();
void dump_down_for(int tlimit);
void dump_dump_for(int tlimit);

//Autonomous
void auto_A();
void auto_B();
void auto_C();
void prg_skills();

#endif //SS_H
