/**
 * @author Sargis Yonan
 * @date 2 October 2018
 *
 * @brief A simple server in the internet domain using TCP
 *        The port number is passed as an argument 
 */

#include "server.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define BACKLOG_SIZE 5

/**
 * The socket file descriptor for the backlog.
 */
static int sockfd = 0;

/**
 * The socket file descriptor for data/acks.
 */
static int newsockfd = 0;

/**
 * The client socket length.
 */
static socklen_t client_length;

/**
 * The server's address.
 */
static struct sockaddr_in server_address;

/**
 * The address of the client.
 */
static struct sockaddr_in client_address;

/**
 * Starts the server.
 *
 * @param port_number The port number of the socket.
 *
 * @return Server start error code.
 */
int start_server(const unsigned int port_number)
{
    /*
     * Socket already open.
     */
    if (sockfd > 0 || newsockfd > 0)
    {
        /*
         * Socket(s) already open.
         */
        return -100;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        return -1;
    }

    bzero((char *) &server_address, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port_number);

    if (bind(sockfd, (struct sockaddr *) &server_address,
        sizeof(server_address)) < 0) 
    {
        return -1;
    }

    listen(sockfd, BACKLOG_SIZE);

    client_length = sizeof(client_address);

    newsockfd = accept(sockfd, 
             (struct sockaddr *) &client_address, 
             &client_length);

    if (newsockfd < 0) 
    {
        return -2;
    }

    return 1;
}

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
int get_server_packet(char *receiver, unsigned int packet_length)
{
    int n = 1;

    memset(receiver, 0, packet_length);

    /*
     * Read data.
     */
    n = read(newsockfd, receiver, packet_length);
    if (n <= 0)
    {
        return -3;
    }

    /*
     * Write an ack.
     */
    n = write(newsockfd, "ack", 3);
    if (n <= 0)
    {
        return -4;
    }

    return n; 
}

/**
 * Stops the server.
 */
void stop_server(void)
{
    /*
     * Close the sockets.
     */
    close(newsockfd);
    close(sockfd);

    sockfd = 0;
    newsockfd = 0; 
}
