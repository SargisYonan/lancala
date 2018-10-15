/**
 * @author Sargis Yonan
 * @date 2 October 2018
 *
 * @brief Game server logic for Mancala.
 */

#pragma once

#include "board.h"

#include <cstdbool>
#include <cstdint>

namespace Mancala
{
    class GameServer
    {

    public:
        /**
         * Constructor.
         *
         * @note Starts a server on the game port.
         *
         * @param hostname The opponent's hostname.
         * @param server_port The port to play on.
         * @param side This players side.
         */
        GameServer(char *hostname, Side side, uint16_t server_port = 6969);
        
        /**
         * Check to see if a move has been received.
         *
         * @note If a packet is received, the buffer is overwritten.
         *
         * @return True if a move had been received,
         *         False if not.
         */
        bool move_received();

        /**
         * Receive a move from the opponent.
         *
         * @param[out] round_number The round number received.
         * @param[out] side The side the move took place on.
         * @parama[out] row The row the move started from.
         */
        void get_move(uint16_t &round_number, Side &side, uint8_t &row);

        /**
         * Send a move.
         *
         * @param round_number The round number to send.
         * @param side The side the move happend on.
         * @param row The row to start the move on.
         *
         * @return True if move sent successfully.
         */
        bool send_move(uint16_t round_number, Side side, uint8_t row);

    private:
        /**
         * The receive buffer for the server.
         *
         * @note Packet structure:
         *       Round Number -- 2B
         *       Side         -- 1B
         *       Row          -- 1B
         */
        char receiver[4];
    };
}