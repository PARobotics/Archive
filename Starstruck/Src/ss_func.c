#ifndef SS_FUNC_C
#define SS_FUNC_C
//#include "ss.h"

/*
  FUNCTION.C
  For general functions
*/


//Sensor Functions
int volt1() { //Returns voltage of main battery in millivolts
  return nImmediateBatteryLevel;
}

int volt2() { //Returns voltage of power expander battery in millivolts
  return SensorValue(PWR)*1000/286;
}


int is_timed_out(int tstop)
// customized function to enforce time logics
{
  if (time1[T1] >= tstop)
  {
    return 1;
  }
  else {
    return 0;
  }
}

#endif //SS_FUNC_C
