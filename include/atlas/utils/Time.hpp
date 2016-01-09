#ifndef ATLAS_INCLUDE_ATLAS_UTILS_TIME_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_TIME_HPP

#pragma once

#include "Utils.hpp"

namespace atlas
{
    namespace utils
    {
        /**
         *	\class Time
         *	\brief Stores all timing data required for animation driven Scenes.
         *	
         *	This struct holds the following information:
         *	\li The total time the scene has been running,
         *	\li the time delta between the last tick and the current tick,
         *	\li and the current tick.
         *	
         *	This should provide all of the necessary information for most
         *	animation algorithms.
         *	
         *	\note
         *	The time units used in Atlas (and the ones in GLFW) are seconds.
         */
        struct Time
        {
            /**
             * Standard constructor. Initializes everything to 0.
             */
            Time() :
                totalTime(0.0f),
                deltaTime(0.0f),
                currentTime(0.0f)
            { }

            /**
             * Defualt copy constructor.
             */
            Time(Time const& t) = default;

            /**
             * Default destructor.
            */
            ~Time()
            { }

            /**
             * \var totalTime
             * The total time that the containg object (generally a Scene) has
             * been running.
             */
            float totalTime;

            /**
             *	\var deltaTime
             *	The difference between the current tick and the last one.
             */
            float deltaTime;

            /**
             *	\var currentTime
             *	The current tick.
             */
            float currentTime;
        };
    }
}

#endif