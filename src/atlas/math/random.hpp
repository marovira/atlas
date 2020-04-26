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

        Random(typename Engine::result_type seed) : m_engine{seed}
        {}

        T get_random_in_range(T min, T max)
        {
            if constexpr (std::is_floating_point<T>::value)
            {
                std::uniform_real_distribution<T> dist{min, max};
                return dist(m_engine);
            }
            else
            {
                std::uniform_int_distribution<T> dist{min, max};
                return dist(m_engine);
            }
        }

        T get_random_zero_to_max()
        {
            return get_random_in_range(static_cast<T>(0),
                                       std::numeric_limits<T>::max());
        }

        T get_random_zero_to_one()
        {
            return get_random_in_range(static_cast<T>(0), static_cast<T>(1));
        }

    private:
        Engine m_engine{std::random_device{}()};
    };
} // namespace atlas::math
