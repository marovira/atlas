#ifndef ATLAS_INCLUDE_ATLAS_UTILS_UTILS_HPP
#define ATLAS_INCLUDE_ATLAS_UTILS_UTILS_HPP

#pragma once

#include "atlas/Atlas.hpp"

#ifdef USING_ATLAS_UTILS_NS
#undef USING_ATLAS_UTILS_NS
#endif

#define USING_ATLAS_UTILS_NS using namespace atlas::utils

namespace atlas
{
    namespace utils
    {
        class Film;
        class Geometry;
        struct Time;
    }
}

#endif