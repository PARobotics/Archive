#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    Power2,         sensorPotentiometer)
#pragma config(Sensor, in2,    FORK_ANGLE,     sensorPotentiometer)
#pragma config(Sensor, in4,    CLAW_L,         sensorPotentiometer)
#pragma config(Sensor, in6,    CLAW_R,         sensorPotentiometer)
#pragma config(Sensor, I2C_1,  IE_CLAW_L,      sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  IE_CLAW_R,      sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_3,  IE_WHEEL_FL,    sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_4,  IE_WHEEL_BL,    sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_5,  IE_WHEEL_BR,    sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_6,  IE_WHEEL_FR,    sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           M_DUMPER_ML,   tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           M_CLAW_R,      tmotorVex393_MC29, openLoop, encoderPort, I2C_2)
#pragma config(Motor,  port3,           M_CLAW_L,      tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_1)
#pragma config(Motor,  port4,           M_DUMPER_OL,   tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           M_WHEEL_BL,    tmotorVex393_MC29, openLoop, encoderPort, I2C_4)
#pragma config(Motor,  port6,           M_WHEEL_BR,    tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_5)
#pragma config(Motor,  port7,           M_WHEEL_FR,    tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_6)
#pragma config(Motor,  port8,           M_WHEEL_FL,    tmotorVex393_MC29, openLoop, encoderPort, I2C_3)
#pragma config(Motor,  port9,           M_DUMPER_OR,   tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          M_DUMPER_MR,   tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(1200)

#include "Vex_Competition_Includes.c"
#include "Libs/FuncLib.c"     // basic functions
#include "Libs/Pid.c"         // pid functions

// -- Push-release buttons
const int NUM_PR_BUTTONS = 9;    // define how many buttons to monitor as PR buBtn6D
#define PR_Btn7U 0
#define PR_Btn7D 1
#define PR_Btn8L 2
#define PR_Btn8U 3
#define PR_Btn8R 4
#define PR_Btn8D 5
#define PR_Btn5U 6
#define PR_Btn6U 7
#define PR_Btn6D 8
#include "Libs\PR_Button.c"     // include source file
#include "Libs/SmartMotorLib.c"

#include "ss.h"
#include "ss_func.c"
#include "ss_lcd.c"
#include "ss_dump.c"
#include "ss_claw.c"
#include "ss_monitor.c"
#include "ss_auton.c"

/*
PRE AUTONOMOUS
functions
*/

void sensor_reset(){
	nMotorEncoder[M_WHEEL_FR] = 0;
	nMotorEncoder[M_WHEEL_FL] = 0;
	nMotorEncoder[M_WHEEL_BR] = 0;
	nMotorEncoder[M_WHEEL_BL] = 0;
	nMotorEncoder[M_CLAW_L] = 0;
	nMotorEncoder[M_CLAW_R] = 0;
  //SensorValue(G_SENSOR) = 0;
}

void bailout(){
	DUMP_COMMAND = STOP;
	CLAW_COMMAND = STOP;
	move(0, 0, 0);
}

void pre_auton()
{
  bStopTasksBetweenModes = false; //Keep tasks between Auton and user control
	clearTimer(T1);
	sensor_reset();
  //Initialize Button library
  add_pr_button(PR_Btn7U,1,Btn7U);
  add_pr_button(PR_Btn7D,1,Btn7D);
  add_pr_button(PR_Btn8L,1,Btn8L);
  add_pr_button(PR_Btn8U,1,Btn8U);
  add_pr_button(PR_Btn8R,1,Btn8R);
  add_pr_button(PR_Btn8D,1,Btn8D);
  add_pr_button(PR_Btn5U,1,Btn5U);
  add_pr_button(PR_Btn6U,1,Btn6U);
  add_pr_button(PR_Btn6D,1,Btn6D);
  start_pr_button();

  //startTask(MotorSlewRateTask, 9);
	//startTask(ss_monitor, 9);
	startTask(ss_dump, 7);
	startTask(ss_claw, 7);
  CLAW_COMMAND = STOP;
  DUMP_COMMAND = STOP;
  sensor_reset();
	//lcd_selection();
	//lcd_msg_init();
}

/*
 	AUTONOMOUS
	task
*/

task autonomous(){
	clearTimer(T1);
  if(MODE==AUTO_A) auto_A();
	if(MODE==AUTO_B) auto_B();
	if(MODE==AUTO_C) auto_C();

	lcd_message();
}

/*
	USER CONTROL
*/

task usercontrol()
{
	DUMP_COMMAND = STOP;

	int V, H, X;

	SmartMotorsInit();
  SmartMotorsAddPowerExtender(M_WHEEL_FL, M_WHEEL_BL, M_WHEEL_FR, M_WHEEL_BR);
  SmartMotorPtcMonitorEnable();
  SmartMotorSetPowerExpanderStatusPort(in1);
  SmartMotorRun();

	while(true){
		lcd_message();

		if(vexRT[Btn7D] == 1){
			bailout();
		}

		/*
			DRIVE TRAIN
		*/

		V = vexRT[Ch2];
		H = vexRT[Ch4];
		X = -vexRT[Ch1];
		//Prevent against Small Motions
		if (abs(V) < MOTION_DRV_THRESHOLD) {V=0;}
		if (abs(H) < MOTION_DRV_THRESHOLD) {H=0;}
		if (abs(X) < MOTION_DRV_THRESHOLD) {X=0;}

		move(V, H, X);

		/*
			DUMPER
		*/

		if(vexRT[Btn6D] == 1){
			//Up, smart
			CLAW_COMMAND = OPEN;
			//reset_pr_button(PR_Btn6D);
    }
		else if (vexRT[Btn6U] == 1){
			CLAW_COMMAND = CLOSE;
			//reset_pr_button(PR_Btn6U);
		}

		if(vexRT[Btn5U] == 1){
			//Up, force
      DUMP_COMMAND = UP_MANUAL;
		}
		else if(vexRT[Btn5D] == 1){
      //Down force
      DUMP_COMMAND = DOWN_MANUAL;
		}
		else if(DUMP_COMMAND != CARRY){
			DUMP_MONITOR = STOP;
			DUMP_COMMAND = STOP;
			dumper(STOP);
		}

    if (vexRT[Btn7U] == 1) DUMP_COMMAND = CARRY;

		/*
			CLAW
		*/

		if(get_pr_button(PR_Btn8U) == 1){
			auto_A();
			reset_pr_button(PR_Btn8U);
		}

		/*if(get_pr_button(PR_Btn8U) == 1){
			//Toggle Claw
			if(CLAW_COMMAND == OPEN){
				CLAW_COMMAND = CLOSE;
			}
			else{
				CLAW_COMMAND = OPEN;
			}
			reset_pr_button(PR_Btn8U);
		}*/

		/*
			SOUND EFFECTS
		*/

		/*if(get_pr_button(PR_Btn8L) == 1){
			PlaySoundFile("XGonnaGiveItToYou.wav");
		}

		if(get_pr_button(PR_Btn8R) == 1){
			PlaySoundFile("FTP.wav");
		}*/

		wait1Msec(50);

	}
}
