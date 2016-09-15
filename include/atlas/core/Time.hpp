/**
 *	\file Time.hpp
 *	\brief Defines the timing storage system.
 */

#ifndef ATLAS_INCLUDE_ATLAS_CORE_TIME_HPP
#define ATLAS_INCLUDE_ATLAS_CORE_TIME_HPP

#pragma once

#include "Core.hpp"

namespace atlas
{
    namespace core
    {
        /**
         *	\class Time
         *	\brief Stores all timing data required for animation driven Scenes.
         *	
         *	This class holds the following information:
         *	\li The total time the scene has been running,
         *	\li the time delta between the last tick and the current tick,
         *	\li and the current tick.
         *	
         *	This should provide all of the necessary information for most
         *	animation algorithms.
         *	
         *	\tparam GenType The precision for the time units.
         *	
         */
        template <typename GenType>
        class Time
        {
        public:
            /**
             * Standard constructor. Initializes everything to 0.
             */
            Time() :
                totalTime(GenType(0.0)),
                deltaTime(GenType(0.0)),
                currentTime(GenType(0.0))
            { }

            /**
             * \var totalTime
             * The total time that the containg object (generally a Scene) has
             * been running.
             */
            GenType totalTime;

            /**
             *	\var deltaTime
             *	The difference between the current tick and the last one.
             */
            GenType deltaTime;

            /**
             *	\var currentTime
             *	The current tick.
             */
            GenType currentTime;
        };
    }
}

#endif