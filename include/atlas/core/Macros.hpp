#ifndef ATLAS_INCLUDE_ATLAS_CORE_MACROS_HPP
#define ATLAS_INCLUDE_ATLAS_CORE_MACROS_HPP

#pragma once

#include "Core.hpp"

#define UNUSED(expr) (void)expr

#if defined(_DEBUG) || defined(DEBUG) || defined(__DEBUG__)
#define ATLAS_DEBUG
#endif

#endif