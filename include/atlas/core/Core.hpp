#ifndef ATLAS_INCLUDE_ATLAS_CORE_CORE_HPP
#define ATLAS_INCLUDE_ATLAS_CORE_CORE_HPP

#pragma once

#include "atlas/Atlas.hpp"

#ifdef USING_ATLAS_CORE_NS
#undef USING_ATLAS_CORE_NS
#endif

#define USING_ATLAS_CORE_NS using namespace atlas::core

namespace atlas
{
    namespace core
    {
        class Scene;
        class Timer;
    }
}

#endif