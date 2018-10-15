/**
 * @author Sargis S Yonan
 * @date 29 September 2018
 *
 * @brief Client for mancala server
 */

#ifndef _CLIENT_H_
#define _CLIENT_H_

/**
 * Start the client.
 *
 * @note hostname Must be NULL terminated!
 *
 * @param port_number The port to open.
 * @param[in] hostname The hostname of IP of the
 *            other machine.
 *
 * @return Socket error code. Success if positive.
 */
int start_client(const int port_number, 
    const char *hostname);

/**
 * Try reconnecting if the start call failed.
 *
 * @return postive on success.
 */
int client_reconnect(void);

/**
 * Send a packet to a server.
 *
 * @param[in] packet The packet to send.
 * @param packet_length The number of bytes to send.
 *
 * @return The socket error code.
 */
int send_packet(const char *packet, 
    const unsigned int packet_length);

/**
 * Close the client connection.
 */
void close_client(void);

#endif // _CLIENT_H_
