#ifndef ATLAS_INCLUDE_ATLAS_CORE_NUMERIC_HPP
#define ATLAS_INCLUDE_ATLAS_CORE_NUMERIC_HPP

#pragma once

#include <vector>

namespace atlas
{
    namespace core
    {
        template <typename ItType, typename GenType>
        void iotaStride(ItType begin, ItType end, GenType start, GenType step)
        {
            while (begin != end)
            {
                *begin++ = start;
                start += step;
            }
        }
    }
}

#endif