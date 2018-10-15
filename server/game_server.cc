/**
 * @author Sargis Yonan
 * @date 2 October 2018
 *
 * @brief Game server logic for Mancala.
 */

#include "game_server.h"

extern "C"
{
    #include "client.h"
    #include "server.h"
}

namespace Mancala
{
    GameServer::GameServer(char *hostname, Side side, uint16_t server_port)
    {
        int error_code = 0;

        printf("Waiting for opponent to connect...\n");
        
        if (side == Side::B)
        {
            error_code = start_server(server_port);
            if (error_code < 0)
            {
                printf("Error: server cannot open port %d with hostname %s. Code: %d\n", 
                    server_port, hostname, error_code);
            }

            if (start_client(server_port + 1, hostname) > 0)
            {
                while(client_reconnect() < 0);
            }
        }
        else
        {
            if (start_client(server_port, hostname) > 0)
            {
                while(client_reconnect() < 0);            
            }    

            error_code = start_server(server_port + 1);
            if (error_code < 0)
            {
                printf("Error: server cannot open port %d with hostname %s. Code: %d\n", 
                    server_port + 1, hostname, error_code);
            }
        }
    
        memset(receiver, '\0', 4);
    }

    bool GameServer::move_received()
    {
        if (get_server_packet(receiver, 4) >= 0)
        {
            return true;
        }
        else
        {
            printf("Error: server cannot receive.\n");
            return false;
        }
    }

    void GameServer::get_move(uint16_t &round_number, Side &side, uint8_t &row)
    {
        round_number = 0x0000;
        round_number = static_cast<uint16_t>(receiver[0] << 8 || receiver[1]);

        side = static_cast<Side>(receiver[2]);

        row = static_cast<uint8_t>(receiver[3]);
    }

    bool GameServer::send_move(uint16_t round_number, Side side, uint8_t row)
    {
        char send_buffer[4] = {0};

        send_buffer[0] = static_cast<char>(round_number >> 8);
        send_buffer[1] = static_cast<char>(round_number & 0x00FF);

        send_buffer[2] = static_cast<char>(side);

        send_buffer[3] = static_cast<char>(row);

        if (send_packet(send_buffer, 4) < 0)
        {
            printf("Error: cannot send move.\n");
            return false;
        }
        else
        {
            return true;
        }
    }
}