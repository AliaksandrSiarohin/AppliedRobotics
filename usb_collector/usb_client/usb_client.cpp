/* sample.cpp for TOPPERS/ATK(OSEK) */ 

// This program needs to use nxtOSEK/samples_c/usbtest/usbhost/usbhost.exe.

#include <string.h>

// ECRobot++ API
#include "Usb.h"
#include "Lcd.h"
#include "Motor.h"
using namespace ecrobot;

extern "C"
{
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

Usb usb;
Lcd lcd;
Motor motor(PORT_B);

/* nxtOSEK hook to be invoked from an ISR in category 2 */
void user_1ms_isr_type2(void)
{
	usb.commHandler(); // USB communication handler
}


TASK(TaskMain)
{
    const S8 MOTOR_POWER = 50;
	U32 len = 8;

	U32 tick;
	S32 tacho_count;

	U8 data[MAX_USB_DATA_LEN];
	

	lcd.clear();

	lcd.putf("sn", "USB_COLLECTOR");
	lcd.disp();
	U8 is_power_set = 0;
	while(1)
  	{
  		if (usb.isConnected()) // check usb connection
  		{
  			if (!is_power_set) {
  				lcd.putf("sn", "POWER SET.");
  				lcd.disp();
  				is_power_set = 1;
  				motor.setPWM(MOTOR_POWER);
  			}

  			tick = systick_get_ms();
  			tacho_count = motor.getCount();

  			data[0] = (tick >> 24) & 0xFF;
			data[1] = (tick >> 16) & 0xFF;
			data[2] = (tick >> 8) & 0xFF;
			data[3] = tick & 0xFF;

  			data[4] = (tacho_count >> 24) & 0xFF;
			data[5] = (tacho_count >> 16) & 0xFF;
			data[6] = (tacho_count >> 8) & 0xFF;
			data[7] = tacho_count & 0xFF;

			usb.send(data,0,len);
		}
  	}
}
}
