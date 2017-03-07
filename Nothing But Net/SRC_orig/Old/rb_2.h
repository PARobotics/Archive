///////////////////////////////////////////////////////////////////////////////
// SMART BAT using RB-2
///////////////////////////////////////////////////////////////////////////////

// THE PARAMETERS MOST LIKELY NEED ADJUSTMENT
int BAT_LONG_HIT_V_BASE_2 = 46;       // ***** the mostly important one, adjusted in LCD
#define BAT_SHORT_HIT_V_BASE  50   // *** important for short - -but no adjustment in LCD
#define BAT_MEDIUM_HIT_V_BASE 60   // *** important for medium - -but no adjustment in LCD
#define BAT_HIT_LIMIT        -15   // most likely never need to change
#define BAT_LONG_HIT_V_BASE  50   // retired

// -- long shots - the following parameters are adjusted to optimize the curves using analyze code
//    adjusted during lab calibration of rubber band
#define BAT_LONG_PULL_V1     127     // was 127
#define BAT_LONG_PULL_V2     100
#define BAT_LONG_PULL_V3     20
#define BAT_LONG_PULL_ANG1   50      // was 45
#define BAT_LONG_PULL_ANG2   110     // was 90 (80)
#define BAT_LONG_PULL_ANG3   160     // was 160 (150)
#define BAT_LONG_PB_V        80
#define BAT_LONG_PB_DT       80
#define BAT_LONG_PB_DT_2     100

// -- short shots - the following parameters are adjusted to optimize the curves using analyze code
//    adjusted during lab calibration of rubber band
#define BAT_SHORT_PULL_V     70    // was 80  ****
#define BAT_SHORT_PULL_ANG   60    // was   ****
#define BAT_SHORT_PB_V       60
#define BAT_SHORT_PB_DT      40
#define BAT_SHORT_PB_DT_2    40

// -- medium shots- the following parameters are adjusted to optimize the curves using analyze code
//    adjusted during lab calibration of rubber band
#define BAT_MEDIUM_PULL_V    80      // ****
#define BAT_MEDIUM_PULL_ANG  100    // was 90 ****
#define BAT_MEDIUM_PB_V      60
#define BAT_MEDIUM_PB_DT     50
#define BAT_MEDIUM_PB_DT_2   50

// holding parameters
#define BAT_HOLD1_V    10
#define BAT_HOLD2_V    20
#define BAT_HOLD1_TMAX 2000
#define BAT_HOLD2_TMAX 2000
#define BAT_HOLD2_TWAIT 80         //** this one can be adjusted to optimize the curve in analyze

// pull/hit/pb
#define BAT_PULL_TMAX  500
#define BAT_HIT_TMAX   400
#define BAT_PB_TMAX    400
#define BAT_PB_KV      0.5
#define BAT_PB_DRPM    20

// reset
#define BAT_RESET_V    20          //** was 15  this value needs adjustment -
#define BAT_RESET_ANG  -15
#define BAT_RESET_TMAX   1000

// PHOENIX
#define BAT_PHOENIX_VOLT 30
#define BAT_PHOENIX_VMIN 0
#define BAT_PHOENIX_VMAX 60
#define BAT_PHOENIX_TMAX 2000
#define BAT_PHOENIX_ANG1 -350
#define BAT_PHOENIX_RPM1 -200
#define BAT_PHOENIX_KV1  0.1
#define BAT_PHOENIX_ANG2 -60
#define BAT_PHOENIX_RPM2  100
#define BAT_PHOENIX_KV2  0.1

// declaration
int  BAT_SHORT_HIT_V  ;
int  BAT_MEDIUM_HIT_V ;
int  BAT_LONG_HIT_V   ;              // retired
int  BAT_LONG_HIT_V_2   ;
int  BAT_NUMBER = 0;                 // it is a global variable that goes with BAT_CMD=CONTINUOUS to
                                    // control how many bats to hit in a row
int BAT_COUNT  = 0;                 // it is a global variable to track if number of hits are reached
int BAT_COUNT_TOTAL  = 0;           // track total number of bats (long, short, continuous etc)
int BAT_HOLD1_TLAST = 0;
