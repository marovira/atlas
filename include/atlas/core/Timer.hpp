/**
 *	\file Timer.hpp
 *	\brief Defines the Timer class for timing events.
 */

#ifndef ATLAS_INCLUDE_ALTAS_CORE_TIMER_HPP
#define ATLAS_INCLUDE_ALTAS_CORE_TIMER_HPP

#pragma once

#include "Core.hpp"

#include <iostream>
#include <chrono>


namespace atlas
{
    namespace core
    {
        /**
         * \class Timer
         * \brief Defines a simple system for timing events.
         * 
         * This class employs the <tt> \<chrono\> </tt> library to compute
         * the time that passes from the time it is created (or reset), to the
         * time the elapsed function is called.
         * 
         * \tparam The precision of the timer.
         */
        template <class GenType>
        class Timer
        {
        public:
            /**
             * Constructs the timer and initializes it to 0.
             */
            Timer()
            { }

            /**
             * Sets the timer to the current time. The elapsed time can be
             * retrieved by calling the ellapsed() function.
             */
            inline void start()
            {
                mBegin = Clock::now();
            }

            /**
             * Resets the timer back to 0.
             */
            inline void reset()
            {
                mBegin = std::chrono::time_point<Clock>();
            }

            /**
             * Returns the amount of time. The time returned is determined
             * by the resolution that was given when the Timer was created.
             */
            GenType elapsed() const
            {
                return std::chrono::duration_cast<Second>(
                        Clock::now() - mBegin).count();
            }

        private:
            typedef std::chrono::high_resolution_clock Clock;
            typedef std::chrono::duration<GenType> Second;
            std::chrono::time_point<Clock> mBegin;
        };
    }
}

#endif