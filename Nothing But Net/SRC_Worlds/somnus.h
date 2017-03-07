#ifndef SOMNUS_H
#define SOMNUS_H

///////////////////////////////////////////////////////////////////////////////
// EASY NAMES
///////////////////////////////////////////////////////////////////////////////
#define YES             1
#define NO              0
#define STOP            0
#define LEFT           -1
#define RIGHT           1
#define FWD             1
#define BKWD           -1
#define OPEN            1
#define CLOSE          -1
#define UP              1
#define DOWN           -1
#define IN              1
#define OUT            -1
#define PULL            1
#define PUSH           -1
#define BLUESIDE  	   -1
#define REDSIDE 	      1
#define ON              1
#define OFF             0
#define START           1
#define MONITOR         2
#define FEED            1
#define SINGLE      100
#define SHORT       101
#define MEDIUM      102
#define SKILLS      106
#define LONG        103
#define SLOW        2
#define FAST        3
#define AUTO_A      0
#define AUTO_B      1
#define AUTO_C      2
#define AUTO_D      3
#define PRG_SKILL   4
#define RBT_SKILL   5
#define TEST_A      6
#define TEST_B      7

///////////////////////////////////////////////////////////////////////////////
// DEBUG
///////////////////////////////////////////////////////////////////////////////
#define DEBUG               YES   // flag; turn ON/OFF debug message
#define DEBUG_BALL          YES   // debug ball loaded, queued and ball count
#define DEBUG_LCD           YES   // debug LCD

///////////////////////////////////////////////////////////////////////////////
// MAIN FLOW -- initialize, LCD, remote related
///////////////////////////////////////////////////////////////////////////////
// -- lcd, side and mode selection
void sensor_reset();
void global_variable_reset();
int  MODE = 0;               // id of the auton and other modes selected
int  SIDE = BLUESIDE;        // Blue or Red side that is selected
int  t_push_center;          // time -- used to trank long press mid button

// -- lcd related variables
string  lcdStr1;                // hold the 16 character string for LCD line 1
string  lcdStr2;                // hold the 16 character string for LCD line 2
string  lcdStrSide;             // 'B' or 'R' indicates the field selection
string  lcdStrMode;             // lcdStr for Mode

#define LEFTBUTTON     1        // button names
#define CENTERBUTTON   2
#define RIGHTBUTTON    4

// -- main operation modes control
//    including ball status and led status control
int PUSH_RELEASE[10]  = {0,0,0,0,0,0,0,0,0,0};
int DIST;                       // = SHORT, MEDIUM, LONG
int Ball0         = 0;          //0/1 ball 1 is loaded or not
int Ball1         = 0;          //0/1 ball 2 is queued or not
int FEEDER_ON     = ON;         // =ON/OFF to allow feeder to be on
float VOLT_SCALE = 1.0;


//  -- LED control
#define LED_SLOW_DT       400
#define LED_FAST_DT       100
int LAST_LED_1 = 0;
int LAST_LED_2 = 0;
void led_ctrl(tSensors theLed, int option);

// --
#define ROT_STEP_VOL   70      // hold 50 msec at was 50
#define ROT_STEP_DT    50      // msec


///////////////////////////////////////////////////////////////////////////////
// CONTROL, SENOSR
///////////////////////////////////////////////////////////////////////////////
// -- constants
#define RPM_393_HS    392      // # ticks per revolution
#define RPM_393       627.2
#define RPM_SE        360

// -- main control switches
#define BAT_DT           10    // time interval between each monitor frame in msec
#define DEBUG_BAT         1    // print out time inteval = DEBUG_BAT*BAT_DT (0) disable it
int     BAT_MONITOR  = STOP;   // STOP/START that starts/ends monitoring
int     BAT_CMD      = STOP;   // STOP/BAT1/???? control the " smartBat task"

#define FEEDER_DT          10  // time interval between each monitor frame in msec
#define DEBUG_FEEDER        0  // print out time inteval = DEBUG_FEEDER*FEEDER_DT (0) disable it
int     FEEDER_MONITOR = STOP; // STOP/START that starts/ends monitoring
int     FEEDER_CMD     = STOP; // STOP/FEED control the " smartFeeder task"

#define INTAKER_DT         50  // time interval between each monitor frame in msec (can be slower)
#define DEBUG_INTAKER       4  // print out time inteval = DEBUG_INTAKER*INTAKER_DT (0) disable it
int     INTAKER_MONITOR = STOP;// control parameter that start/end monitoring

// #define WHEEL_DT        10      // time interval between each monitor frame in msec
#define WHEEL_DT        20      // time interval between each monitor frame in msec -- use larger # due to jerk motion
#define DEBUG_WHEEL     1       // print out time inteval = DEBUG_WHEEL*WHEEL_DT (0) disable it
int     WHEEL_MONITOR   = STOP; // STOP/START that starts/ends monitoring

// -- BAT monitoring variables
int     Bat_val      = 0;    // sensor value in 1/10 deg from SE_BAT
int     Bat_rpm      = 0;    // RPM1 value from SE_BAT  10xRPM
int     Bat_rpm_last = 0;    // RPM1 value from SE_BAT  10xRPM
int     Bat_t0        = 0;   // t0
int     Bat_t_last    = 0;   // cache of the time at last monitor frame (used to calculate rpm)
int     Bat_val_last = 0;    // cache of val2 of the last frame
int     Bat_volt      = 0;   // the motor voltage

// -- Feeder monitoring variables
int     Feeder_val       = 0;    // sensor value in 1/10 deg from SE_BAT
int     Feeder_rpm       = 0;    // RPM value in 10xRPM
int     Feeder_t_last    = 0;    // cache of the time at last monitor frame (used to calculate rpm)
int     Feeder_t0        = 0;    //
int     Feeder_val_last  = 0;    // cache of val1 of the last frame (used to calculate rpm)
int     Feeder_volt      = 0;    // the motor voltage

// -- Intaker monitoring variables -- disabled -- no need to monitor intaker
int     Intaker_val       = 0;    // sensor value in 1/10 deg from SE_BAT
int     Intaker_volt      = 0;    // the motor voltage

// -- Wheel and gyro monitoring variables
int     Wheel_val         = 0;    // in 0.01 inch
int     Wheel_val_L       = 0;    // in 0.01 inch
int     Wheel_val_R       = 0;    // in 0.01 inch
int     Wheel_speed       = 0;    // in 0.1 inch per second
int     Wheel_speed_L     = 0;    // in 0.1 inch per second
int     Wheel_speed_R     = 0;    // in 0.1 inch per second
int     Wheel_t_last      = 0;    // cache of the time at last monitor frame (used to calculate rpm)
int     Wheel_t0          = 0;
int     Wheel_val_last    = 0;    // cache of val1 of the last frame (used to calculate rpm)
int     Wheel_val_L_last  = 0;    // cache of val1 of the last frame (used to calculate rpm)
int     Wheel_val_R_last  = 0;    // cache of val1 of the last frame (used to calculate rpm)
int     Wheel_volt        = 0;    // the left  wheel motors voltage
int     Gyro_val          = 0;     // in 1/10 degree
int     Gyro_val_last     = 0;     // in 1/10 degree
int     Gyro_speed        = 0;     // deg/second
int     US0_val           = 0;     // in mm
int     US1_val           = 0;     // in mm
#define US0_DMIN           25      // dmin for us0 detecting ball   ***
#define US0_DMAX           65      // dmax for us0 detecting ball   ***
#define US1_DMIN           50      // dmin for us1 detecting ball   ***
#define US1_DMAX           80      // dmax for us1 detecting ball   ***


///////////////////////////////////////////////////////////////////////////////
// SMART FEEDER
///////////////////////////////////////////////////////////////////////////////
#define FEEDER_TMAX_UP       320   // time out threshold in pull
#define FEEDER_TMAX_DOWN     300   // time out threshold in pull
#define FEEDER_V_MAX         120   // max voltage to use
#define FEEDER_V_MIN         20    // in PID if v < vmin, stop, exit
#define FEEDER_UP_BRAKE      120   //
#define FEEDER_UP_LIMIT      180   //
#define FEEDER_DOWN_BRAKE    60   //
#define FEEDER_DOWN_LIMIT      0   //
#define FEEDER_KP_UP         1.0  //pid kp parameters
#define FEEDER_KV_UP         0.4  //pid kv parameters
#define FEEDER_KP_DOWN       1.0  //pid kp parameters
#define FEEDER_KV_DOWN       0.33  //pid kv parameters
#define FEEDER_RESET_V       50
#define FEEDER_RESET_KP      1

///////////////////////////////////////////////////////////////////////////////
// SMART MOVEMENT
///////////////////////////////////////////////////////////////////////////////
#define MOVE_HOLD_V        10   // the voltage that will hold the wheel in place (but not moving)
#define MOVE_X_DX1        1000  // start to brake dist  0.01 "
#define MOVE_X_DX2        800   // end of brake distance 0.1"
#define MOVE_X_DX3        50   // end of brake distance 0.1"
#define MOVE_X_VOLT1       127 // end of brake distance 0.1"
#define MOVE_X_VOLT2       0   // linearly reduce the speed till let it go distance
#define MOVE_X_VOLT_MAX    40
#define MOVE_X_VOLT_BRAKE  -40
#define MOVE_X_V_SLOW  100
#define MOVE_X_KV  2
#define ROTATE_DA1        200 // start to brake angle
#define ROTATE_DA2        20  // end brake angle
#define ROTATE_BRAKE_V1   180 //
#define ROTATE_BRAKE_V2   40  //
#define ROTATE_BRAKE_DV   10  //  range allowed +/- 1"/second
#define ROTATE_BRAKE_VOLT -20  //  default brake voltage
#define ROTATE_BRAKE_VMAX 60  //  default brake voltage
#define ROTATE_BRAKE_DT 40  //  default brake voltage
#define ROTATE_BRAKE_KV   0.75   //  pid control parameters
// slow move and slow rotation for short distance
// #define MOVE_X_SLOW_VOLT 40        //
#define MOVE_X_SLOW_VOLT 60        //
#define ROTATE_SLOW_VOLT 40        //
#define ROTATE_SLOW_BRAKE_VOLT -20  //
//
#define MOTION_DRV_THRESHOLD  10      // threshold of joystick sensitivity

///////////////////////////////////////////////////////////////////////////////
// TASKS and FUNCTIONS WRAPPER
///////////////////////////////////////////////////////////////////////////////
// -- task definition
task somnus_monitor();
task somnus_remote();
task somnus_bat();
task somnus_feeder();

// -- LCD functions
void waitForPress();
void waitForRelease();
void lcd_selection();          // select side and then call lcd_mode_selection// to modify
void lcd_mode_selection();     // select MODE // to modify
void lcd_bat_adj();             // adjust long shot voltage
void lcd_message();            // display message ON LCD
void lcd_msg_init();            // display message INIT

// -- utilites functions
int bound (int a, int amin, int amax);     // bound the value to [amin amax]
int sign (int x);                          // sign function
int is_timed_out(int tstop);               // time out at tstop = time1[T1]
int is_bailed_out();                       // bailout

// -- sensor value functions
int  volt1() ;                            // main battery in mV
int  volt2() ;                            // second battery in mV (scale factor adjusted to 286)
//void getGyroVal() ;                       // Gyro_val in 0.1 deg
void getWheelVal() ;                      // Wheel_val in 0.01 inch
void getIntakerVal();                     //Intaker_val in 0.01 inch
void getFeederVal();                      //Feeder_val in degree
void getBatVal();                         //Bat_val in degree
void getUS0Val() ;                        //US0_val in mm
void getUS1Val() ;                        //US1_val in mm
int is_loaded();                          //Ball0 = 0/1 based on US0_val
int is_queued();                          //Ball1 = 0/1 based on US1_val
                                          //Bat_rpm in RPM
                                          //Feeder_rpm in RPM
                                          //Wheel_speed in 0.1 inch/second
                                          //Gyro_speed in deg/second

// -- basic control functions
void move_x(int vol)   ;
void rotate(int  vol)  ;
void bat(int vol)      ;
void intaker(int vol)  ;
void feeder(int vol)   ;
int  move_x_by_slow(int d, int tlimit);
int  move_x_by(int d, int tlimit);
int  move_x_by_nostop(int d, int tlimit);
int  rotate_by(int angle, int tlimit);
int  rotate_by_slow(int angle, int tlimit);

// -- bat functions
void bat_short();
void bat_medium();
int  bat_skills();
int  bat_long();
void bat_pull(int v1, int v2, int ang1, int ang2 );
void bat_pull_long();
void bat_pullback(int v0, int dt, int dt2);
void bat_hit(int v, int dt);
void bat_hold(int v, int dt);
void bat_reset();
void volt_adj();

// -- feeder functions
void feeder_reset();        // auto reset feeder 0 position
void feed_1ball();          // motion (pid controlled) to feed 1 ball
int  is_bat_to_feed();       // detect if bat position is ok to feed

// -- control drive motions
void shooting_dist_ctrl();
void drv_motion_ctrl();
void rotate_by_step(int dir);
void slow_bat_ctrl();

// -- control auto and complex motions
void auto_A();
void auto_B();
void auto_C();
void prg_skills();
void magic_1();
void magic_2();
void fire_1_shot(int CMD, int TWAIT, int TPAUSE);
void skills_fire_1();
void skills_fire_n(int nShots);
// speaker functions -- ??

//

#endif //SOMNUS_H
