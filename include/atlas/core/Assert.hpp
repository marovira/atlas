#ifndef ATLAS_INCLUDE_ATLAS_CORE_ASSERT_HPP
#define ATLAS_INCLUDE_ATLAS_CORE_ASSERT_HPP

#pragma once

#include "Macros.hpp"

namespace atlas
{
    namespace core
    {
        namespace Assert
        {
            void assertion(const char* exprString, bool expr, const char* file,
                int line, const char* messsage);
        }
    }
}

#ifdef ATLAS_DEBUG
#define ATLAS_ASSERT(expr, message) \
atlas::core::Assert::assertion(#expr, expr, __FILE__, __LINE__, message)
#else
#define ATLAS_ASSERT(expr, message)
#endif

#endif