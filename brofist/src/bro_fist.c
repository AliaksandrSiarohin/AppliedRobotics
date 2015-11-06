#include "headers/bro_fist.h"
#include "headers/bro_comm.h"
#include "headers/bro_opts.h"
#include <unistd.h>

const float MOTOR_POWER = 50.0; //Power to apply to motor
const uint32_t MEASURE_TIME = 5000; //Time in ms, for running the motor
const int WRITE_PACKETS_TO_STD = 1;
const uint32_t READ_PERIOD = 100; //Sleep time between consequent querys of brick

main (int argc, char *argv[])
{
    int bro_server_socket = -1;
    int bro_client_socket = -1;
    int bro_spam_socket = -1;
    bro_fist_t in_packet[BUFFER_SIZE];
    bro_fist_t out_packet[BUFFER_SIZE];

	
	char file_name[200];
	sprintf(file_name, "/home/ubuntu/NXT/AppliedRobotics/motor_data/p%i", 
				(int) MOTOR_POWER);	
    
    int i, comm_res, packet_no;
    
    bro_opts_t options;

    
    memset (in_packet, 0, sizeof(bro_fist_t) * BUFFER_SIZE);
    memset (out_packet, 0, sizeof(bro_fist_t) * BUFFER_SIZE);
    
    srand ( time(NULL) );
    
    if (bro_opts_parse (&options, argc, argv) == -1) {
        return 0;
    };
    
	printf("Connecting to the device...\n");

    int conect_status = bro_bt_connect_device (&bro_spam_socket, options.mac);;
    if (conect_status == 0) {
		printf("Connected. Press RUN>.\n");
	} else {
		printf("Can`t connect to brick.\n");
		exit(1);
	}

	printf("Open File: %s", file_name);  
    
	FILE *f = fopen(file_name, "w");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}

	fprintf(f, "TIME, TACHO_COUNT\n");

    packet_no = 0;
	int time = 0 , elapsed_time, start_time, distance;
	int time_setted = 0;
    do {
    	bro_fist_t countFist;
		countFist.operation = TACHO_COUNT;
		countFist.port = MOTOR_B;
		in_packet[0] = countFist;
		if(packet_no == 0) {
			bro_fist_t startFist;
			startFist.operation = SET_POWER;
			startFist.port = MOTOR_B;
			startFist.data = MOTOR_POWER;
			in_packet[1] = startFist;
		} else {
			in_packet[1] = countFist;
		}

        comm_res = bro_bt_client_fist(in_packet, out_packet, bro_spam_socket);       
        if (comm_res < 0) {
        	printf("Conection closed by brick\n");
        	break;
        }
        for (i = 0; i < BUFFER_SIZE; i++) {
			if(out_packet[i].operation == 6) {					
				distance = (int) out_packet[i].data;
				time = out_packet[i].timestamp;
			}			
        }
		if (!time_setted && time != 0) {
			start_time = time;
			time_setted = 1;
		}
		elapsed_time = time - start_time;
		fprintf(f, "%i,%i\n", elapsed_time, distance);
		if (WRITE_PACKETS_TO_STD) {
			printf("TIME: %i, TACHO_COUNT: %i\n", elapsed_time, distance);
		}
		packet_no++;
		//usleep(READ_PERIOD * 1000);
    } while ((time - start_time) < MEASURE_TIME);

    printf("Exited communication loop after %i packets!\n", packet_no);

	printf("closing connection...");
    bro_bt_close_connection(bro_spam_socket);
	fclose(f);
    
    bro_stop_server(bro_server_socket, bro_client_socket);
    
    return 0;
}
