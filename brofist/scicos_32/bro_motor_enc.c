#include <scicos/scicos_block.h>
#include <stdio.h>
#include <unistd.h>

/* Block encoding function. Nothing hard, simply... It's necessary.
 * First output value is an identifier for the request. Simply it tells the
 * dispatcher whether the request is to read a sensor (1) or set a motor (2).
 * However the output is a two slots array that contains in the first place the
 * Sensor type and the second the port number (1 to 4 for sensor ports 1 to 4
 * and 5 to 7 for motor ports A to C).
 * The Sensor types are:
 *
 * 1 -> RAW Tacho counter
 * 2 -> AVG Speed (computed by the NXT Block)
 * 3 -> Light Sensor
 * 4 -> Touch Sensor
 * 5 -> Sound Sensor
 * 6 -> Ultrasonic (Distance) Sensor
 */

void bro_motor_enc_run (scicos_block *block)
{
    block->outptr[0][0] = block->ipar[0];
    block->outptr[0][1] = block->ipar[1];
    block->outptr[0][2] = block->inptr[0][0];
}


void bro_motor_enc (scicos_block *block, int flag)
{
	switch (flag) {
		case 1:	/* set output */
		    bro_motor_enc_run(block);
		    break;
		case 2:	/* get input */
			break;
		case 4:	/* initialisation */
	        break;
		case 5:	/* ending */
			break;
	    default:
	        break;
	}
}
