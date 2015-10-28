#include "headers/bro_fist.h"
#include "headers/bro_comm.h"
#include "headers/bro_opts.h"

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
    
    bro_bt_connect_device (&bro_spam_socket, options.mac);
    
    bro_start_server (&bro_server_socket, &bro_client_socket);
    
    packet_no = 0;

    do {
        // TODO: Fare il loop di comunicazione con, magari, una word per il blocco
        comm_res = bro_server_fist(in_packet, out_packet, bro_client_socket, bro_spam_socket);
        
        packet_no++;

        printf("Pacchetto numero: %i\n", packet_no);

        for (i = 0; i < BUFFER_SIZE; i++) {
            printf("Istruzione %i:\n", i+1);
            printf("\tOperazione: %i\n", in_packet[i].operation);
            printf("\tPorta: %i\n", in_packet[i].port);
            printf("\tDati: %.2f\n", in_packet[i].data);
            printf("\tDati settati: %.2f\n\n", out_packet[i].data);
        }

    } while ((in_packet[0].operation != BRO_END_COMMUNICATION) && (comm_res
    >= 0));
    
    printf("Exited communication loop after %i packets!\n", packet_no);

    bro_bt_close_connection(bro_spam_socket);
    
    bro_stop_server(bro_server_socket, bro_client_socket);
    
    return 0;
}
