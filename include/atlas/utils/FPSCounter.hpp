#ifndef ATLAS_INCLUDE_ATLAS_UTILS_FPS_COUNTER_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_FPS_COUNTER_HPP

#pragma once

#include "Utils.hpp"
#include "Time.hpp"

#include <memory>

namespace atlas
{
    namespace utils
    {
        class FPSCounter
        {
        public:
            FPSCounter();
            FPSCounter(float fps);
            FPSCounter(FPSCounter const& t);
            ~FPSCounter();

            void setFPS(float fps);

            bool isFPS(Time& t) const;

        private:
            struct FPSCounterImpl;
            std::unique_ptr<FPSCounterImpl> mImpl;
        };
    }
}

#endif