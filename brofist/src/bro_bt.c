#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "headers/bro_bt.h"

/*struct bro_bt_device {
    char       name[248];
    bdaddr_t   mac;
};*/

int bro_bt_connect_device (int * spam_sock, bdaddr_t mac_addr)
{

    int connect_status;    
    
    struct sockaddr_rc addr_data = { 0 };
    
    /* AF_BLUETOOTH means that it's a Bluetooth socket, while BTPROTO_RFCOMM
    * means that we are going to use the RFCOMM protocol to get the data.
    */
    *spam_sock = socket (AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    /* As written on the official guide for the Bluetooth Libraries...
    * struct sockaddr_rc {
    *    sa_family_t	rc_family;
    *    bdaddr_t	    rc_bdaddr;
    *    uint8_t		rc_channel;
    *  };
    * The sockaddr_rc contains various and interesting values. The
    * communication family, the RFCOMM channel and the MAC address :3
    * In here we set all this data. Fuck yea.
    */ 
    addr_data.rc_family   = AF_BLUETOOTH;
    addr_data.rc_channel  = (uint8_t) 1;
    addr_data.rc_bdaddr   = mac_addr;

    // Let's connect to SPAM!
    connect_status = connect(*spam_sock, (struct sockaddr *)&addr_data,
                           sizeof(addr_data));

    return connect_status;
}

size_t bro_bt_scan_devices (bro_bt_device_t *devices[MAX_BT_DEVICES])
{
    inquiry_info *scan_res = NULL;
    size_t num_rsp;
    int dev_id, sock, flags;
    int i;
    char addr[19] = { 0 };
    char name[248] = { 0 };
    
    flags = IREQ_CACHE_FLUSH;
    
    // Initializing BT Device for scan
    dev_id = hci_get_route(NULL);
    sock = hci_open_dev( dev_id );
    
    if (dev_id < 0 || sock < 0) {
      perror("opening socket");
      return -1;
    }

    memset(devices, 0, MAX_BT_DEVICES * sizeof(bro_bt_device_t *));
    
    // Allocate memory for all the devices found
    scan_res = (inquiry_info*)malloc(MAX_BT_DEVICES * sizeof(inquiry_info));
    
    // Scan to find all the devices in range
    num_rsp = hci_inquiry(dev_id, BT_INQUIRY_LEN, MAX_BT_DEVICES, NULL,
                          &scan_res, flags);
    if( num_rsp < 0 ) {
        perror("hci_inquiry");
        return -1;
    }

    // For each of the found devices we retrieve its name
    for (i = 0; i < num_rsp; i++) {
      ba2str(&scan_res[i].bdaddr, addr);
      memset(name, 0, sizeof(name));
      if (hci_read_remote_name(sock, &scan_res[i].bdaddr, sizeof(name), 
          name, 0) < 0) {
        strcpy(name, "[unknown]");
      };
      //devices[i]->name = strdup(name);      // todo FREE on devices elements
      devices[i] = malloc(sizeof(bro_bt_device_t));
      strcpy(devices[i]->name, name);
      memcpy (&devices[i]->mac, &scan_res[i].bdaddr, sizeof(bdaddr_t));
    }

    free( scan_res );
    close( sock );
    return num_rsp;
}

int bro_bt_client_fist (bro_fist_t * input_fist, bro_fist_t * out_fist,
                        int spam_sock) {
    
    bro_spam_fists_t buffer;
    size_t res;
    
    buffer.size = sizeof(bro_fist_t) * BUFFER_SIZE;
    memcpy (buffer.packets, input_fist, ( sizeof(bro_fist_t) * BUFFER_SIZE));
                        
    res = send(spam_sock, &buffer, sizeof(bro_spam_fists_t), 0);
    if (res < 0) return res;
    res = recv(spam_sock, out_fist, ( sizeof(bro_fist_t) * BUFFER_SIZE ), 0);
    
    return res;
};

int bro_bt_close_connection (int spam_sock)
{
    return close(spam_sock);
}
