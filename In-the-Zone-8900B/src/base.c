/*
  This file controls the base.
*/
#include "main.h"

int min(int a, int b) {
    return (a<b)?a:b;
}

int max(int a, int b) {
    return (a>b)?a:b;
}

void driver_control() {
  while (true) {
    int V = joystickGetAnalog(1, 2);
    if (V > -10 && V < 10) { // threshold
        V = 0;
    }
    int H = joystickGetAnalog(1, 1);
    if (H > -10 && H < 10) { // threshold
        H = 0;
    }
    /*
    motorSet(wheel_R2, min(127, max(-127, 1*(V-H))));
    motorSet(wheel_L2, min(127, max(-127,  V+H)));
    motorSet(wheel_L1, min(127, max(-127, 1*(V+H))));
    motorSet(wheel_R1, min(127, max(-127, V-H)));
    */
    motorSet(wheel_L2, min(127, max(-127, 1*(H+V))));
    motorSet(wheel_R2, min(127, max(-127,  H-V)));
    motorSet(wheel_R1, min(127, max(-127, 1*(H-V))));
    motorSet(wheel_L1, min(127, max(-127, H+V)));
    delay(5);
  }
}
