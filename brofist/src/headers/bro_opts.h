/** @file bro_opts.h */
#ifndef __bro_headers_options_h
#define __bro_headers_options_h

#include "bro_bt.h"
/** @addtogroup BROOpts */
/* @{ */

/** Struct that contains all the data parsed from the options.
 *  I know it might be skipped but it could be used for future
 *  improvements. It's easier to update a code without having to rewrite
 *  every function descriptor.
 *  bdaddr_t mac:    */
typedef struct 
{
/** MAC address chosen for the SPAM communication.
 *
 *  This is the 6-bytes encoding variable type defined by the Libbluetooth.
 */
  bdaddr_t mac;
} bro_opts_t;

/** Function that uses getopt to parse every option passed to the BROFist
 *  Server.
 *  
 *  It should be called before any initialisation function, to get the MAC
 *  addres for the SPAM or any other data.
 *  @param so  The struct that will contain every parsed option.
 *  @param argc Sounds familiar, doesn't it?
 *              It's the argument count passed to the main(). It is
 *              needed, alongside argv, by getopt.
 *  @param argv[] Another familiar name: it's the array of strings passed
 *                to the applications as arguments. Another thing required
 *                by getopt.
 *  @return Result of the operation.
 *  @retval 0 if everything went as espected and the operation did not need
 *            to start the server.
 *  @retval -1 if there was an error or we called either the help (@c -h)
 *             or the BlueTooth Discovery (@c -l).
 */
int bro_opts_parse (bro_opts_t *so, int argc, char * const argv[]);

/* @} */

#endif 
