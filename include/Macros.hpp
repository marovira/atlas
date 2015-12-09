#ifndef APOLLO_INCLUDE_ATLAS_MACROS_HPP
#define APOLLO_INCLUDE_ATLAS_MACROS_HPP

#pragma once

#define UNUSED(expr) (void)expr

#if defined(_DEBUG) || defined(DEBUG) || defined(__DEBUG__)
#define APOLLO_DEBUG
#endif

#endif
