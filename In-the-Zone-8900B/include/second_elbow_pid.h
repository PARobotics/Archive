#include "main.h"
/* PID STUFF */
// FROM PURDUE PROS LIB
#define EXAMPLE_DB 15 // the motors don't do much below this speed
#define EXAMPLE_TOLERANCE 5 // Depends heavily on the system
#define EXAMPLE_CONFIDENCE 3 // Means that the controller needs to be in the tolerance band for ~200ms, usually enough

#define EXAMPLE_KP 10.0f
#define EXAMPLE_KI 0.04f
#define EXAMPLE_KD 130.0f
#define EXAMPLE_INTEG_CAP 100000


// Initialized elbow PID.
void secondElbowInit();

// Moves elbow
void moveSecondElbow(int power);

// Holds position after moved
void holdElbowPos();

void stopHoldElbowPos();
