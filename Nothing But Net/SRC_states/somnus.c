#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, I2C_1,  IE_FEEDER,   sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  IE_WHEEL_L,  sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_3,  IE_WHEEL_R,   sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port1,  M_INTAKER_L, tmotorVex393_HBridge, openLoop,  reversed)
#pragma config(Motor,  port10, M_INTAKER_R, tmotorVex393_HBridge, openLoop, reversed                     )
#pragma config(Motor,  port2,  M_WHEEL_L,   tmotorVex393_MC29,   reversed, openLoop,            encoderPort, I2C_2)
#pragma config(Motor,  port3,  M_WHEEL_R,   tmotorVex393_MC29,     openLoop, encoderPort, I2C_3)
#pragma config(Motor,  port4,  M_BAT_1,     tmotorVex393_MC29,    openLoop)
#pragma config(Motor,  port5,  M_BAT_2,     tmotorVex393_MC29,    openLoop, reversed)
#pragma config(Motor,  port6,  M_BAT_3,     tmotorVex393_MC29,    openLoop, reversed)
#pragma config(Motor,  port7,  M_BAT_4,     tmotorVex393_MC29,    openLoop)
#pragma config(Motor,  port8,  M_BAT_5,     tmotorVex393_MC29,    openLoop, reversed)
#pragma config(Motor,  port9,  M_FEEDER,    tmotorVex393_MC29,    openLoop, reversed,  encoderPort, I2C_1)
#pragma config(Sensor, dgtl1,  SE_BAT,      sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  US_BALL_0,   sensorSONAR_mm)
#pragma config(Sensor, dgtl3,  US_BALL_1,   sensorSONAR_mm)
#pragma config(Sensor, in1,    Power2,      sensorPotentiometer)
#pragma config(Sensor, in2,    G_SENSOR,    sensorGyro)
#pragma config(Sensor, dgtl11,  LED1,           sensorDigitalOut)
#pragma config(Sensor, dgtl12,  LED2,           sensorDigitalOut)

// Make sure these ^ values are consistent with the Engineering Book

//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
    This is the main driver routine for 2015-2016 VEX NBN
    somnus.c
*/

#pragma platform(VEX)
#pragma competitionControl(Competition)
//#pragma competitionControl(OFF)  //Comment this out in competition
#pragma autonomousDuration(20)
#pragma userControlDuration(1200)

#include "Vex_Competition_Includes.c"

#include "somnus.h"
#include "state_rb_1.h"             // Rubber band specific parameters
#include "somnus_lcd.c"
#include "somnus_func.c"
#include "somnus_remote.c"
#include "somnus_bat.c"
#include "somnus_feeder.c"
#include "somnus_monitor.c"
#include "somnus_auton.c"

/*
    PRE AUTON
    functions
*/

void sensor_reset()
{
  SensorType[G_SENSOR] = sensorNone;
	nMotorEncoder[M_FEEDER] = 0;
	nMotorEncoder[M_WHEEL_L] = 0;
	nMotorEncoder[M_WHEEL_R] = 0;
	nMotorEncoder[M_INTAKER_L] = 0;
  SensorValue(SE_BAT) = 0;
	SensorType[G_SENSOR] = sensorGyro;
	wait1Msec(1000);
}

void global_variable_reset()
{
    BAT_SHORT_HIT_V = BAT_SHORT_HIT_V_BASE;
    BAT_MEDIUM_HIT_V = BAT_MEDIUM_HIT_V_BASE;
    BAT_LONG_PULL_ANG_MAX = BAT_LONG_PULL_ANG_MAX_BASE;
    DIST = LONG;
    BAT_CMD = STOP;
    FEEDER_CMD = STOP;
    BAT_MONITOR = STOP;
    FEEDER_MONITOR = STOP;
    INTAKER_MONITOR = STOP;
    FEEDER_ON = ON;
}

void pre_auton()
{
    writeDebugStreamLine("%%Initializing ... ");
    clearTimer(T1);
    writeDebugStreamLine("%%  batt %4.2fv %4.2fv ",volt1()/1000.0,volt2()/1000.);
    #if DEBUG == YES
        writeDebugStreamLine("%% * pre-auton ");
        writeDebugStreamLine("%%    pre-initializer ");
        writeDebugStreamLine("%%P L_ANGM = %d",BAT_LONG_PULL_ANG_MAX);
        writeDebugStreamLine("%%P L_ANG1 = %d",BAT_LONG_PULL_ANG1);
        writeDebugStreamLine("%%P L_ANG2 = %d",BAT_LONG_PULL_ANG2);
        //writeDebugStreamLine("%%P L_ANG3 = %d",BAT_LONG_PULL_ANG3);
        writeDebugStreamLine("%%P S_V    = %d",BAT_SHORT_PULL_V);
        writeDebugStreamLine("%%P S_ANG1 = %d",BAT_SHORT_PULL_ANG1);
        writeDebugStreamLine("%%P S_ANG2 = %d",BAT_SHORT_PULL_ANG2);
        writeDebugStreamLine("%%P M_V    = %d",BAT_MEDIUM_PULL_V);
        writeDebugStreamLine("%%P M_ANG1 = %d",BAT_MEDIUM_PULL_ANG1);
        writeDebugStreamLine("%%P M_ANG2 = %d",BAT_MEDIUM_PULL_ANG2);
    #endif
    sensor_reset();
    global_variable_reset();
    lcd_selection();
    lcd_msg_init();
    startTask(somnus_monitor, 9);
    startTask(somnus_bat, 9);
    startTask(somnus_feeder, 9);
    startTask(somnus_remote, 9);
}

/*
    AUTONOMOUS
    task
*/

task autonomous()
{
    #if DEBUG == YES
        writeDebugStreamLine("%% * autonomous ");
    #endif
    clearTimer(T1);
	if(MODE==AUTO_A)      {auto_A(); lcd_message();}
	if (MODE==AUTO_B )    {auto_B(); lcd_message();}
	//if (MODE==AUTO_C )    {auto_C();	   lcd_message();}
	if (MODE==PRG_SKILL ) {prg_skills(28); lcd_message();}
}

/*
    USER CONTROL
    task
*/

task usercontrol()
{
    #if DEBUG == YES
        writeDebugStreamLine("%% * usercontrol ");
    #endif
    global_variable_reset();
    startTask(somnus_monitor, 9);
    startTask(somnus_bat, 9);
    startTask(somnus_feeder, 9);
    startTask(somnus_remote, 9);
	move_x(STOP);

    while(true){
        // bailout logic
        if (is_bailed_out()==1){
            bat(STOP); intaker(STOP); feeder(STOP); move_x(STOP);
            BAT_CMD = STOP; BAT_MONITOR = STOP;
            FEEDER_CMD = STOP; FEEDER_MONITOR = STOP;
            WHEEL_MONITOR = STOP;
        }

        //Reselect mode by pushing any button, restart everything by holding center button
        t_push_center = time1[T1];     // Time for holding center
        if(nLCDButtons == LEFTBUTTON || nLCDButtons == RIGHTBUTTON || nLCDButtons == CENTERBUTTON ){
            #if DEBUG==YES
                writeDebugStreamLine("goto reselect");
            #endif
            lcd_mode_selection();
        }

        //Controls for normal driving, robot skills and test b
        if(MODE == AUTO_A || MODE == AUTO_B || MODE == AUTO_C || MODE == RBT_SKILL || MODE == TEST_B ){
            lcd_message();  // keep on print message -- based on modes
            drv_motion_ctrl();
            slow_bat_ctrl();

            // Phoenix Control
            if(SHIFT_ON ==1 && PUSH_RELEASE[1] ==1){
                BAT_CMD = PHOENIX;
                SHIFT_ON = 0;
                PUSH_RELEASE[1] = 0;
            }

            // Intake Control (Disabled in lift)
            if(LIFT_MODE == OFF){
                if(vexRT[Btn5D] == 1) intaker(IN*127);
                if(vexRT[Btn5U] == 1) intaker(STOP);
                if(vexRT[Btn6D] == 1) intaker(OUT*127);
            }

            // Lift Control
            if (LIFT_MODE==ON){
                if(vexRT[Btn5U] == 1) bat(PUSH*127); //lift up
                //if(vexRT[Btn6U] == 1) bat(STOP); //stop lift
                if(vexRT[Btn5D] == 1) bat(PULL*50); // lift down much slowly
                if(vexRT[Btn6U] == 1){
                	feeder(PUSH * 127);
                	wait1Msec(100);
                	feeder(STOP);
              	}
            }

            shooting_dist_ctrl();

            // Shooting Button (Disabled in lift)
            if(vexRT[Btn6UXmtr2] == 1 && LIFT_MODE == OFF){
                if (BAT_MODE==SINGLE && DIST==SHORT) BAT_CMD = SHORT;
                if (BAT_MODE==SINGLE && DIST==MEDIUM) BAT_CMD = MEDIUM;
                if (BAT_MODE==SINGLE && DIST==LONG) BAT_CMD = LONG;
                if (BAT_MODE==CONTINUOUS ) {
                   // in ROBOT SKILLS ETC -- do we need to turn on auto_adjust???

                    BAT_NUMBER = 10; //****BAT NUMBER subject to change
                    // AUTO_ADJUST = ON;
                    AUTO_ADJUST = USE_AUTO_ADJUST;
                    BAT_CMD = CONTINUOUS;
                    hold_bat_N(30000);
                    AUTO_ADJUST = OFF;
                    // Smart shift from continuous to single
                    if (BAT_COUNT_TOTAL>19)  BAT_MODE=SINGLE;
                }
            }
        }

        //Extra controls for test b
        if( MODE == TEST_B){
            // Test auto_A
            if(vexRT[Btn8L] == 1 ) auto_A();

            // Test programming skills with less shots (5) to test position and grab ball logics
            if(vexRT[Btn8U] == 1 ){
                // Hit 2 bats, wait until shoot is over
                // BAT_NUMBER =2; BAT_CMD = CONTINUOUS; hold_bat_N(4000);
                // magic_1();
                // prg_skills(5);
                prg_skills(2);
            }

            // Test manuever to auto intake balls
            if(vexRT[Btn8R] == 1 ){
                intaker(IN*127);
                wait1Msec(2);
                move_x_by_slow(5000, 4000);
                intaker(STOP);
            }

            // test auto_B
            if(vexRT[Btn8D] == 1 ) auto_B();

            if(vexRT[Btn8UXmtr2] == 1){
            	 int t1=time1[T1];
               // BAT_NUMBER = 8; //****BAT NUMBER subject to change
               BAT_NUMBER = 8; //****BAT NUMBER subject to change

               AUTO_ADJUST = ON;
               BAT_CMD = CONTINUOUS;
               hold_bat_N(15000);

               AUTO_ADJUST = OFF;
               writeDebugStreamLine("*** %d",time1[T1]-t1);
          	}
        }

        //Extra controls for robot skills
        if(MODE == RBT_SKILL){
            if(vexRT[Btn8L] == 1) magic_1();
            // if(vexRT[Btn8L] == 1) magic_2();
            // turn it off, no magic continuous shooting
            // if(vexRT[Btn8U] == 1){
                // BAT_MODE = CONTINUOUS;
                // BAT_NUMBER = 30;
                // AUTO_ADJUST = ON;
                // AUTO_ADJUST = USE_AUTO_ADJUST;
                // BAT_CMD = CONTINUOUS;
                // hold_bat_N(35000);
                // AUTO_ADJUST = OFF;
                // if (BAT_COUNT_TOTAL>28 && BAT_COUNT_TOTAL<36)  magic_2();
                // BAT_MODE = SINGLE;
                // //if (BAT_COUNT_TOTAL>28 && BAT_COUNT_TOTAL<36)  magic_1();
            // }
        }

        //Test a
        if (MODE==TEST_A){
            sprintf(lcdStr1, "%1d%1d%1d %2d %2d %2d", LIFT_MODE,BAT_MODE-100,DIST-100, volt1()/100,volt2()/100, BAT_SHORT_HIT_V-BAT_SHORT_HIT_V_BASE);
            drv_motion_ctrl();
            if(vexRT[Btn5DXmtr2] == 1) auto_A(); //TEMPORARY

            // test pull slightly
            int ang1=0;  int ang2=0;  int ang3=0;
            int rpm1=0;  int rpm2=0; int rpm3=0;
            if(vexRT[Btn7L] == 1){
                BAT_MONITOR = START;
                bat(PULL*127);
                wait1Msec(100);
                bat(STOP);
                BAT_MONITOR = STOP;
                if (Bat_val>ang1) ang1=Bat_val;
                if (Bat_rpm>rpm1) rpm1=Bat_rpm;
                sprintf(lcdStr2, "A=%3d RPM=%3d",ang1,rpm1);
            }



            // Test turning 5 motors
            if(vexRT[Btn7R] == 1){
                // BAT_MONITOR = START;
                motor[M_BAT_1] = 127;  wait1Msec(100); ang1=Bat_val; rpm1=Bat_rpm;
                writeDebugStreamLine("%%M_BAT_1 %d %d",ang1,rpm1);
                motor[M_BAT_1] = 0;    wait1Msec(1000);
                motor[M_BAT_1] =-127;  wait1Msec(100);
                motor[M_BAT_1] = 0;    wait1Msec(1000);
                motor[M_BAT_2] = 127;  wait1Msec(100); ang2=Bat_val; rpm2=Bat_rpm;
                writeDebugStreamLine("%%M_BAT_2 %d %d",ang2,rpm2);
                motor[M_BAT_2] = 0;    wait1Msec(1000);
                motor[M_BAT_2] =-127;  wait1Msec(100);
                motor[M_BAT_2] = 0;    wait1Msec(1000);
                motor[M_BAT_3] = 127;  wait1Msec(100);ang3=Bat_val; rpm3=Bat_rpm;
                writeDebugStreamLine("%%M_BAT_3 %d %d",ang3,rpm3);
                motor[M_BAT_3] = 0;    wait1Msec(1000);
                motor[M_BAT_3] =-127;  wait1Msec(100);
                motor[M_BAT_3] = 0;    wait1Msec(1000);
                // BAT_MONITOR = STOP;
                sprintf(lcdStr2, "A=%4d %4d %4d",ang1,ang2,ang3);
                // displayLCDString(1,0, lcdStr2);
            }

            // move and rotation
            int x0, ang0;
            if(vexRT[Btn8U] == 1 || vexRT[Btn8D] == 1 || vexRT[Btn8L] == 1|| vexRT[Btn8R] == 1){
                WHEEL_MONITOR=START;
                getWheelVal();
                x0 = Wheel_val; ang0 = Gyro_val;
                if(vexRT[Btn8U] == 1) move_x_by( 500, 1500);
                if(vexRT[Btn8D] == 1) move_x_by(-500, 1500);
                if(vexRT[Btn8L] == 1) rotate_by(-150 , 1500);
                if(vexRT[Btn8R] == 1) rotate_by( 150 , 1500);
                // if(vexRT[Btn8U] == 1) move_x_by( 2400, 1500);
                // if(vexRT[Btn8D] == 1) move_x_by(-2400, 1500);
                // if(vexRT[Btn8L] == 1) rotate_by(-900 , 1500);
                // if(vexRT[Btn8R] == 1) rotate_by( 900 , 1500);
                getWheelVal();
                sprintf(lcdStr2, "%3d %3d %3d %3d",ang0/10,Gyro_val/10,x0/10,Wheel_val/10);
                WHEEL_MONITOR=STOP;
            }

            // Test autos
            if(vexRT[Btn5D] == 1) auto_A();
            if(vexRT[Btn6U] == 1) auto_B();
        }
    }
}

void drv_motion_ctrl()
{
	int X, R;
    X = vexRT[Ch2];
	R = vexRT[Ch4];
	// Threshold
	if (abs(X) < MOTION_DRV_THRESHOLD) {X=0;}
	if (abs(R) < MOTION_DRV_THRESHOLD) {R=0;}
    //Apply the voltage
	motor[M_WHEEL_L]  = X+R;
	motor[M_WHEEL_R]  = X-R;
	// motor[M_WHEEL_L_2]  = X+R;
	// motor[M_WHEEL_R_2]  = X-R;
}

void slow_bat_ctrl()
{
	int X;
    if (BAT_CMD==STOP){
        X = vexRT[Ch2Xmtr2];
        //Threshold
        if (abs(X) < 40) X=0;
        //Move bat at slow speed
        bat(X/3);
    }
}

void shooting_dist_ctrl()
{
    if(SHIFT_ON==0 && PUSH_RELEASE[2]==1 && DIST != SHORT ) {
        DIST = SHORT;
        BAT_SHORT_HIT_V=BAT_SHORT_HIT_V_BASE;
        PUSH_RELEASE[0]=0;
    }
    if(SHIFT_ON==0 && PUSH_RELEASE[1]==1 && DIST != MEDIUM) {
        DIST = MEDIUM;
        BAT_MEDIUM_HIT_V=BAT_MEDIUM_HIT_V_BASE;
        PUSH_RELEASE[1]=0;
    }
    if(SHIFT_ON==0 && PUSH_RELEASE[0]==1 && DIST != LONG) {
        DIST = LONG;
        BAT_LONG_PULL_ANG_MAX  = BAT_LONG_PULL_ANG_MAX_BASE;
        PUSH_RELEASE[2]=0;
    }
    if(SHIFT_ON==0 && PUSH_RELEASE[4]==1) {
        rotate_by_step(LEFT);
        PUSH_RELEASE[4]=0;
    }
    if(SHIFT_ON==0 && PUSH_RELEASE[5]==1) {
        // if (DIST==LONG)       { BAT_LONG_PULL_ANG_MAX  += 8; BAT_LONG_PULL_ANG2 +=8; }
        if (DIST==LONG)       { BAT_LONG_PULL_ANG_MAX  += 8;  }
        if (DIST==MEDIUM)     { BAT_MEDIUM_HIT_V       += 4;}
        if (DIST==SHORT)      { BAT_SHORT_HIT_V        += 2;}
        PUSH_RELEASE[5]=0;
    } //8U
    if(SHIFT_ON==0 && PUSH_RELEASE[6]==1) {
        rotate_by_step(RIGHT);
        PUSH_RELEASE[6]=0;
    }
    if(SHIFT_ON==0 && PUSH_RELEASE[7]==1) {
        // if (DIST==LONG)       { BAT_LONG_PULL_ANG_MAX -= 8; BAT_LONG_PULL_ANG2 -= 8; }
        if (DIST==LONG)       { BAT_LONG_PULL_ANG_MAX -= 8;  }
        if (DIST==MEDIUM)     { BAT_MEDIUM_HIT_V      -= 4;}
        if (DIST==SHORT)      { BAT_SHORT_HIT_V       -= 2;}
        PUSH_RELEASE[7]=0;
    } //8D
}

void rotate_by_step(int dir)
{
    #if DEBUG_WHEEL>0
        getWheelVal();
        int rot0= Gyro_val;
    #endif
    rotate(dir*ROT_STEP_VOL);
    wait1Msec(ROT_STEP_DT);
    rotate(STOP);
    #if DEBUG_WHEEL>0
        getWheelVal();
        lcdStr2 =  sprintf(lcdStr2,"%4d %4d %4d",rot0,Gyro_val,Gyro_val-rot0);
        displayLCDString(1,0, lcdStr2);
    #endif
}
