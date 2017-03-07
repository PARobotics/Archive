#ifndef SS_DUMP
#define SS_DUMP

#define DUMP_MIN             -10     //In 1/10 degree
#define DUMP_MIN2            200     //this is when to left robot move at the angle
// #define DUMP_DT              20

// DUMPER_DUMP
#define DUMP_MAX             1220    //In 1/10 degree
#define DUMP_RELEASE        1000    //Value to release dumper at
// #define UP_V0               127     //Initial up voltage
// #define UP_RPM1             240      //if RMP > 24 rpm, reduce power
// #define UP_V1               50      //slow it down if too fast
// #define UP_RPM2             320      //if RMP > 32 rpm, cut off power
// #define UP_V2               0       //slow it down further if too fast
// #define DUMP_MAX            1200    //In 1/10 degree
// #define DUMP_RELEASE        950    //Value to release dumper at
#define UP_V0               127     //Initial up voltage
#define UP_RPM1             280      //if RMP > 24 rpm, reduce power
#define UP_V1               127      //slow it down if too fast
#define UP_RPM2             360      //if RMP > 32 rpm, cut off power
#define UP_V2               127       //slow it down further if too fast

// DUMPER_DOWN
#define DOWN_V0           -127      // initial voltage (full speed down)

//DUMPER_HOLD   -- soft hold good to grab stars, use a high volt angle above hold angle; but
// a low voltage otherwise.
// to grab cube we could simple set DUMP_COMMAND = DOWN to fully hold on groound
#define HOLD_V0            -127
#define HOLD_V1            -45
#define HOLD_ANG0          150

//DUMPER_CARRY
#define CARRY_THRESHOLD    50
// from the video, carry angle too small, star rub the floor
#define CARRY_ANG          600      //  this is where you want the carry angle to be
#define CARRY_ANG0         400      //  Measure speed to determine hold voltage
#define CARRY_V0           40      // initial voltage
#define CARRY_V_DEF        20      // default and range of voltage during deceleration
#define CARRY_V_MIN        -20
#define CARRY_V_MAX        40
#define CARRY_KX            0.1       // control parameter
#define CARRY_KV            0.1       // control parameter
#define CARRY_KDEF            0.1       // control parameter for adjusting v_def
#define CARRY_RPM_DEF       400       // control parameter for adjusting v_def

// carry high full speed up to 30 deg, then try to balance
#define CARRY_HIGH_ANG          700      //  this is where you want the carry angle to be
#define CARRY_HIGH_ANG0         300      //  Measure speed to determine hold voltage
#define CARRY_HIGH_V0           127      // initial voltage
#define CARRY_HIGH_V_DEF        20      // default and range of voltage during deceleration
#define CARRY_HIGH_V_MIN        -20
#define CARRY_HIGH_V_MAX        60
#define CARRY_HIGH_KX            0.1       // control parameter
#define CARRY_HIGH_KV            0.1       // control parameter
#define CARRY_HIGH_KDEF            0.1       // control parameter for adjusting v_def
#define CARRY_HIGH_RPM_DEF       400       // control parameter for adjusting v_def


void dumper(int vol) {
  motor[M_DUMP_L1]  =  vol;
  motor[M_DUMP_L2]  =  vol;
  motor[M_DUMP_L3]  =  vol;
  motor[M_DUMP_R1]  =  vol;
  motor[M_DUMP_R2]  =  vol;
  motor[M_DUMP_R3]  =  vol;
  motorReq[M_DUMP_L1]  =  vol;
  motorReq[M_DUMP_L2]  =  vol;
  motorReq[M_DUMP_L3]  =  vol;
  motorReq[M_DUMP_R1]  =  vol;
  motorReq[M_DUMP_R2]  =  vol;
  motorReq[M_DUMP_R3]  =  vol;
}

void claw(int status){
  if(status == TOGGLE){
    if(SensorValue(P_CLAW_L) == 1) status = 0;
    else status = 1;
  }
  SensorValue(P_CLAW_L) = status;
  SensorValue(P_CLAW_R) = status;
}

//void dump_down_for(int tlimit) {
//  int tnow = time1[T1];
//  while (true) {
//    DUMP_COMMAND = DOWN;
//    if (is_timed_out(tnow + tlimit) || DUMP_VAL <= DUMP_MIN) {
//      DUMP_COMMAND = STOP;
//      break;
//    }
//    wait1Msec(10);
//  }
//}

//void dump_dump_for(int tlimit) {
//  int tnow = time1[T1];
//  while (true) {
//    DUMP_COMMAND = UP;
//    if (is_timed_out(tnow + tlimit) || DUMP_VAL >= DUMP_MAX) {
//      DUMP_COMMAND = STOP;
//      break;
//    }
//    wait1Msec(10);
//  }
//}

/*
  DUMP TASK
  Constantly monitor dumper value and perform the following functions:
  1) Release the claw automatically at certain point
  2) Hold the dumper on DUMP_CARRY
  3) PID Control on up and down
*/

task ss_dump() {
  int v_def;
  while (true) {
    if(BAILOUT == 1) DUMP_COMMAND = STOP;
  	if(DUMP_VAL > DUMP_RELEASE) claw(OPEN); //Release claw at a certain value
    if(DUMP_COMMAND == UP){ //PID up and control the automatic release of the claw
      // use UP_V0/1/2, UP_RPM1/2 to reduce to power supply to dump UP
    	getDumpVal();
      if(DUMP_VAL < DUMP_MAX) {
        DUMP_VCMD = UP_V0;
        // if  (DUMP_OMEGA>UP_RPM2) {
          // DUMP_VCMD = UP_V2;
        // }
        // else if (DUMP_OMEGA>UP_RPM1) {
          // DUMP_VCMD = UP_V1;
        // }
        // else {
        	// DUMP_VCMD = UP_V0;
        // }
        dumper(DUMP_VCMD);
      }
      else {
        dumper(STOP);
        DUMP_COMMAND = STOP;
      }
      wait1Msec(5);
    }
    else if(DUMP_COMMAND == WIGGLE){ 
      dumper(70);
      wait1Msec(500);
      dumper(-70);
      wait1Msec(500);
      DUMP_COMMAND = STOP;
    }
    else if(DUMP_COMMAND == DOWN){ //PID Slowly down
      DUMP_VCMD = DOWN_V0;
      dumper(DUMP_VCMD);
      wait1Msec(5);
    }
    else if(DUMP_COMMAND == MANUAL_UP){
      dumper(127);
      wait1Msec(5);
    }
    else if(DUMP_COMMAND == MANUAL_DOWN){
      dumper(-127);
      wait1Msec(5);
    }
    else if (DUMP_COMMAND == CARRY_HIGH) {
      //Hold the dumper at a certain height
      getDumpVal();
      if (DUMP_COMMAND != CARRY_HIGH) DUMP_MONITOR = START;

      //Stage 1: Full speed
      if (DUMP_VAL < CARRY_HIGH_ANG0) {
        DUMP_VCMD = CARRY_HIGH_V0;
        dumper(DUMP_VCMD);

        //Constantly read value to determine hold voltage
        v_def = CARRY_HIGH_V_DEF - CARRY_HIGH_KDEF * ((DUMP_OMEGA + DUMP_LAST_OMEGA) / 2 - CARRY_HIGH_RPM_DEF);
      }
      //Stage 2: Apply a hold voltage based on load
      else if (DUMP_VAL > DUMP_MIN ) {
        DUMP_VCMD = vcmd_ctrl_xv(
                      DUMP_VAL, DUMP_OMEGA,
                      CARRY_HIGH_ANG, 0, CARRY_HIGH_KX, CARRY_HIGH_KV,
                      CARRY_HIGH_V_DEF, CARRY_HIGH_V_MIN, CARRY_HIGH_V_MAX);
        dumper(DUMP_VCMD);
      }
      else if (DUMP_VAL > CARRY_HIGH_ANG) {
        DUMP_COMMAND = STOP;
      }
      wait1Msec(5);
    }
    else if (DUMP_COMMAND == CARRY) {
      //Hold the dumper at a certain height
      getDumpVal();
      if (DUMP_COMMAND != CARRY) DUMP_MONITOR = START;

      //Stage 1: Full speed
      if (DUMP_VAL < CARRY_ANG0) {
        DUMP_VCMD = CARRY_V0;
        dumper(DUMP_VCMD);

        //Constantly read value to determine hold voltage
        v_def = CARRY_V_DEF - CARRY_KDEF * ((DUMP_OMEGA + DUMP_LAST_OMEGA) / 2 - CARRY_RPM_DEF);
      }
      //Stage 2: Apply a hold voltage based on load
      else if (DUMP_VAL > DUMP_MIN ) {
        DUMP_VCMD = vcmd_ctrl_xv(
                      DUMP_VAL, DUMP_OMEGA,
                      CARRY_ANG, 0, CARRY_KX, CARRY_KV,
                      CARRY_V_DEF, CARRY_V_MIN, CARRY_V_MAX);
        dumper(DUMP_VCMD);
      }
      else if (DUMP_VAL > CARRY_ANG) {
        DUMP_COMMAND = STOP;
      }
      wait1Msec(5);
    }
    else if(DUMP_COMMAND == HOLD){
      //Hold the dumper onto the ground
      getDumpVal();
      if(DUMP_VAL > HOLD_ANG0){
      	DUMP_VCMD = HOLD_V0;
      }
      else if(DUMP_VAL > DUMP_MIN){
      	DUMP_VCMD = HOLD_V1;
      }
      else {
      	DUMP_VCMD = 0;
      }
      dumper(DUMP_VCMD);
      wait1Msec(5);
    }
    else if(DUMP_COMMAND == STOP && LIFT_COMMAND == STOP){
      dumper(STOP);
      wait1Msec(5);
    }
    else {
      wait1Msec(5);
    }
  }
}

#endif //SS_DUMP
