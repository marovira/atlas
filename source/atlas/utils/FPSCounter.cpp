#include "atlas/utils/FPSCounter.hpp"
#include "atlas/core/Float.hpp"
#include "atlas/core/Time.hpp"

namespace atlas
{
    namespace utils
    {
        struct FPSCounter::FPSCounterImpl
        {
            FPSCounterImpl() :
                lastTime(0.0f),
                fps(0.0f),
                tick(0.0f)
            { }

            FPSCounterImpl(float fps) :
                lastTime(0.0f),
                fps(fps),
                tick(1.0f / fps)
            { }

            FPSCounterImpl(FPSCounterImpl const& fps) = default;

            FPSCounterImpl* clone()
            {
                return new FPSCounterImpl(*this);
            }

            float lastTime;
            float fps;
            float tick;
        };

        FPSCounter::FPSCounter() :
            mImpl(new FPSCounterImpl)
        { }

        FPSCounter::FPSCounter(float fps) :
            mImpl(new FPSCounterImpl(fps))
        { }

        FPSCounter::FPSCounter(FPSCounter const& t) :
            mImpl(t.mImpl->clone())
        { }

        FPSCounter::~FPSCounter()
        { }

        void FPSCounter::setFPS(float fps)
        {
            mImpl->fps = fps;
            mImpl->tick = 1.0f / fps;
        }

        bool FPSCounter::isFPS(atlas::core::Time<>& t) const
        {
            if (core::geq(t.currentTime - mImpl->lastTime, mImpl->tick))
            {
                mImpl->lastTime += mImpl->tick;
                t.deltaTime = mImpl->tick;
                return true;
            }

            return false;
        }
    }
}