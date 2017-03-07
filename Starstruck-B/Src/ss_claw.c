#ifndef SS_CLAW
#define SS_CLAW

void claw(int vol){
	motor[M_CLAW_L] = vol;
	motor[M_CLAW_R] = vol;
}

void claw_left(int vol){
	motor[M_CLAW_L] = vol;
}

void claw_right(int vol){
	motor[M_CLAW_R] = vol;
}

task ss_claw(){
	CLAW_COMMAND = STOP;
	int tnow;
	int CLAW_VAL_LAST, CLAW_VAL_CUR, CLAW_OMEGA;
	while(true){
		//CLAW_VAL_LAST = (CLAW_VAL_L + CLAW_VAL_R) / 2;
		getClawVal();
		//CLAW_VAL_CUR = (CLAW_VAL_L + CLAW_VAL_R) / 2;
		//CLAW_OMEGA = abs((CLAW_VAL_CUR - CLAW_VAL_LAST) * 10;

		//writeDebugStreamLine("%4d", CLAW_OMEGA);

		if(CLAW_COMMAND == OPEN){
			if(CLAW_VAL_L <= 0) claw_left(STOP);
			else if(CLAW_VAL_L <= 500) claw_left(-10);
			else claw_left(-127);

			if(CLAW_VAL_R >= 2020) claw_right(STOP);
			else if(CLAW_VAL_R >= 1600) claw_right(-10);
			else claw_right(-127);
		}
		else if(CLAW_COMMAND == CLOSE){
			if(CLAW_VAL_L >= 1000) claw_left(0);
			else if(CLAW_VAL_L >= 500) claw_left(20);
			else claw_left(50);

			if(CLAW_VAL_R <= 1190) claw_right(0);
			else if(CLAW_VAL_R <= 1600) claw_right(20);
			else claw_right(50);
		}
		else if(CLAW_COMMAND == HOLD_OPEN){
			claw(-10);
		}
		else if(CLAW_COMMAND == HOLD_CLOSE){
			claw(10);
		}
		else{
			claw(STOP);
		}

		wait1Msec(100);
	}
}

#endif //SS_CLAW
