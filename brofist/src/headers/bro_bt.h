/** @file bro_bt.h  */
#ifndef __bro_headers_bt_h
#define __bro_headers_bt_h

#include <stdint.h>
#include <sys/socket.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include "bro_fist.h"

/**@addtogroup BroBT */
/*@{*/

/** Maximum nuber of BlueTooth Devices discoverable. */
#define MAX_BT_DEVICES 255
/** Lenght of the BlueTooth Inquiry.
 *
 *  This time is not in seconds but in 1.28 seconds. Beats me why they have
 *  chosen to this, however it is discussed on the BlueZ tutorial here:
 *  http://people.csail.mit.edu/albert/bluez-intro/c404.html 
 */
#define BT_INQUIRY_LEN 8

/** Structure for representing a BlueTooth Device.
 *
 *  It contains the data needed for connection and for the interaction with
 *  the user.
 */
typedef struct {
    /** Human Readable Name (As set on the Device). */
    char       name[248];      
    /** Mac address. 
     *
     *  The MAC address here is not stored as a Human Readable String, but
     *  as a @c bdaddr_t. This type is declared inside the BlueTooth
     *  Library (@c BlueZ or @c Libbluetooth) as an array of 6 @c uint8_t (Since
     *  a MAC address is a 6 integer tuple, all of which can have a maximum
     *  value of 0xFF or 255). This kind of variable can be transformed in
     *  a Human Readable String with the use of ba2str(const bdaddr_t*,
     *  char*) function, part of the Libbluetooth.
     */
    bdaddr_t   mac;             
} bro_bt_device_t;

/** Discover all the BT Devices in range and then put their details in an array.
 *
 *  Bla.
 *
 *  @param devices[MAX_BT_DEVICES] Array that will contains al the Devices Found.
 *  @return Number of Devices found.
 *  @retval -1 if any connection error emerged.
 */
size_t bro_bt_scan_devices (bro_bt_device_t * devices[MAX_BT_DEVICES]);

/** Connect to the given device.
 *
 *  The connection is done using a @c bdaddr_t parameter since it is
 *  possible to connect to a BlueTooth device knowing only the MAC address,
 *  since one of the connecting options for the BROFist server is to pass
 *  directly the MAC address at the startup.
 *
 *  @param spam_sock Pointer to the SPAM Socket Descriptor.
 *  @param mac_addr MAC Address of the Device to connect to.
 *  @return Result of the operation
 *  @retval 0 if the connection was succesful.
 *  @retval -1 if there were problems in the connection.
 *  @note If there were errors with the connection then @c errno will be
 *        set accordingly to the return values listed on the @c "man 2
 *        connect" page.
 */
int bro_bt_connect_device (int * spam_sock, bdaddr_t mac_addr);

/** Send a BROFist to the SPAM and receive the response.
 *
 *  @param input_fist The operations to send to the SPAM.
 *  @param out_fist The response received from the SPAM.
 *  @param spam_sock The SPAM Socket Descriptor.
 *  @return Bytes received.
 *  @retval -1 if there was any problem.
 */
int bro_bt_client_fist (bro_fist_t * input_fist, bro_fist_t * out_fist,
                        int spam_sock);   

/** Close the connection to the SPAM unit.
 *
 *  @param spam_sock The SPAM Socket Descriptor.
 *  @return Result of the operation
 *  @retval 0 if the closing was succesful.
 *  @retval -1 if there was any problem.
 *  
 */
int bro_bt_close_connection (int spam_sock);

/*@}*/

#endif 
