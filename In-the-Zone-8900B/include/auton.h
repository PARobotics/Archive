#ifndef _AUTON_H
#define _AUTON_H

#include <time.h>

//#define REV_TO_MG_1 3.501409
#define REV_TO_MG_1 2.08493
#define REV_TO_MG_2 2.34754
#define REV_TO_TURN_AROUND 2.25000
#define REV_BACK_TO_ZONE 3.27063
#define REV_MG_REV_RELEASE 0.397887
#define REV_MOVE_AWAY 1.84620

typedef enum {FORWARD, BACKWARD, LEFT, RIGHT} direction;

void autonomous(void);

void auton_move(double revolutions, direction d);
void move_full_forward(void);
void move_full_backward(void);
void turn_full_right(void);
void turn_full_left(void);

void stop_all_motors(void);

void check_encoder(void);

#endif
