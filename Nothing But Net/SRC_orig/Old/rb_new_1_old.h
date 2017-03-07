///////////////////////////////////////////////////////////////////////////////
// SMART BAT using RB-(new)-1
///////////////////////////////////////////////////////////////////////////////

// THE PARAMETERS MOST LIKELY NEED ADJUSTMENT
// -- base value for long hits that can be adjusted in LCD
int BAT_LONG_PULL_ANG_MAX_BASE = 210;
// int BAT_LONG_PULL_ANG_MAX_BASE = 240;
int BAT_LONG_PULL_ANG_DELTA    = 0;
// int BAT_CONT_HOLD_IN_BTW       = 40;    // the hold time using hold_V before starting the next hit
// int BAT_CONT_HOLD_IN_BTW       = 1000;    // the hold time using hold_V before starting the next hit
// int BAT_CONT_HOLD_IN_BTW       = 250;    // the hold time using hold_V before starting the next hit
// int BAT_CONT_HOLD_IN_BTW       = 1000;    // the hold time using hold_V before starting the next hit
int BAT_CONT_HOLD_IN_BTW       = 400;    // the hold time using hold_V before starting the next hit
// int BAT_CONT_HOLD_IN_BTW       = 250;    // the hold time using hold_V before starting the next hit
// int BAT_CONT_HOLD_IN_BTW       = 150;    // the hold time using hold_V before starting the next hit
// int BAT_CONT_HOLD_IN_BTW       = 200;    // the hold time using hold_V before starting the next hit
// int BAT_CONT_HOLD_IN_BTW       = 100;    // the hold time using hold_V before starting the next hit

// -- important LONG parameters that are RB dependent
// #define BAT_LONG_PULL_V1     120
// #define BAT_LONG_PULL_V1     70
int BAT_LONG_PULL_V1 = 70;
// #define BAT_LONG_PULL_V1     75
// #define BAT_LONG_PULL_V2      30    // *** make sure it moves bat ... was 20
// #define BAT_LONG_PULL_V2      20    // *** make sure it moves bat ... was 20
// #define BAT_LONG_PULL_V2      30    // *** make sure it moves bat ... was 20
int BAT_LONG_PULL_V2 = 30;    // *** make sure it moves bat ... was 20
#define BAT_LONG_PULL_ANG1    60    // *** make sure over max Torque was 75
// #define BAT_LONG_PULL_ANG1    55    // *** make sure over max Torque was 75
// #define BAT_LONG_PULL_ANG1    60    // *** make sure over max Torque was 75
// int BAT_LONG_PULL_ANG2    =  100;    // ***
int BAT_LONG_PULL_ANG2    =  90;    // ***
// int BAT_LONG_PULL_ANG2    =  80;    // ***
// #define BAT_LONG_PULL_DT     50     // *** hold 50 msec to settle before check ang and rpm
#define BAT_LONG_PULL_DT     30     // *** hold 50 msec to settle before check ang and rpm
// #define BAT_LONG_PULL_REV_DT  90
// #define BAT_LONG_PULL_REV_DT  70
// #define BAT_LONG_PULL_REV_DT  40
// #define BAT_LONG_PULL_REV_DT  50
#define BAT_LONG_PULL_REV_DT  20
#define BAT_HIT_LIMIT        -10    // *** make sure no RPM slow down before hit and not too much carry thru
// #define BAT_LONG_HIT_V       120
// #define BAT_LONG_HIT_V       100
int  BAT_LONG_HIT_V = 100;
#define BAT_LONG_HIT_KP      5       // vcmd adjustment coeff (for every 10 deg in error)
#define BAT_PULL_TMAX        600
#define BAT_HIT_TMAX         400

// -- important SHORT parameters
// #define BAT_SHORT_HIT_V_BASE  44   // ***
// #define BAT_SHORT_PULL_V      80   // ***
#define BAT_SHORT_HIT_V_BASE  70   // ***
#define BAT_SHORT_PULL_V      45   // ***
#define BAT_SHORT_PULL_ANG1   10   // ***
#define BAT_SHORT_PULL_ANG2   30   // ***

// -- important MEDIUM parameters
#define BAT_MEDIUM_HIT_V_BASE  30  // ***
#define BAT_MEDIUM_PULL_V      60  // ***
#define BAT_MEDIUM_PULL_ANG1   40  // ***
#define BAT_MEDIUM_PULL_ANG2   90  // ***

// -- for SHORT/MEDIUM hold the fire off by 2000 msec if ball is not loaded and reset to STOP
int BAT_WAIT_TIME    = 0;
#define BAT_WAIT_TIME_MAX     2000


// -- pull back parameters -- not likely need to change after finetured
#define BAT_PB_TMAX         400
#define BAT_PB_KV           0.5
#define BAT_PB_DRPM          20
#define BAT_LONG_PB_V        80 // control PB after long
#define BAT_LONG_PB_DT       80
#define BAT_LONG_PB_DT_2    100
#define BAT_SHORT_PB_V       60   // control PB after short shots
#define BAT_SHORT_PB_DT      40
#define BAT_SHORT_PB_DT_2    40
#define BAT_MEDIUM_PB_V      60  // control PB after medium shots
#define BAT_MEDIUM_PB_DT     50
#define BAT_MEDIUM_PB_DT_2   50


// -- holding parameters -- not likely need to change
#define BAT_HOLD1_V          10
#define BAT_HOLD2_V          10
#define BAT_HOLD1_TMAX       2000
#define BAT_HOLD2_TMAX       2000
#define BAT_HOLD2_TWAIT      80

// -- reset  need to test and afterwards not likely needs change
#define BAT_RESET_V         20
#define BAT_RESET_ANG       -15
#define BAT_RESET_TMAX      1000

// phoenix
#define BAT_PHOENIX_VOLT    30
#define BAT_PHOENIX_VMIN    0
#define BAT_PHOENIX_VMAX    60
#define BAT_PHOENIX_TMAX    2000
#define BAT_PHOENIX_ANG1    -350
#define BAT_PHOENIX_RPM1    -200
#define BAT_PHOENIX_KV1     0.1
#define BAT_PHOENIX_ANG2    -60
#define BAT_PHOENIX_RPM2    100
#define BAT_PHOENIX_KV2     0.1

// declaration
int  BAT_SHORT_HIT_V       = BAT_SHORT_HIT_V_BASE;
int  BAT_MEDIUM_HIT_V      = BAT_MEDIUM_HIT_V_BASE;
int  BAT_LONG_PULL_ANG_MAX = BAT_LONG_PULL_ANG_MAX_BASE;
int  BAT_NUMBER            = 0;     // in BAT_CMD=CONTINUOUS; BAT_NUMBER is the desired number of hits
int  BAT_COUNT             = 0;     // BAT_COUNT is the number of hits made in current continous cmd
int  BAT_COUNT_TOTAL       = 0;     // track total number of bats (long, short, continuous etc)
int  BAT_HOLD1_TLAST       = 0;
