/** @file bro_comm.h */
#ifndef __bro_headers_comm_h
#define __bro_headers_comm_h

#include "bro_fist.h"

/** @addtogroup BroComm  */
/*@{*/

/** Start the server by making it listen on a local socket for a communication by the client.
 *  @param server_sock Server's socket descriptor.
 *                  This value is used only at the end (please refer to
 *                  bro_stop_server()) to close the socket.
 *  @param client_sock Client's socket (Scicos or another application) descriptor. 
 *  @return Result of the operation
 *  @retval 0 if the connection was succesful.
 *  @retval -1 if there were problems in the connection.
 *  @note If there were errors with the connection then @c errno will be
 *        set accordingly to the return values listed on the @c "man 2
 *        connect" page.   
 */
int bro_start_server (int * server_sock, int * client_sock);

/** bro_server_fist is the function that receives the Operational Fists from the
 *  @param input_fist* Array containing the operations to send to the SPAM.
 *  @param output_fist*  Array containing the responses for the sent operations.
 *  @param scicos_sock Socket Descriptor for the client connection.
 *                     Normally is the @c client_sock returned by
 *                     bro_start_server().
 *  @param spam_sock Socket Descriptor for the SPAM connection.
 *                   Normally is the @c spam_sock returned by the
 *                   bro_bt_connect_device().
 *  @return Result of the operation
 *  @retval 0 if the connection was succesful.
 *  @retval -1 if there were problems in the connection.
 *  @note If there were errors with the connection then @c errno will be
 *        set accordingly to the return values listed on the @c "man 2
 *        connect" page.
 */
int bro_server_fist (const bro_fist_t * input_fist, bro_fist_t * out_fist, int scicos_sock, int spam_sock);

/** Stop the server, normally called on finalization. 
 *  @param server_sock The server socket descriptor.
 *  @param client_sock The client socket descriptor.
 *  @return Result of the operation
 *  @retval 0 if the connection was succesful.
 *  @retval -1 if there were problems in the connection.
 *  @note If there were errors with the connection then @c errno will be
 *        set accordingly to the return values listed on the @c "man 2
 *        connect" page.
 */
int bro_stop_server (int server_sock, int client_sock);

/*@}*/

#endif 
