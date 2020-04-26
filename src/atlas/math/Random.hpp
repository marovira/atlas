#pragma once

#include <limits>
#include <random>
#include <type_traits>

namespace atlas::math
{
    template<typename T,
             typename Engine = std::mt19937,
             typename        = std::enable_if<std::is_arithmetic<T>::value>>
    class Random
    {
    public:
        Random() = default;

        Random(typename Engine::result_type seed) : mEngine{seed}
        {}

        T getRandomRange(T min, T max)
        {
            if constexpr (std::is_floating_point<T>::value)
            {
                std::uniform_real_distribution<T> dist{min, max};
                return dist(mEngine);
            }
            else
            {
                std::uniform_int_distribution<T> dist{min, max};
                return dist(mEngine);
            }
        }

        T getRandomMax()
        {
            return getRandomRange(static_cast<T>(0),
                                  std::numeric_limits<T>::max());
        }

        T getRandomOne()
        {
            return getRandomRange(static_cast<T>(0), static_cast<T>(1));
        }

    private:
        Engine mEngine{std::random_device{}()};
    };
} // namespace atlas::math
