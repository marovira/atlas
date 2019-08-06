#pragma once

#include <chrono>

namespace atlas::core
{
    template<typename T, typename Res = std::chrono::high_resolution_clock>
    class Timer
    {
    public:
        Timer() = default;

        void start()
        {
            mBegin = Res::now();
        }

        void reset()
        {
            mBegin = std::chrono::time_point<Res>();
        }

        T elapsed() const
        {
            return std::chrono::duration_cast<Second>(Res::now() - mBegin)
                .count();
        }

    private:
        using Second = std::chrono::duration<T>;
        std::chrono::time_point<Res> mBegin;
    };
} // namespace atlas::core
