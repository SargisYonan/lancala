/**
 * @author Sargis S Yonan
 * @date 2 October 2018
 *
 * @brief The game logic for mancala.
 */

#include "game.h"

#include <cstdbool>
#include <cstdint>
#include <cstdlib>

namespace Mancala
{
    Game::Game(Board<> &_board) : 
        rounds(0), 
        winner(Side::A), 
        error_code(GameState::RoundFailure), 
        board(_board)
    {
    }

    GameState Game::run_round(Side current_player_side, uint8_t row)
    {
        bool ended_in_home = false;
        uint8_t wrap_arounds = 0;

        /*
         * No repeats.
         */
        if ((error_code == GameState::SideA && current_player_side == Side::B) ||
            (error_code == GameState::SideB && current_player_side == Side::A))
        {

            return error_code;
        }

        if (board.get_hole(current_player_side, row) == 0)
        {
            error_code = GameState::EmptyHoleError;
            return GameState::EmptyHoleError;
        }

        uint8_t marbles_collected = board.get_hole(
            current_player_side, 
            row);

        board.clear_hole(current_player_side, row);

        Side other_player;

        /*
         * Going to be using the sign bit.
         */
        int8_t row_select = static_cast<int8_t>(row);

        switch(current_player_side)
        {
            case Side::A:
                other_player = Side::B;

                row_select++;

                for (uint8_t marbles = marbles_collected;
                    marbles > 0;
                    marbles--)
                {
                    /*
                     * On your side.
                     */
                    if (wrap_arounds % 2 == 0)
                    {
                        if (row_select >= 6)
                        {
                            board.add_home(current_player_side);
                            row_select = 5;
                            wrap_arounds++;

                            if (marbles == 1)
                            {
                                ended_in_home = true;                                
                            }
                        }
                        else
                        {
                            if (marbles == 1 && 
                                board.get_hole(current_player_side, row_select) == 0)
                            {
                                uint8_t marbles_stolen = 
                                board.get_hole(other_player, row_select);

                                board.clear_hole(other_player, row_select);

                                board.add_home(current_player_side, marbles_stolen + 1);
                            }
                            else
                            {
                                board.add(current_player_side, row_select++);
                            }
                        }    
                    }
                    /*
                     * On the other side.
                     */
                    else
                    {
                        board.add(other_player, row_select--);
                        if (row_select == -1 && marbles > 0)
                        {
                            row_select = 0;
                            wrap_arounds++;
                        } 
                    }
                }

                if (ended_in_home)
                {
                    error_code = GameState::SideA;
                }
                else
                {
                    error_code = GameState::SideB;
                }

                break;

            case Side::B:
                other_player = Side::A;
                
                row_select--;

                for (uint8_t marbles = marbles_collected;
                    marbles > 0;
                    marbles--)
                {
                    /*
                     * On your side.
                     */
                    if (wrap_arounds % 2 == 0)
                    {
                        if (row_select < 0)
                        {
                            board.add_home(current_player_side);
                            row_select = 0;
                            wrap_arounds++;

                            if (marbles == 1)
                            {
                                ended_in_home = true;
                            }
                        }
                        else
                        {
                            if (marbles == 1 && 
                                board.get_hole(current_player_side, row_select) == 0)
                            {
                                uint8_t marbles_stolen = 
                                board.get_hole(other_player, row_select);

                                board.clear_hole(other_player, row_select);

                                board.add_home(current_player_side, marbles_stolen + 1);
                            }
                            else
                            {
                                board.add(current_player_side, row_select--);                                
                            }
                        }    
                    }
                    /*
                     * On the other side.
                     */
                    else
                    {
                        board.add(other_player, row_select++);

                        if (row_select == 6 && marbles > 0)
                        {
                            row_select = 5;
                            wrap_arounds++;
                        } 
                    }
                }

                if (ended_in_home)
                {
                    error_code = GameState::SideB;
                }
                else
                {
                    error_code = GameState::SideA;
                }
                
                break;

            default:
                error_code = GameState::InvalidSide;

                return GameState::InvalidSide;
                break;
        }

        Side empty_side;
        Side non_empty_side;

        for (uint8_t row = 0; row < 6; row++)
        {
            if (board.get_hole(Mancala::Side::A, row) != 0)
            {
                break;
            }

            if (row == 5)
            {
                error_code = GameState::GameOver;
                empty_side = Side::A;
                non_empty_side = Side::B;
            }
        }

        for (uint8_t row = 0; row < 6; row++)
        {
            if (board.get_hole(Side::B, row) != 0)
            {
                break;
            }

            if (row == 5)
            {
                error_code = GameState::GameOver;

                empty_side = Side::B;
                non_empty_side = Side::A;
            }
        }

        if (error_code == GameState::GameOver)
        {
            uint8_t surplus_marbles = 0;

            for (uint8_t row = 0; row < 6; row++)
            {
                surplus_marbles += board.get_hole(non_empty_side, row);
                board.clear_hole(non_empty_side, row);
            }

            board.add_home(non_empty_side, surplus_marbles);

            winner = board.get_home(Side::A) > board.get_home(Side::B) ? Side::A : Side::B;
        }

        rounds++;
        
        return error_code;
    }

    uint16_t Game::get_rounds() const
    {
        return rounds;
    }

    Side Game::get_winner() const
    {
        return winner;
    }

    /**
     * Reset the game.
     */
    void Game::reset()
    {
        board.reset();
        rounds = 0;
        winner = Side::A;
    }
}



    