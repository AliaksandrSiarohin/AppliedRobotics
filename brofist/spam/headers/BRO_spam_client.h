/** @file BRO_spam_client.h */
#ifndef __bro_headers_spam_client_h
#define __bro_headers_spam_client_h

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

#include "../../src/headers/bro_fist.h"

/** @addtogroup BROSClient */
/* @{ */
/*--------------------------------------------------------------------------*/
/* Definitions                                                              */
/*--------------------------------------------------------------------------*/

/*  It's a shame, but it's required to declare on which port are connected the
 *  sonar or the light sensor. It's messy but there are some initialisation that
 *  have to be performed even BEFORE the first packet. XD
 */

/** Ultrasonic Distance Sensor Flag.
 *  Set to @c 1 if connected, @c 0 if not present.
 */
#define CONN_SONAR      1           
/** Ultrasonic Distance Sensor PORT.
 *  Use the standard nxtOSEK port descriptors to tell the system which port
 *  the Sonar is connected to.
 */
#define SONAR_PORT      NXT_PORT_S1

/** Light Sensor Flag.
 *  Set to @c 1 if connected, @c 0 if not present.
 */
#define CONN_LIGHT      1            
/** Ultrasonic Distance Sensor PORT.
 *  Use the standard nxtOSEK port descriptors to tell the system which port
 *  the Sonar is connected to.
 */
#define LIGHT_PORT      NXT_PORT_S2 

#define PID_CONTROLLED  1           //
#define RAW_POWER       0           //
#define NOT_USING       -1          // This is the initial condition and if not
                                    // used... :3

/** Number of samples for Average Speed Computation. */
#define SAMP_NUM       10

/** Servo Motor's descripting structure.
 *
 *  The data stored insiThde this kind of structure is used for various
 *  purposes, such as PID Control, Average Speed computation and things
 *  like that.
 */

typedef struct {

        /** Kind of Speed Control for this Servo Motor.
         *  Please refer to the Definitions to know which values shall be
         *  used for this variable.
         */
        S8 speed_control_type;

        /** Target speed for PID Controller. */
        U32 speed_ref;

        /** Last SAMP_NUM Tachometric Readings.
         *  Used by the avg() function to compute the Average Speed for the
         *  current Servo Motor.
         */
        U32 revolutions[SAMP_NUM];  

        /** Last SAMP_NUM System Times */
        U32 times[SAMP_NUM];        

        /** Last SAMP_NUM Average Speeds. */
        F32 speeds[SAMP_NUM];

        /** Last three RAW Powers set.
         *  Used by the PID controller or, if on RAW Power Control, only
         *  the first value is used to set the power.
         */
        F32 powers[3];

        /** Last three Speed Errors.
         *  This is the difference between the motor_t::speed_ref and the
         *  current Average Speed. This value is needed by the PID
         *  Controller.
         */
        F32 speed_errors[3];

        /** Servo Motor's Port. */
        U8  port;

        /** Circular Array Counter for AVG Speed Computing. */
        U32 counter;
        /** Previous Slot in the Circular Array. */
        S32 previous_counter;       

} motor_t;

/** Structure containing all the Servo Motors' Strunctures. */
typedef struct {
    motor_t first;
    motor_t second;
    motor_t third;
} engines_t;

/* @} */

#endif
