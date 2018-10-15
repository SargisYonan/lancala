/**
 * @author Sargis S Yonan
 * @date 2 October 2018
 *
 * @brief The game logic for mancala.
 */

#pragma once

#include "board.h"

#include <cstdbool>
#include <cstdint>
#include <cstdlib>

namespace Mancala
{
    typedef enum : int8_t
    {
        SideA = 1,
        SideB = 2,
        GameOver = 3,

        RoundFailure = -1,
        InvalidSide = -2,
        EmptyHoleError = -3
    } GameState;

    class Game
    {

    public:
        /**
         * Game constructor.
         *
         * @param _board The board constructed for this game.
         */
        Game(Board<> &_board);

        /**
         * Run a round of the game.
         *
         * @param current_player_side The current player's side.
         * @param row The row to select from.
         *
         * @return The error code for the round played.
         */
        GameState run_round(Side current_player_side, uint8_t row);

        /**
         * Get number of rounds played.
         *
         * @return The number of played rounds.
         */
        uint16_t get_rounds() const;

        /**
         * Return the side of the last winner.
         *
         * @return The last winner.
         */
        Side get_winner() const;

        /**
         * Reset the game.
         */
        void reset();

    private:
        /**
         * Round counter.
         */
        uint16_t rounds;

        /**
         * Winner state.
         */
        Side winner;

        /**
         * Last error state.
         */
        GameState error_code;

        /**
         * The board instance to play on.
         */
        Board<> &board;
    };
}