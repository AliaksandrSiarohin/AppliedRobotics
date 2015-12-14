#include <stdio.h>
#include <math.h>
#include "kernel.h"
#include "ecrobot_interface.h"
DeclareTask(task1);
DeclareCounter(SysTimerCnt);
int prev_rev = 0;
double gain = 10;
double alpha = 0.075;
double T = 0.005; // period sampling time
double reference_speed = 10.0;

void user_1ms_isr_type2(void)
{
  StatusType ercd;
  ercd = SignalCounter(SysTimerCnt);
  if (ercd != E_OK){ShutdownOS(ercd);}
}
double speed(double raw_speed) {
	static double prev_speed = 0.0;
	prev_speed = alpha * raw_speed + (1-alpha)*prev_speed;
	return prev_speed;
}
double controller(double u_2) {
	static double u_0 = 0.0, u_1 = 0.0;
	static double y_0 = 0.0, y_1 = 0.0;

	double y_2 = 1/(4 + 42 * T) *
		((gain * (u_2 * (4 + 100*T*T + 40*T) + u_1 * (-8 + 200*T*T) + u_0 * (4 - 40*T + 100*T*T))) - y_1 * (-8) - y_0 * (4 - 42*T));
	u_0 = u_1;
	u_1 = u_2;
	y_0 = y_1;
	y_1 = y_2;
	return y_2;
}
TASK(task1) // called every 5 ms
{
	//get motor evolution
	int current_rev = nxt_motor_get_count(NXT_PORT_B);
	//find out the speed
	int space = current_rev - prev_rev; //how space it takes in degree
	//speed_deg = space / time_step; // degree / s
	double speed_rad = speed((space * (M_PI / 180.0)) / T); // radiant / s
	double error = reference_speed - speed_rad;
	double power = controller(error);
	prev_rev = current_rev;

	//adjust the motor power
	nxt_motor_set_speed(NXT_PORT_B, power, 1);

	//print to display
    display_clear(0);
    display_goto_xy(0,0); //(horizontal [0:15], vertical[0:7])
    display_int(power, 3);
    display_goto_xy(0,1);
    display_int(speed_rad, 3);
    display_update();
    TerminateTask();
}
