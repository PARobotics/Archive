// IMPORTANT BASE SHOOT PARAMETERS
int BAT_LONG_PULL_ANG_MAX_BASE = 210;  // between 180-250
#define BAT_LONG_PULL_ANG1 50          // between 40-70
#define BAT_LONG_PULL_ANG2 100         // *** between 90-120
#define BAT_LONG_PULL_DANG3 50         // ***  between 30-70      //ang3 = max-dang3
#define BAT_LONG_PULL_RPM1 150         // ***
#define BAT_LONG_PULL_RPM2 150         // ***
#define BAT_LONG_PULL_RPM3 100         // ***
#define BAT_LONG_PULL_VCMD1 60         // *** 
#define BAT_LONG_PULL_VCMD2 40
#define BAT_LONG_PULL_VCMD2_MIN 20
#define BAT_LONG_PULL_VCMD2_MAX 60
#define BAT_LONG_PULL_VCMD3 40
#define BAT_LONG_PULL_VCMD3_MIN 20
#define BAT_LONG_PULL_VCMD3_MAX 60
#define BAT_LONG_PULL_VCMD4 20 
#define BAT_LONG_PULL_VCMD4_MIN 10
#define BAT_LONG_PULL_VCMD4_MAX 30

#define BAT_LONG_PULL_REV_DT  40         // *** used for estimte dtMore
#define BAT_LONG_PULL_REV_HOLDTMAX  60   // used for  dtMore

#define BAT_HIT_LIMIT        -10        // *** adjusted to give ball a full kick but not having too much juice left afterwards

int  BAT_LONG_HIT_V = 100;              // vcmd value during hit back

// -- important SHORT parameters
#define BAT_SHORT_HIT_V_BASE  70   // ***
#define BAT_SHORT_PULL_V      45   // ***
#define BAT_SHORT_PULL_ANG1   10   // ***
#define BAT_SHORT_PULL_ANG2   30   // ***

// -- important MEDIUM parameters
#define BAT_MEDIUM_HIT_V_BASE  30  // ***
#define BAT_MEDIUM_PULL_V      60  // ***
#define BAT_MEDIUM_PULL_ANG1   40  // ***
#define BAT_MEDIUM_PULL_ANG2   90  // ***

// IMPORTANT SKILLS PARAMETERS
// - magic_1()
#define MAGIC1_X1   4600
#define MAGIC1_R1   850
#define MAGIC1_X2   -6400

#define AUTOB_RED_R1   200
#define AUTOB_RED_X1   4800
#define AUTOB_RED_R2   6000
#define AUTOB_RED_X2   -400
#define AUTOB_RED_R3   200

#define AUTOB_BLUE_R1  350
#define AUTOB_BLUE_X1  4800
#define AUTOB_BLUE_R2  -200
#define AUTOB_BLUE_X2  6000
#define AUTOB_BLUE_R3  150


//*** KEY SKILLS PARAMETERS
#define AUTO_ADJ_VMIN  375      // *** 90-deg avg speed (from 100 to 10 degree)
#define AUTO_ADJ_VMAX  385      // ***
#define SKILLS_BAT_INC0  4      // increase ANG_MAX of second shot from the 1st
#define SKILLS_BAT_INC1  1     // the increment when bat speed falls MIN MAX range in AUTO_ADJUST
#define SKILLS_BAT_INC2 -5     // lower ANG_MAX of the first shot after reposition

// -- adjust the following to speed up/slow the programming skills
#define D_TIME_PULL   120       // *** threshold to determine when battery needs rest
#define T_BATT_REST   1000      // the rest time for battery if needed    
#define T_INBW_REST   200        // rest time in between shots



/////////////////////////////////////////////////////////////////////////////////
// likely no changes below this line
// KEY SKILLS PARAMETER
#define USE_AUTO_ADJUST OFF      // set to 1/0 to turn on/off AUTO_ADJUST in game and robotskills

// OTHER PARAMETERS -- most likely don't adjust
#define BAT_LONG_PULL_VCMD2_K 1.0
#define BAT_LONG_PULL_VCMD3_K 1.0
#define BAT_LONG_PULL_VCMD4_K 0.5
#define BAT_LONG_HIT_KP      0       // vcmd adjustment coeff (for every 10 deg in error)
#define BAT_PULL_TMAX        600
#define BAT_HIT_TMAX         400

// -- for SHORT/MEDIUM hold the fire off by 2000 msec if ball is not loaded and reset to STOP
int BAT_WAIT_TIME    = 0;
#define BAT_WAIT_TIME_MAX     2000

// -- pull back parameters -- not likely need to change after finetured
#define BAT_PB_TMAX         400
#define BAT_PB_KV           0.5
#define BAT_PB_DRPM          20
#define BAT_LONG_PB_V        80 // control PB after long
#define BAT_LONG_PB_DT       80
#define BAT_LONG_PB_DT_2    160   // was 100
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


#define MAGIC2_R1   -450
#define MAGIC2_X1   6000
#define MAGIC2_R2   1000
#define MAGIC2_X2   -360

int BAT_CONT_HOLD_IN_BTW       = 100;    // not used

// declaration
int  BAT_SHORT_HIT_V       = BAT_SHORT_HIT_V_BASE;
int  BAT_MEDIUM_HIT_V      = BAT_MEDIUM_HIT_V_BASE;
int  BAT_LONG_PULL_ANG_MAX = BAT_LONG_PULL_ANG_MAX_BASE;
int  BAT_NUMBER            = 0;     // in BAT_CMD=CONTINUOUS; BAT_NUMBER is the desired number of hits
int  BAT_COUNT             = 0;     // BAT_COUNT is the number of hits made in current continous cmd
int  BAT_COUNT_TOTAL       = 0;     // track total number of bats (long, short, continuous etc)
int  BAT_HOLD1_TLAST       = 0;
