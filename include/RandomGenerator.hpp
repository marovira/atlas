#ifndef APOLLO_INCLUDE_ATLAS_RANDOM_GENERATOR_HPP
#define APOLLO_INCLUDE_ATLAS_RANDOM_GENERATOR_HPP

#include <random>
#include <limits>

#include "Atlas.hpp"

namespace atlas
{
    template <typename GenType = float>
    class RandomGenerator
    {
    public:
        RandomGenerator() = default;
        RandomGenerator(std::mt19937::result_type seed) :
            mEngine(seed)
        { }

        inline GenType getRandomReal(GenType min, GenType max)
        {
            std::uniform_real_distribution<GenType> dist(min, max);
            return dist(mEngine);
        }

        inline GenType getRandomRealMax()
        {
            return getRandomReal(GenType(0.0), std::numeric_limits<GenType>::max());
        }

        inline GenType getRandomRealOne()
        {
            return getRandomReal(GenType(0.0), GenType(1.0));
        }

        inline int getRandomInt(int min, int max)
        {
            std::uniform_int_distribution<> dist(min, max);
            return dist(mEngine);
        }

        inline int getRandomInt()
        {
            return getRandomInt(0, std::numeric_limits<int>::max());
        }

    private:
        std::mt19937 mEngine{ std::random_device{} () };
    };
}

#endif
