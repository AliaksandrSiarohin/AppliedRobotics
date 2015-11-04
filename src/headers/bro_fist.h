/** @file bro_fist.h */
#ifndef __bro_headers_fist_h
#define __bro_headers_fist_h


#include <stdint.h>
#include <stdlib.h>
#include <time.h>

/** @addtogroup BroFist */
/* @{ */

// Definitions for the Cient-Server model
/** Server path for the UNIX socket. */
#define SERVER_PATH             "/tmp/BROFist"
/** Number of simultaneous operations to send/recv.
 *  This has been set as 7 since the ports on the NXT are 7. For further
 *  insight on this issue, please read @ref Why7.
 */
#define BUFFER_SIZE             7
/** The value that has to be sent to stop the server.
 *
 *  Will there be ever more than 10 operations? XD
 */
#define BRO_END_COMMUNICATION   255

// Definitions for Ports
#define MOTOR_A                 5 ///<Integer representation for Port A.
#define MOTOR_B                 6 ///<Integer representation for Port B.
#define MOTOR_C                 7 ///<Integer representation for Port C.

#define PORT_1                  1 ///<Integer representation for Port 1.
#define PORT_2                  2 ///<Integer representation for Port 2.
#define PORT_3                  3 ///<Integer representation for Port 3.
#define PORT_4                  4 ///<Integer representation for Port 4.

// Definitions for operations
#define LIGHT_SENSOR            1 ///<Integer representation for Light Sensor.
#define TOUCH_SENSOR            2 ///<Integer representation for Touch Sensor.
#define SOUND_SENSOR            3 ///<Integer representation for Sound Sensor.
#define RADAR_SENSOR            4 ///<Integer representation for Ultrasonic Distance Sensor.

#define TACHO_COUNT             6 ///<Integer representation for RAW Tachometric Count.
#define AVG_SPEED               7 ///<Integer representation for Computed Average Speed.

#define SET_SPEED               8 ///<Integer representation for Set PID Controlled Speed.
#define SET_POWER               9 ///<Integer representation for Set RAW Power. 

/** The structure containing a "Fist" for the SPAM.
 *
 *  This structure is needed to request any Sensor's value or to set the
 *  Speed / RAW Power of a Servo Motor.
 */
typedef struct {
    /** Operation to request on the given port.
     *
     *  The value for this field can/should be one of the various defines
     *  declared in this file.
     */
    uint8_t operation;
    /** On wich port is installed the Sensor / Servo Motor we want to
     *  interact with.
     *
     *  The value for this field can/should be one of the various @c PORT
     *  @c MOTOR defines declared in this file.
     */
    uint8_t port;
    /** The data to use in coordination with the given operation.
     *
     *  The value of this field will be ignored with every operation but
     *  SET_POWER and SET_SPEED. This is because the Sensors don't need any
     *  value to be set.
     */
    float data;
}
#ifndef DOXYGEN
__attribute__((__packed__))
#endif
bro_fist_t;

/** The structure containing BUFFER_SIZE BROFists to send to the SPAM.
 *
 *  This structure, as you can read below, contains also the dimension of
 *  the combined BUFFER_SIZE. This is needed by the
 *  ecrobot_read_bt_packet() function of the nxtOSEK system, since it
 *  requires the dimension of the packet to read from the BlueTooth buffer
 *  on the SPAM. A tricky way, really, that is not really needed, since
 *  that function requires both the buffer to write into AND the size of
 *  data to read. Really, it doesn't make much sense, but it works.
 */
typedef struct {
    /** Size of the combined BROFists
     *
     *  It is an uint16_t instead of an uint8_t since the aforementioned
     *  ecrobot_read_bt_packet() function requires the size of the packet
     *  to read to be 2 bytes large but only the first is used. Beats me if
     *  I know why is this way. However, another way to do this could have
     *  been
@verbatim
uint8_t size;
uint8_t padding; // Padding byte. Not used for anything else.
@endverbatim
     */
    uint16_t size;
    /** Set of Operations to send to the SPAM. */
    bro_fist_t packets[BUFFER_SIZE];
}
#ifndef DOXYGEN
__attribute__((__packed__))
#endif
bro_spam_fists_t;

/* @} */
#endif 
