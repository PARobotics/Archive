#ifndef PR_BUTTON_C
#define PR_BUTTON_C
// PR_button.c converts pushbutton or limitswitches into push and release button
// November 2016
//
// How to use the code from driver code -- example below
// -- initialization
//   static int NUM_PR_BUTTONS = 2;  // define how many buttons to monitor as PR buttons
//   #include "./Libs/PR_Button.c"   // include this file
//   add_pr_button(0,1,Btn7L)        // add vexRT[Btn7L] to be the first monitored PR button
//   add_pr_button(1,3,dgtl2)        // add limitswitch on dgtl2 to be the second monitored PR button
//   start_pr_button();              // now start the pr button monitoring tasks
                                     // note that this needs to be called again in driver control (task stuff)
// -- use it in the code
//  if (get_pr_button(1) == 1) {
//      ....
//      reset_pr_button(1)
//  }
//
// Note
//   1. to make code more flexible, the index can be saved as a variable instead of hard coded
//   2. the task is monitored every 10 msec -- can be changed if needed
//
//

/*
  Statuses:
  1: Button pushed and released
  2: Button long held
*/

// NUM_PR_BUTTONS needs to be defined before include thfilelength

// create global data array PRN
// -- define data struct PRB_type
typedef struct {
    int type;    // 1: vexRT, 2: ; 3: limitSwitch
    int port;
    int pushed;  // temporary variable to hold that the button has been pushed
    int status;  // 1: yes push and released; 0 no
    int timePushed;
} PRB_type;

// -- create the array PRB of data type PRB_type
static PRB_type PRB[ NUM_PR_BUTTONS ];

// public interface
// -- add_pr_button
void add_pr_button(int idx, int type, int port)
{
  /*
  add_pr_button(0,1,Btn7L)   add vexRT[Btn7L] to be the first monitored PR button
  add_pr_button(1,3,dgtl2)   limitswitch on dgtl2 to be the second monitored PR button
  */
  PRB[idx].type = type;
  PRB[idx].port = port;
}

// -- start_pr_button
task PR_Monitor();
void start_pr_button()
{
  startTask(PR_Monitor, 9);
}

// -- get_pr_button
int get_pr_button(int idx)
{
  return PRB[idx].status;
}

// -- reset_pr_button
void reset_pr_button(int idx)
{
  PRB[idx].status = 0;
}


// PR monitor task
task PR_Monitor()
{
  int sv = 0;     // temp variable to hold sensor value

  while (1) {
    for(int idx=0; idx<NUM_PR_BUTTONS; idx++)
    {
      if (PRB[idx].type == 1) {
        sv = vexRT[PRB[idx].port];
      }
      else if (PRB[idx].type == 3) {
        sv = SensorValue(PRB[idx].port);
      }
      // as soon as a botton is pushed
      if (sv == 1) {
      	if(PRB[idx].pushed == 0) PRB[idx].timePushed = time1[T1];
        PRB[idx].pushed = 1;
      }
      // alreay pushed and now released
      if (sv == 0 && PRB[idx].pushed == 1) {
         PRB[idx].pushed = 0;
         if(time1[T1] - PRB[idx].timePushed >= 1000){
           PRB[idx].status = 2;
         }
         else PRB[idx].status = 1;
      }
      wait1Msec(10);         // I think monitor at 10 msec is over kill but keep it as is
    }
  }
}
#endif //PR_BUTTON_C
