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
#define OUT            -1
#define MONITOR         2
#define RESET           1
#define BLUESIDE  	   -1
#define REDSIDE 	      1
#define OPEN            0
#define CLOSE           1
#define AUTO_A          0
#define AUTO_B          1
#define AUTO_C          2
#define AUTO_D          3
#define CORTEX_RESET    99
#define PRG_SKILL       4
#define RBT_SKILL       5
#define TESTING         6
#define MANUAL_UP       2
#define MANUAL_DOWN     3
#define HOLD            4
#define CARRY           5
#define CARRY_HIGH      51
#define TOGGLE          2
#define PRG             6
#define WIGGLE          7

/*
	DEBUG
*/
// -- most debug are turned off
// -- dump and driver sample at 20 msec
//    but debug stream at 20x5 = 100msec
#define DEBUG               YES   // flag; turn ON/OFF debug message
#define DEBUG_LCD           0     // flag; turn ON/OFF debug message for LCD
#define DEBUG_MSG           0     // tunroff landmark and message output

                    /*
                    	MAIN FLOW
                    	Initialize, LCD, Remote related
                    */

void sensor_reset();
void global_variable_reset();

// LCD
int  MODE = AUTO_A;
int  SIDE = REDSIDE;         // Blue or Red side that is selected
int  t_push_center;          // Time used to track long press mid button

string  lcdStr1;                // hold the 16 character string for LCD line 1
string  lcdStr2;                // hold the 16 character string for LCD line 2
string  lcdStrSide;             // 'B' or 'R' indicates the field selection
string  lcdStrMode;             // lcdStr for Mode

#define LEFTBUTTON     1
#define CENTERBUTTON   2
#define RIGHTBUTTON    4

//Wheel monitor
#define M_PI 3.14159265358979323846
int WHEEL_MONITOR = 0;
// .1 inch, .,1deg as units
typedef struct {
	int    raw      [3] ;  //raw sensor data [l-wheel, r-wheel, gyro]
	int    raw_last [3] ;  //
	float  pos      [3] ;
	float  move     [3] ;  // -- track dist, lateral and rotation from pre
	float  pos_pre  [3] ;
	float  pos_delta[3] ;  // delta X, Y, R, Rgyro
	float  speed    [2] ;
  int    method       ;  // method for detla_R   1: use gyro, 2: use wheels,
  float  pi2d         ;           // .1 deg per pi
  float  tick2inch    ;   // 0.1 inch per tick
  int    t_last       ;
  int    dbg_cnt      ;
  int    DT           ;
  int    DX           ;               // 12" between the wheel
} drv_train;
drv_train DRV;                      // create drive train data structure

// int  Wheel_t_last      = 0;
// int  Y_disp            = 0;    // in 0.01 inch
// int  R_disp            = 0;    // in 1/10 degree
// int  Y_disp_last       = 0;    // in 0.01 inch
// int  R_disp_last       = 0;    // in 1/10 degree
// int  X_vel             = 0;    // in in/sec
// int  Y_vel             = 0;    // in in/sec
// int  R_vel             = 0;    // in deg/sec
// int  GYRO_VAL          = 0;

#define MOTION_DRV_THRESHOLD  10      // threshold of joystick sensitivity
#define  DRIVE_VOLT 127                 // don't want full speed in drive

//MOVE Y
#define MOVEY_DT          100
#define MOVEY_BRAKE_V     -20
#define MOVEY_DIST1       60
#define MOVEY_DIST0       20
#define MOVEY_V1          127
#define MOVEY_Y1          200
#define MOVEY_Y2          20
#define MOVEY_DY          10
#define MOVEY_KV          0.1
#define MOVEY_V_DEF       60
#define MOVEY_V_MIN       -50
#define MOVEY_V_MAX       127

int MOVEY_VCMD = 0;

//Smart rotate
int ROTATE_V1 = 127;
#define ROTATE_ANG1       200
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

// Lift monitor
int LIFT_COMMAND = STOP;
int LIFT_MONITOR = 0;
int LIFT_VAL;
int LIFT_V = 0;
// int lift_last_monitor;
int LIFT_LAST_VAL = 0;
int LIFT_LAST_V = 0;


// Dump monitor

int DUMP_MONITOR = 0;
int DUMP_COMMAND = STOP;
int DUMP_VAL;
int DUMP_OMEGA = 0;
int dump_last_monitor;
int DUMP_LAST_VAL = 0;
int DUMP_LAST_OMEGA = 0;
int DUMP_VCMD;

//Accelerometer
int ACC_MONITOR = 0;
int COLLIDED = NO;

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


//Sensors
int  volt1();                            // main battery in mV
// int  volt2();                            // second battery in mV (scale factor adjusted to 286)
void getWheelVal();                      // Wheel_val in 0.01 inch
void getLiftVal();
void getDumpVal();
int getAccelerometerVal();
void hasCollided();

//Movement
//void move(int V, int H);
//void rotate();
//int rotate_by(int angle, int tlimit);


//Dumper
void dumper(int vol);
//void one_dump();
//void dump_down_for(int tlimit);
//void dump_dump_for(int tlimit);

//Lift
void transmission(int status);
void lift_up();
void lift_down();
//void one_lift();
void lift_piston(int status);
//void lift_grabbed();
//bool trans_successful();


#endif //SS_H
