/**
 * @author Sargis S Yonan
 * @date 29 September 2018
 * 
 * @brief The main driver file for Mancala.
 */

#include "board.h"
#include "game.h"
#include "game_server.h"

#include <iostream>

#include <cstdbool>
#include <cstdint>
#include <cstdlib>

 int main(void)
 {
    char opponent_hostname[64] = {0};
    char side = '\0';

    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~\n";
    std::cout << " ~o~o~o Mancala o~o~o~\n";
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~\n\n";

    std::cout << "Enter opponent's hostname or ip address: ";
    std::cin >> opponent_hostname;

    Mancala::Board<4u> board;

    board.pretty_print();

    std::cout << "Enter your side (A | B): ";
    std::cin >> side;

    Mancala::Side current_player_side;
    Mancala::Side opponent_side;

    if (side == 'A' || side == 'a')
    {
        current_player_side = Mancala::Side::A; 
    }
    else if (side == 'B' || side == 'b')
    {
        current_player_side = Mancala::Side::B; 
    }
    else
    {
        std::cout << "Error: invalid side!\n";
        return 1;
    }

    Mancala::Game game(board);
    Mancala::GameServer server(opponent_hostname, current_player_side);

    uint16_t round = 0;

    srand(time(NULL));

    Mancala::GameState error_code = Mancala::GameState::SideA;

    while(true)
    {
        printf("\n________________________________\n\n");
        printf("Round: %u\n", game.get_rounds());
        board.pretty_print();

        int row_input = 0;
        uint8_t row = 0;

        switch (error_code)
        {
            case Mancala::GameState::SideA:
                if (current_player_side == Mancala::Side::A)
                {
                    printf("Your turn!\nSelect a row: ");

                    std::cin >> row_input;
                    row = static_cast<uint8_t>(row_input);

                    error_code = game.run_round(current_player_side, row);

                    if (static_cast<int8_t>(error_code) > 0)
                    {
                        server.send_move(game.get_rounds(), current_player_side, row);
                    }
                    else
                    {
                        printf("Something went wrong.\n");
                    }
                }
                else
                {
                    printf("Waiting for opponent to move...\n");
                    while(!server.move_received());

                    server.get_move(round, opponent_side, row);

                    if (row < 6)
                    {
                        error_code = game.run_round(opponent_side, row);
                    }
                    else
                    {
                        printf("Error: received move with incorrect row number.");
                    }

                    error_code = game.run_round(opponent_side, row);
                }

                break;

            case Mancala::GameState::SideB:
                if (current_player_side == Mancala::Side::B)
                {
                    printf("Your turn!\nSelect a row: ");

                    std::cin >> row_input;
                    row = static_cast<uint8_t>(row_input);

                    error_code = game.run_round(current_player_side, row);

                    if (static_cast<int8_t>(error_code) > 0)
                    {
                        server.send_move(game.get_rounds(), current_player_side, row);
                    }
                    else
                    {
                        printf("Error: code %x\n", error_code);
                    }
                }
                else
                {
                    printf("Waiting for opponent to move...\n");
                    while(!server.move_received());

                    server.get_move(round, opponent_side, row);

                    if (row < 6)
                    {
                        error_code = game.run_round(opponent_side, row);
                    }
                    else
                    {
                        printf("Error: received move with incorrect row number.");
                    }
                }

                break;

            case Mancala::GameState::GameOver:
                printf("\nGame Over.\n\n");

                board.pretty_print();

                if (game.get_winner() == current_player_side)
                {
                    printf("You win!\n");
                }
                else
                {
                    printf("You lose.\n");
                }

                return 0;
                break;

            case Mancala::GameState::RoundFailure:
                printf("Error: round failure.\n");
                return 1;
                break;

            case Mancala::GameState::InvalidSide:
                printf("Error: invalid side.\n");
                return 1;
                break;

            case Mancala::GameState::EmptyHoleError:
                /*
                 * Reselect hole.
                 */
                break;

            default:
                break;
        }
    }
    return 0;
 }