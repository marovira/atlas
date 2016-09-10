/**
 *	\file FPSCounter.hpp
 *	\brief Defines an fps counter for animations.
 */

#ifndef ATLAS_INCLUDE_ATLAS_UTILS_FPS_COUNTER_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_FPS_COUNTER_HPP

#pragma once

#include "Utils.hpp"
#include "atlas/core/Core.hpp"

#include <memory>

namespace atlas
{
    namespace utils
    {
        /**
         * \class FPSCounter
         * \brief Provides a way of setting the fps for a scene.
         *  
         * This class provides an easy way of handling timing events for both
         * animations as well as rendering. It can be used to control when
         * geometries are updated/rendered by simply adding an if statement
         * inside the appropriate function in the scene class.
         */
        class FPSCounter
        {
        public:
            /**
             * Standard empty constructor.
             */
            FPSCounter();

            /**
             * Initializes the counter to the desired FPS.
             *
             * \param[in] fps The frames per second the counter will keep
             * track of.
             */
            FPSCounter(float fps);

            /**
             * Standard copy constructor.
             */
            FPSCounter(FPSCounter const& t);

            /**
             * Standard destructor.
             */
            ~FPSCounter();

            /**
             * Sets the FPS for the counter.
             *
             * \param[in] fps The frames per second the counter will keep
             * track of.
             */
            void setFPS(float fps);

            /**
             * Checks whether the required time has elapsed between 
             * frames.
             * 
             * \return True if the next frame should be executed, 
             * false otherwise.
             */
            bool isFPS(atlas::core::Time<>& t) const;

        private:
            struct FPSCounterImpl;
            std::unique_ptr<FPSCounterImpl> mImpl;
        };
    }
}

#endif
