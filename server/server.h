/**
 * @author Sargis Yonan
 * @date 2 October 2018
 *
 * @brief A simple server in the internet domain using TCP
 *        The port number is passed as an argument 
 */

#ifndef _SERVER_H_
#define _SERVER_H_

/**
 * Starts the server.
 *
 * @param port_number The port number of the socket.
 *
 * @return Server start error code.
 */
int start_server(const unsigned int port_number);

/**
 * Get a packet from the client.
 *
 * @param[out] receiver The buffer to write data back into.
 * @param packet_length The maximum number of bytes to read into
 *        the receriver buffer.
 *
 * @return Positive number of bytes copied into receiver. Negative
 *         if no copy occured.
 */
int get_server_packet(char *receiver, unsigned int packet_length);

/**
 * Stops the server.
 */
void stop_server(void);

#endif // _SERVER_H_
