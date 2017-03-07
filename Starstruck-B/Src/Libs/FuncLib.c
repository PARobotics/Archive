#ifndef FUNCLIB_C
#define FUNCLIB_C
/*-----------------------------------------------------------------------------*/
/* define commonly used functions -- should be included early in the file tree
//   MIN
//   MAX
//   SIGN
//   BOUND
/*-----------------------------------------------------------------------------*/

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

//Utility functions

int SIGN (int x) {
  if (x > 0) return 1;
  if (x < 0) return -1;
  return 0;
}

int BOUND (int a, int amin, int amax) { // Keeps value between amin and amax
  if (a <= amin) {a = amin;}
  if (a >= amax) {a = amax;}
  return a;
}

#endif //FUNCLIB_C
