#include <scicos/scicos_block.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <assert.h>

// BAD BAD STUFF
#include <stdarg.h>
FILE * output;

static
void __attribute__((constructor)) startup_shit () 
{
    output = fopen("/tmp/logging", "wt");
}

static
void log_shit (const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(output, fmt, args);
    fflush(output);
    va_end(args);
}

static
void __attribute__((destructor)) shutdown_shit ()
{
    fclose(output);
}

// Include needed to have the bro_fist_t structure

#include "../src/headers/bro_fist.h"

int sensor_count = 0;
int motor_count = 0;

void bro_encode_sci_datablock (bro_fist_t *data_block, double const * params)
{
    switch ((uint8_t)params[2])
    {
        case 1:
            data_block->port = PORT_1;
            break;
        case 2:
            data_block->port = PORT_2;
            break;
        case 3:
            data_block->port = PORT_3;
            break;
        case 4:
            data_block->port = PORT_4;
            break;
        case 5:
            data_block->port = MOTOR_A;
            break;
        case 6:
            data_block->port = MOTOR_B;
            break;
        case 7:
            data_block->port = MOTOR_C;
            break;
        default:
            break;
    };
    
    switch ((uint8_t)params[1])
    {
        case 1:
            data_block->operation = TACHO_COUNT;
            break;
        case 2:
            data_block->operation = AVG_SPEED;
            break;
        case 3:
            data_block->operation = LIGHT_SENSOR;
            break;
        case 4:
            data_block->operation = TOUCH_SENSOR;
            break;
        case 5:
            data_block->operation = SOUND_SENSOR;
            break;
        case 6:
            data_block->operation = RADAR_SENSOR;
            break;
        case 7:
            data_block->operation = SET_POWER;
            break;
        case 8:
            data_block->operation = SET_SPEED;
            break;
        default:
            break;
    };
    
}

void bro_encode_sci_servo (bro_fist_t *data_block, double const * params)
{
    switch ((uint8_t)params[1])
    {
        case 1:
            data_block->port = MOTOR_A;
            break;
        case 2:
            data_block->port = MOTOR_B;
            break;
        case 3:
            data_block->port = MOTOR_C;
            break;
        default:
            break;
    };
    
    switch ((uint8_t)params[0])
    {
        case 1:
            data_block->operation = SET_POWER;
            break;
        case 2:
            data_block->operation = SET_SPEED;
            break;
        default:
            break;
    };

    data_block->data = params[2];
    
    //log_shit("Set Packet. Port %i, Operation %i, Data %.2f\n",
    //data_block->port, data_block->operation, data_block->data);
}

void bro_decode_sci_datablock (bro_fist_t const * data_block, double * param)
{
    //log_shit("Ho ricevuto il dato: %.2f\n", data_block->data);
    *param = (double)data_block->data;
}


int bro_comm_init (scicos_block *block)
{
    int rc, length;
    // char   buffer[BUFFER_LENGTH];
    struct sockaddr_un serveraddr;
    int global_sd = -1;
    
    log_shit("Init. Socket\n");
    
    /********************************************************************/
    /* The socket() function returns a socket descriptor, which represents   */
    /* an endpoint.  The statement also identifies that the UNIX  */
    /* address family with the stream transport (SOCK_STREAM) will be   */
    /* used for this socket.                                            */
    /********************************************************************/
    global_sd = socket(AF_UNIX, SOCK_STREAM, 0);
    log_shit("Socket Descriptor: %i\n", global_sd);
    if (global_sd < 0)
    {
        perror("socket() failed");
        return -1;
    }


    log_shit("Linking it to the system socket\n");
    /********************************************************************/
    /* If an argument was passed in, use this as the server, otherwise  */
    /* use the #define that is located at the top of this program.      */
    /********************************************************************/
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sun_family = AF_UNIX;
    strcpy(serveraddr.sun_path, SERVER_PATH);
    
    log_shit("Connecting...\n");
    
    /********************************************************************/
    /* Use the connect() function to establish a connection to the      */
    /* server.                                                          */
    /********************************************************************/
    rc = connect(global_sd, (struct sockaddr *)&serveraddr, SUN_LEN(&serveraddr));
    if (rc < 0)
    {
        perror("connect() failed");
        return -1;
    }

    log_shit("Done, setting connection params :3\n");
    length = sizeof(bro_fist_t) * BUFFER_SIZE;
    rc = setsockopt(global_sd, SOL_SOCKET, SO_RCVLOWAT,
    (char *)&length, sizeof(length));

    if (rc < 0)
    {
        perror("setsockopt(SO_RCVLOWAT) failed");
        return -1;
    }

    log_shit("Ok, all done!\n");
    
    block->outptr[0][0] = (double)global_sd;
    
    return 0;
}


int bro_comm_kill (scicos_block *block)
{
    int rc;
    bro_fist_t packet[BUFFER_SIZE];
    memset(packet, 0, sizeof(bro_fist_t)*BUFFER_SIZE);

    packet[0].operation = BRO_END_COMMUNICATION;
   
    log_shit("Allora, guarda, io ho settato il pacchetto così:\n");
    log_shit("Operazione: %i, Porta: %i, Dati: %.2f\n", packet[0].operation,
    packet[0].port, packet[0].data);

    rc = send((int)block->outptr[0][0], &packet, sizeof(bro_fist_t) * BUFFER_SIZE, 0);
    
    log_shit("Sent END COMMUNICATION order.\n");

    if (rc < 0)
    {
        perror("send() failed");
        return -1;
    }
    
    rc = recv((int)block->outptr[0][0], &packet, sizeof(bro_fist_t) * BUFFER_SIZE, 0);
    //log_shit("%d bytes of data were received\n", rc);
    if (rc < 0)
    {
        perror("recv() failed");
        return -1;
    }
 
    close((int)block->outptr[0][0]);
    
    return 0;
}

void bro_comm_controller (scicos_block *block, int flag)
{
	switch (flag) {
		case 1:	/* set output */
		    break;
		case 2:	/* get input */
			break;
		case 4:	/* initialisation */
		    bro_comm_init(block);
	        log_shit("Communication Initialized! Socked Descriptor: %i "
                   "[%p]\n", (int)block->outptr[0][0], &block->outptr[0][0]);
            break;
		case 5:	/* ending */
	        log_shit("Closing Communications. Socked Descriptor: %i "
                   "[%p]\n", (int)block->outptr[0][0], &block->outptr[0][0]);
			bro_comm_kill(block);
			log_shit("Communications Closed! :D\n");
            break;
	    default:
	        break;
	}
}

int bro_sens_send (scicos_block *block)
{
    int rc, i;
    bro_fist_t out_packet[BUFFER_SIZE];
    memset(out_packet, 0, sizeof(bro_fist_t)*BUFFER_SIZE);

    for (i = 1; i < block->nin; i++) {
        bro_encode_sci_datablock(&out_packet[i-1], block->inptr[i]);
    };
    
    //log_shit ("Data for first block: %i, %i, %.2f\n", out_packet[0].port,
    //out_packet[0].operation, out_packet[0].data);
    
    rc = send((int)block->inptr[0][0], out_packet, sizeof(bro_fist_t) * BUFFER_SIZE, 0);
    
    if (rc < 0)
    {
        perror("send() failed");
        return -1;
    }

    //log_shit("%d bytes of data were sent\n", rc);
    return 0;
}

int bro_sens_read (scicos_block *block)
{
    int rc, i;
    bro_fist_t packet[BUFFER_SIZE];
    memset(packet, 0, sizeof(bro_fist_t)*BUFFER_SIZE);

    rc = recv((int)block->inptr[0][0], packet, sizeof(bro_fist_t) * BUFFER_SIZE, 0);
    //log_shit("%d bytes of data were received\n", rc);
    if (rc < 0)
    {
        perror("recv() failed");
        return -1;
    }
    
    //log_shit("Starting to set outputs :3 [%i]\n", block->nout);
    
    for (i = 0; i < block->nout; i++) {
        //log_shit("Next Step defining outputs :D [%i]\n", i);
        bro_decode_sci_datablock(&packet[i], &block->outptr[i][0]);
        //log_shit("Output value for port %i is: %.2f[%i]\n", i, block->outptr[i][0], block->outsz[(2*block->nout)+i]);
        
    }
    
    return 0;
}

void bro_comm_sens_disp (scicos_block *block, int flag)
{
	switch (flag) {
		case 1:	/* set output */
		    //log_shit("Output computation begin!\n");
            sensor_count++;
            log_shit("Called bro_comm_sens_disp the %ith time\n",
            sensor_count);
            bro_sens_send(block);
            
		    bro_sens_read(block);
		    //log_shit("Output computation complete! :D\n");
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

int bro_motor_set (scicos_block *block)
{
    int rc, i;
    bro_fist_t out_packet[BUFFER_SIZE];
    bro_fist_t packet[BUFFER_SIZE];
    
    memset(out_packet, 0, sizeof(bro_fist_t)*BUFFER_SIZE);
    memset(packet, 0, sizeof(bro_fist_t)*BUFFER_SIZE);

    for (i = 1; i < block->nin; i++) {
        bro_encode_sci_servo(&out_packet[i-1], block->inptr[i]);
    };
    
    //log_shit ("Data for first block: %i, %i, %.2f\n", out_packet[0].port,
    //out_packet[0].operation, out_packet[0].data);
    
    rc = send((int)block->inptr[0][0], out_packet, sizeof(bro_fist_t) * BUFFER_SIZE, 0);
    
    if (rc < 0)
    {
        perror("send() failed");
        return -1;
    }

    rc = recv((int)block->inptr[0][0], packet, sizeof(bro_fist_t) * BUFFER_SIZE, 0);
    //log_shit("%d bytes of data were received\n", rc);
    if (rc < 0)
    {
        perror("recv() failed");
        return -1;
    }


    //log_shit("%d bytes of data were sent\n", rc);
    return 0;

return 0;
}

void bro_comm_motor_disp (scicos_block *block, int flag)
{
	switch (flag) {
		case 1:	/* set output */
		    motor_count++;
            log_shit("Called bro_comm_motor_disp the %ith time.\n",
            motor_count);
            bro_motor_set(block);
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
