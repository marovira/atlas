#pragma once

#include <GL/gl3w.h>

namespace atlas::glx
{
    template<typename T>
    constexpr void* bufferOffset(std::size_t offset)
    {
        return reinterpret_cast<void*>(offset * sizeof(T));
    }

    template<typename T>
    constexpr GLsizei stride(std::size_t step)
    {
        return static_cast<GLsizei>(step * sizeof(T));
    }

    template<typename T>
    constexpr GLsizei size(std::size_t num)
    {
        return static_cast<GLsizei>(num * sizeof(T));
    }
} // namespace atlas::glx
