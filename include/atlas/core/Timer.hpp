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
         * @tparam The precision of the timer.
         */
        template <class GenType>
        class Timer
        {
        public:
            /**
             * Constructs and initializes the timer to the current time.
             */
            Timer() :
                mBegin(Clock::now())
            { }

            /**
             * Resets the timer to the time whenever this function is called.
             */
            inline void reset()
            {
                mBegin = Clock::now();
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
            typedef std::chrono::duration<GenType, std::ratio<1>> Second;
            std::chrono::time_point<Clock> mBegin;
        };
    }
}

#endif