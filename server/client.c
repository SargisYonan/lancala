/**
 * @author Sargis S Yonan
 * @date 29 September 2018
 *
 * @brief Client for mancala server
 */

#include "client.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

static int sockfd = 0;
static struct sockaddr_in server_address;

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
    const char *hostname)
{
    struct hostent *server = NULL;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        return -1;
    }

    server = gethostbyname(hostname);
    if (!server) 
    {
        return -2;
    }

    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;

    bcopy((char *)server->h_addr, 
          (char *)&server_address.sin_addr.s_addr,
          server->h_length);

    server_address.sin_port = htons(port_number);

    return 1;
}

int client_reconnect(void)
{
    if (connect(sockfd,
           (struct sockaddr *) &server_address,
           sizeof(server_address)) < 0) 
    { 
        return -3;
    }
    else
    {
        return 1;
    }
}

/**
 * Send a packet to a server.
 *
 * @param[in] packet The packet to send.
 * @param packet_length The number of bytes to send.
 *
 * @return The socket error code.
 */
int send_packet(const char *packet, const unsigned int packet_length) 
{
   int n = 0;
   char response[4] = {0};

    /*
     * Check if the socket is defined.
     */
    if (sockfd == 0)
    {
        return -100;
    }

    n = write(sockfd, packet, packet_length);
    if (n < 0)
    {
        return -4;
    }

    n = read(sockfd, response, 4);
    if (n < 0)
    {
        return -5;
    }

    return n;
}

/**
 * Close the client connection.
 */
void close_client(void)
{
    close(sockfd);
    sockfd = 0;
}
