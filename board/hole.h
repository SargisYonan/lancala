/**
 * @author Sargis S Yonan
 * @date 29 September 2018
 *
 * @brief Header file for a Mancala board hole.
 */

#pragma once

#include <cstdint>

namespace Mancala
{
    /**
     * The hole error types.
     */
    typedef enum : int
    {
        success = 0,
        remove_error = -1
    } HoleError;

    /**
     * A class to describe a Mancala board hole.
     *
     * @tparam N the number of marbles to set/reset the hole with.
     */
    template <uint8_t N>
    class Hole
    {
        public:

            /**
             * The hole constructor.
             */
            Hole() : marbles(N)
            {
            }

            /**
             * Add a specified number of marbles to the hole.
             *
             * @param n_marbles The number of marbles to add.
             */
            void add(uint8_t n_marbles = 1u)
            {
                marbles += n_marbles;
            }

            /**
             * Remove one marble from the hole.
             *
             * @param n_marbles the number of marbles to remove.
             *
             * @return Success or remove_error on failure (insufficient number
             *         of marbles)
             */
            HoleError remove(uint8_t n_marbles = 1u)
            {
                if (marbles >= n_marbles)
                {
                    marbles -= n_marbles;

                    return HoleError::success;
                }
                else
                {
                    return HoleError::remove_error;
                }
            }

            /**
             * Getter for the number of marbles in the hole.
             *
             * @return The number of marbles in the hole.
             */
            uint8_t get() const
            {
                return marbles;
            }

            /**
             * Setter for the number of marbles in the hole.
             *
             * @return The number of marbles in the hole.
             */
            void set(uint8_t n_marbles)
            {
                marbles = n_marbles;
            }

            /**
             * Reset the number of marbles to the initial state.
             */
            void reset()
            {
                marbles = N;
            }

        private:

            /**
             * The counter for the number of marbles in the hole.
             */
            uint8_t marbles;
    };
}