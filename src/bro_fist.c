#include "headers/bro_fist.h"
#include "headers/bro_comm.h"
#include "headers/bro_opts.h"
#include <unistd.h>

int 
main (int argc, char *argv[])
{
    int bro_server_socket = -1;
    int bro_client_socket = -1;
    int bro_spam_socket = -1;
    bro_fist_t in_packet[BUFFER_SIZE];
    bro_fist_t out_packet[BUFFER_SIZE];
    
    int i, comm_res, packet_no;
    
    bro_opts_t options;
    
    memset (in_packet, 0, sizeof(bro_fist_t) * BUFFER_SIZE);
    memset (out_packet, 0, sizeof(bro_fist_t) * BUFFER_SIZE);
    
    srand ( time(NULL) );
    
    
    if (bro_opts_parse (&options, argc, argv) == -1) {
        return 0;
    };
    
	printf("SERVER SIDE (bro_fist.c)\n\n");

    bro_bt_connect_device (&bro_spam_socket, options.mac);
    
    //bro_start_server (&bro_server_socket, &bro_client_socket);
    
	FILE *f = fopen("/home/ubuntu/file1.txt", "w");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}

    packet_no = 0;
	int command = 0;
	int op = 0, port = 0;
	int ncount = 0;
	int time = 0, distance = 0;
    do {
		if(command == 0){ //set motor only one time
			//Set MOTOR_B to 15 
			bro_fist_t startB;
			startB.operation = SET_POWER;
			startB.port = MOTOR_B;
			startB.data = 15;
			in_packet[0]=startB;
			command++;
		}else{
			//Get TACHO_COUNT of MOTOR_B 
			bro_fist_t checkB;
			checkB.operation = TACHO_COUNT;
			checkB.port = MOTOR_B;
			//checkB.data = 15;
			in_packet[0]=checkB;
			//Get LIGHT_SENSOR
			bro_fist_t lS;
			lS.operation = LIGHT_SENSOR;
			lS.port = MOTOR_A;
			//checkB.data = 15;
			in_packet[1]=lS;
		}
        comm_res = bro_bt_client_fist(in_packet, out_packet, bro_spam_socket);
        
        packet_no++;

        printf("Packet number: %i\n", packet_no);

        for (i = 0; i < BUFFER_SIZE; i++) {
            printf("Istruzione %i:\n", i+1);
			//printf("\tOperazione: %i\n", in_packet[i].operation);
			op = in_packet[i].operation;
			if(1 == 1){
		        if(op == 1){ 
					printf("Istruzione LIGHT_SENSOR");
					time = (int) out_packet[i].data;
				}
				else if(op == 2) printf("\tOperazione: TOUCH_SENSOR\n");
				else if(op == 3) printf("\tOperazione: SOUND_SENSOR\n");
				else if(op == 4) printf("\tOperazione: RADAR_SENSOR\n");
				else if(op == 6){
					printf("\tOperazione: TACHO_COUNT\n");
					distance = (int) out_packet[i].data;
				}
				else if(op == 7) printf("\tOperazione: AVG_SPEED\n");
				else if(op == 8) printf("\tOperazione: SET_SPEED\n");
				else if(op == 9) printf("\tOperazione: SET_POWER\n");
				op = 0; //reset op
				port = in_packet[i].port;
				//printf("\tPorta: %i\n", in_packet[i].port);
				if(port == 1) printf("\tPort: PORT_1");
				else if(port == 2) printf("\tPort: PORT_2\n");
				else if(port == 3) printf("\tPort: PORT_3\n");
				else if(port == 4) printf("\tPort: PORT_4\n");
				else if(port == 6) printf("\tPort: MOTOR_A\n");
				else if(port == 7) printf("\tPort: MOTOR_B\n");
				else if(port == 8) printf("\tPort: MOTOR_C\n");
				printf("\tDati: %.2f\n", in_packet[i].data);
				printf("\tDati settati: %.2f\n\n", out_packet[i].data);
			}/*else{
				printf("\tNo operation to do\n\n");
			}*/
        }
		ncount++;
		fprintf(f, "%i\t%i\n", time,distance);
		time = 0;
		distance = 0;
		//usleep(100*1000);
    } while ((in_packet[0].operation != BRO_END_COMMUNICATION) && (comm_res
    >= 0) && (ncount < 200));

    printf("Exited communication loop after %i packets!\n", packet_no);

	printf("closing connection...");
    bro_bt_close_connection(bro_spam_socket);
	fclose(f);
    
    bro_stop_server(bro_server_socket, bro_client_socket);
    
    return 0;
}
