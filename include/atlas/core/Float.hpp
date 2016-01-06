#ifndef ATLAS_INCLUDE_ATLAS_FLOAT_HPP
#define ATLAS_INCLUDE_ATLAS_FLOAT_HPP

#pragma once

#include "Atlas.hpp"
#include "Constants.hpp"

namespace atlas
{
    template <typename GenType = float>
    inline bool areEqual(GenType a, GenType b)
    {
        const GenType scale = (std::abs(a) + std::abs(b)) / GenType(2.0);
        return (std::abs(a - b) <= glm::epsilon<float>() * scale);
    }

    template <typename GenType = float>
    inline bool isZero(GenType a)
    {
        return areEqual<GenType>(a, GenType(0.0));
    }

    template <typename GenType = float>
    inline bool isOne(GenType a)
    {
        return areEqual<GenType>(a, GenType(1.0));
    }

    template <typename GenType = float>
    inline bool geq(GenType a, GenType b)
    {
        return (a > b) || areEqual<GenType>(a, b);
    }

    template <typename GenType = float>
    inline bool leq(GenType a, GenType b)
    {
        return (a < b) || areEqual<GenType>(a, b);
    }

}

#endif
