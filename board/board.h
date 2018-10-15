/**
 * @author Sargis S Yonan
 * @date 29 September 2018
 *
 * @brief Header file for a Mancala board.
 */

#pragma once

#include "hole.h"

#include <array>

#include <cstdbool>
#include <cstdint>

namespace Mancala
{
    /**
     * Specifies the possible sides of the board described in Figure 1.
     *
     * @note Strong casting using bool since there can only be two
     *       sides.
     */
    typedef enum : bool
    {
        A = true,
        B = false
    } Side;

    /**
     * A class for a Mancala Board.
     *
     * @tparam N the number of starting marbles in each hole
     *         board is set/reset.
     */
    template <uint8_t N = 4u>
    class Board
    {

        /*
                         _______
                        |       |
                        | ( B ) | <-- Home B
                        |       |
              Row 0 --> | O   O |
              Row 1 --> | O   O |
              Row 2 --> | O   O |
              Row 3 --> | O   O |
              Row 4 --> | O   O |
              Row 5 --> | O   O |
                        |       |
             Home A --> | ( A ) |
                        |_______|

        *       Figure 1. Mancala Board.
        */

        public:
            /**
             * The board constructor.
             */
            Board() : a_home(), b_home()
            {
            }

            /**
             * Pretty print the current state of the board.
             */
            void pretty_print() const
            {
                char space = '\0';
                char line = '\0';
                if (b_home.get() > 9 || a_home.get() > 9)
                {
                    space = ' ';
                    line = '_';
                }

                printf("            %c_______%c\n", line, line);
                printf("           |%c       %c|\n", space, space);
                printf("           |%c ( %u ) | <-- Home B\n", space, b_home.get());
                printf("           |%c       %c|\n", space, space);
                printf(" Row 0 --> |%c %u   %u %c|\n", space, holes[0][0].get(), holes[1][0].get(), space);
                printf(" Row 1 --> |%c %u   %u %c|\n", space, holes[0][1].get(), holes[1][1].get(), space);
                printf(" Row 2 --> |%c %u   %u %c|\n", space, holes[0][2].get(), holes[1][2].get(), space);
                printf(" Row 3 --> |%c %u   %u %c|\n", space, holes[0][3].get(), holes[1][3].get(), space);
                printf(" Row 4 --> |%c %u   %u %c|\n", space, holes[0][4].get(), holes[1][4].get(), space);
                printf(" Row 5 --> |%c %u   %u %c|\n", space, holes[0][5].get(), holes[1][5].get(), space);
                printf("           |%c       %c|\n", space, space);
                printf("Home A --> |%c ( %u ) | \n", space, a_home.get());
                printf("           |%c_______%c|\n\n", line, line);
            }

            /**
             * Resets the board.
             */
            void reset()
            {
                for (auto& row : holes)
                {
                    for (auto& hole : row)
                    {
                        hole.reset();
                    }
                }

                a_home.reset();
                b_home.reset();
            }

            /**
             * Getter for the number of marbles in the hole specified.
             *
             * @param side The side of the board to specify Side::A, or
             *        Side::B.
             * @param row The row on the board to select.
             *
             * @return The number of marbles to return.
             *
             */
            uint8_t get_hole(const Side side,
                const uint8_t row) const
            {
                switch(side)
                {
                    case Side::A:
                        if (row < 6)
                        {
                            return holes[0][row].get();
                        }
                        break;

                    case Side::B:
                        if (row < 6)
                        {
                            return holes[1][row].get();
                        }
                        break;

                    default:
                        break;
                }

                return 0;
            }

            /**
             * Clear a hole.
             *
             * @param side The side of the board to specify Side::A, or
             *        Side::B.
             * @param row The row on the board to select.
             */
            void clear_hole(const Side side, 
                const uint8_t row)
            {
                set_hole(side, row, 0u);
            }

            /**
             * Setter to set the number of marbles in the hole specified.
             *
             * @param side The side of the board to specify Side::A, or
             *        Side::B.
             * @param row The row on the board to select.
             * @param The number of marbles to set the hole with.
             */
            void set_hole(const Side side, 
                const uint8_t row, 
                const uint8_t n_marbles)
            {
                switch(side)
                {
                    case Side::A:
                        if (row < 6)
                        {
                            holes[0][row].set(n_marbles);
                        }
                        break;

                    case Side::B:
                        if (row < 6)
                        {
                            holes[1][row].set(n_marbles);
                        }
                        break;

                    default:
                        break;
                }
            }

            /**
             * Add marble(s) to a hole on the board.
             *
             * @param side The side of the board to specify Side::A, or
             *        Side::B.
             * @param row The row on the board to select.
             * @param n_marbles The number of marbles to place.
             */
            void add(const Side side, 
                const uint8_t row, 
                const uint8_t n_marbles = 1u)
            {
                switch(side)
                {
                    case Side::A:
                        holes[0][row].add(n_marbles);
                        break;

                    case Side::B:
                        holes[1][row].add(n_marbles);
                        break;

                    default:
                        break;
                }
            }

            /**
             * Add marble(s) to the home hole of the side specified.
             *
             * @param side The side of the board to specify Side::A, or
             *        Side::B.
             */
            void add_home(const Side side, 
                const uint8_t n_marbles = 1u)
            {
                switch(side)
                {
                    case Side::A:
                        a_home.add(n_marbles);
                        break;

                    case Side::B:
                        b_home.add(n_marbles);
                        break;

                    default:
                        break;
                }
            }

            /**
             * Get marble(s) count from a home spot.
             *
             * @param side The side of the board to specify Side::A, or
             *        Side::B.
             *
             * @return The home count.
             */
            uint8_t get_home(const Side side)
            {
                switch(side)
                {
                    case Side::A:
                        return a_home.get();
                        break;

                    case Side::B:
                        return b_home.get();
                        break;

                    default:
                        break;
                }
            }     

        private:

            /**
             * The internal data structure to keep track of the holes on the
             * board.
             *
             * @note The structure is a simple 2D array of holes.
             */
            Hole<N> holes[2][6];

            /**
             * The home holes for both sides.
             * @{
             */
            Hole<0u> a_home;
            Hole<0u> b_home;
            /**
             * @}
             */
    };
}