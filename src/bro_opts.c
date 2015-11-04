#include "headers/bro_opts.h"

#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

/*  This is bad. As far as I know these are 4 global library variables needed
 *  by getopt. We will leave these here.
 */
extern char *optarg;
extern int optind, opterr, optopt;

/*  This variable contains every option we might pass via command line and also 
 *  if that option should have an argument or not (The ":" means that the
 *  precedent option needs an argument).
 */
static const char optstring[] = "m:hls:";

/*  This is almost the same as optstring[], but it contains the extended options
 *  (The ones that can be called using "--" instead of "-") and what they mean.
 */
static const struct option longopts[] = {
    {"mac", 1, NULL, 'm'},            // MAC Address
    {"list-devices", 0, NULL, 'l'},   // BT Scan only? 
    {"select-device", 1, NULL, 's'},  // Select device using its name
    {"help", 0, NULL, 'h'},           // Display help
    {NULL, 0, NULL, 0}
};

// The help description.
static const char help [] =
"\n" "BROFist Server" "\n"
"Usage: %s [options]\n\n"
"Available options\n\n"
"  --mac={mac_address} | -m {mac_address}\n"
"        Specify MAC address of the NXT Brick to connect to\n\n"
"  --list-devices | -l\n"
"        Print a List of BT Devices in range\n\n"
"  --select-device={BlueTooth Name} | -s {BlueTooth Name}\n"
"        Connect to the BlueTooth device that has the given name\n\n"
"  --help  | -h\n"
"        Print this help\n";

/*  This function checks if a MAC address passed via command line is correct
 *  and then converts it to a bdaddr_t for future uses. (Like the
 *  bro_bt_connect_device)
 */
static
int chk_mac_addr (char * target, bdaddr_t * mac) {
    
    unsigned b0, b1, b2, b3, b4, b5;
    char buffer[18];
    
    if (sscanf(target, "%2x:%2x:%2x:%2x:%2x:%2x",
               &b0, &b1, &b2, &b3, &b4, &b5) == 6) {
        sprintf(buffer, "%02x:%02x:%02x:%02x:%02x:%02x",
                b0, b1, b2, b3, b4, b5);
        return str2ba(buffer, mac);
    } else {
        return -1;
    }
};

static inline
void print_help (const char *progname)
{
    fprintf(stderr, help, progname);
}

static 
int check_optarg (const char *arg, bro_bt_device_t * target[])
{
    int id = 0;

    while (target[id]->name) {
        if (strcmp(target[id]->name, arg) == 0) {
            return id;
        }
        id ++;
    }
    return -1;
}

int bro_opts_parse (bro_opts_t *so, int argc, char * const argv[])
{
    int opt;
    bro_bt_device_t * devices[MAX_BT_DEVICES];
    size_t ndevs;
    int dev_id;
    int i;
    char conv_mac[18];
    
    while ((opt = getopt_long(argc, argv, optstring, longopts, NULL)) != -1)
    {
        switch (opt) {
            case 'm':
                if (chk_mac_addr(optarg, &so->mac) == -1){
                    return -1;
                }
                break;
            case 's':
                ndevs = bro_bt_scan_devices(devices);
                dev_id = check_optarg(optarg, devices);
                if (dev_id < 0) {
                    fprintf(stderr, "Il device %s non esiste o non è in\
                            range\n", optarg);
                    return -1;
                }
                so->mac = devices[dev_id]->mac;
                break;
            case 'l':
                ndevs = bro_bt_scan_devices(devices);
                fprintf(stderr, "Bluetooth Devices in Range:\n\n");
                for (i = 0; i < ndevs; i++) {
                    ba2str(&devices[i]->mac, conv_mac);
                    fprintf(stderr, "%d: %s [%s]\n", i+1, devices[i]->name,
                            conv_mac);
                };
                return -1;
            case 'h':
            case '?':
                print_help(argv[0]);
                return -1;
        }
    }
    
    return 0;
}
