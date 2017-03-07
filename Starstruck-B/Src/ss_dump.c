#ifndef SS_DUMP
#define SS_DUMP

#define DUMP_MAX             1795    //In 1/10 degree
#define DUMP_RELEASE         1200
#define DUMP_MIN             20     //In 1/10 degree
#define DUMP_HOLD_THRESHOLD  132     //In 1/10 degree
#define DUMP_DT              20

// DUMPER_DUMP
#define UP_ANG0             800    //At this point, start linear deacceleration
#define UP_V0               127     //Initial up voltage
#define UP_ANG_MAX          1775    //Stop the dumper at this angle
#define UP_RPM_MIN          50
#define UP_RPM_0            1000
#define UP_V_DEF            70
#define UP_V_MIN           -80
#define UP_V_MAX            120
#define UP_KV               0.1

//DUMPER_HOLD
#define HOLD_V0            -60
#define HOLD_ANG0          350
#define HOLD_ANG            0
#define HOLD_KX             0.1
#define HOLD_KV             0.1
#define HOLD_V_DEF          0
#define HOLD_V_MIN          -40
#define HOLD_V_MAX          80


//DUMPER_CARRY
#define CARRY_THRESHOLD    50
#define CARRY_ANG          400      //  this is where you want the carry angle to be
#define CARRY_ANG0         300      //  Measure speed to determine hold voltage
#define CARRY_V0           127      // initial voltage
#define CARRY_V_DEF        60      // default and range of voltage during deceleration
#define CARRY_V_MIN        -20
#define CARRY_V_MAX        80
#define CARRY_KX            0.1       // control parameter
#define CARRY_KV            0.1       // control parameter
#define CARRY_KDEF            0.1       // control parameter for adjusting v_def
#define CARRY_RPM_DEF       400       // control parameter for adjusting v_def

// DUMPER_DOWN
#define DOWN_V0           -127      // initial voltage (full speed down)
#define DOWN_ANG0         1500      // this is where we stop to use initial voltage (start ccontrol)
#define DOWN_ANG_MIN       20       // this is where stop DUMP_DOWN
#define DOWN_RPM_0         -800     // these  values define desired ang-rpm curve and bound
#define DOWN_RPM_MIN       -50
#define DOWN_DRPM          0
#define DOWN_V_DEF        -40      // default and range of voltage during deceleration
#define DOWN_V_MIN        -80
#define DOWN_V_MAX        40
#define DOWN_KV            0.1       // control parameter

void dumper(int vol){
	motor[M_DUMPER_ML]  =  vol;
	motor[M_DUMPER_OL]  =  vol;
	motor[M_DUMPER_MR]  =  vol;
	motor[M_DUMPER_OR]  =  vol;
}

void dump_down_for(int tlimit){
	int tnow = time1[T1];
	while(true){
		DUMP_COMMAND = DOWN;
		if(is_timed_out(tnow + tlimit) || DUMP_VAL <= DUMP_MIN){
			dumper(STOP);
			break;
		}
		wait1Msec(10);
	}
}

void dump_dump_for(int tlimit){
	int tnow = time1[T1];
	while(true){
		DUMP_COMMAND = UP;
		if(is_timed_out(tnow + tlimit) || DUMP_VAL >= DUMP_MAX){
			dumper(STOP);
			break;
		}
		wait1Msec(10);
	}
}

void one_dump(){
		dump_dump_for(3000);
		wait1Msec(200);
    dump_down_for(3000);
}

task ss_dump(){
	DUMP_COMMAND = STOP;
	while(true){
		getDumpVal();
		if(DUMP_VAL >= DUMP_RELEASE && CLAW_COMMAND != OPEN && CLAW_COMMAND != HOLD_OPEN) CLAW_COMMAND = OPEN;

    if(DUMP_COMMAND == CARRY){
      //Hold the dumper into an intaked position until another dumper function is called
      int v_def;

			CLAW_COMMAND = CLOSE;

      //Go full speed until reach an angle
      if(DUMP_VAL < CARRY_ANG0){
        DUMP_VCMD = CARRY_V0;
        dumper(DUMP_VCMD);

        //Constantly read value to determine hold voltage
        v_def = CARRY_V_DEF -CARRY_KDEF*((DUMP_OMEGA+DUMP_LAST_OMEGA)/2-CARRY_RPM_DEF);
      }

      // Switch to XV to hold in place
      else if(DUMP_VAL < CARRY_ANG ){
        DUMP_VCMD = vcmd_ctrl_xv(
         DUMP_VAL,DUMP_OMEGA,
         CARRY_ANG, 0, CARRY_KX, CARRY_KV,
         CARRY_V_DEF, CARRY_V_MIN, CARRY_V_MAX);
        dumper(0);
      }

      else if(DUMP_VAL > CARRY_ANG){
        DUMP_COMMAND = STOP;
      }
    }/*
		else if(DUMP_COMMAND == UP){
		  int vcmd, rpm_desired;

			if(DUMP_VAL < UP_ANG_MAX){
				DUMP_VCMD = UP_V0;
				dumper(DUMP_VCMD);
			}
		  else{
		    dumper(STOP);
		    DUMP_MONITOR = STOP;
		    DUMP_COMMAND = STOP;
		  }
		}
		else if(DUMP_COMMAND == DOWN){
			// constant voltage for a while (until hit DOWN_ANG0)
	    if(DUMP_VAL > DOWN_ANG0){
	      DUMP_VCMD = DOWN_V0;
	      dumper(DUMP_VCMD);
	    }

	    // deceleration control phase (between DOWN_ANG0 and DOWN_ANG_MIN) use linear_band control
	    else if(DUMP_VAL > DOWN_ANG_MIN ){
	      DUMP_VCMD = vcmd_ctrl_linear_band(
	        DUMP_VAL,DUMP_OMEGA,
	        DOWN_ANG0, DOWN_ANG_MIN, DOWN_RPM_0,DOWN_RPM_MIN, DOWN_DRPM, DOWN_KV,
	        DOWN_V_DEF,DOWN_V_MIN,DOWN_V_MAX);
	      dumper(DUMP_VCMD);
	    }
	    else{
	      dumper(STOP);
	      DUMP_MONITOR = STOP;
	      DUMP_COMMAND = STOP;
	    }
		}*/
		else if(DUMP_COMMAND == UP_MANUAL){
			dumper(127);
		}
		else if(DUMP_COMMAND == DOWN_MANUAL){
			dumper(-127);
		}
    else{
    	dumper(STOP);
    }
		wait1Msec(50);
  }
}

#endif //SS_DUMP
