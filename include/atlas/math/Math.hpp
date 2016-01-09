/**
 *	\file Math.hpp
 *	\brief Main header for math module.
 *	
 *	This header defines the macro for using its namespaces. All headers
 *	within this module include this header.
 */
#ifndef ATLAS_INCLUDE_ATLAS_MATH_HPP
#define ATLAS_INCLUDE_ATLAS_MATH_HPP

#pragma once

#include "atlas/Atlas.hpp"

#include <glm/trigonometric.hpp>
#include <glm/exponential.hpp>

#ifdef USING_ATLAS_MATH_NS
#undef USING_ATLAS_MATH_NS
#endif

/**
 *	\def USING_ATLAS_MATH_NS
 *	Shortened version for using the math namespace.
 */
#define USING_ATLAS_MATH_NS using namespace atlas::math

#endif