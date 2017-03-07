#ifndef SS_AUTON
#define SS_AUTON

/*
	AUTO A: Main autonomous mode
  STARTS on side opposite pole
	Stars + Cube (18 pts max)
*/

void auto_A(){
  //Shoot preload
  //PlaySoundFile("StrengthOfStreetKnowledge.wav");
  CLAW_COMMAND = OPEN;
  wait1Msec(1000);
  dumper(127);
  wait1Msec(500);
  dumper(-127);
  wait1Msec(500);
  dumper(STOP);
  CLAW_COMMAND = STOP;
}

/*
	AUTO B: Complement autonomous
  STARTS on pole side
	Stars + High Hang (19 pts max)
*/

void auto_B(){
    //Shoot preload
    dump_dump_for(100);
    dump_down_for(200);
}

/*
  AUTO_C
  Safe auton: shoots preload
*/

void auto_C(){
  //Simple + Safe Auton
  dump_dump_for(100);
  dump_down_for(200);
}

#endif //SS_AUTON
