/**
 *	\file Atlas.hpp
 *	\brief Main header for Atlas library.
 *	
 *	This header defines the atlas namespace macros as well as the ones
 *	used for GLM and GLI. All module headers include this header.
 */
#ifndef ATLAS_INCLUDE_ATLAS_ATLAS_HPP
#define ATLAS_INCLUDE_ATLAS_ATLAS_HPP

#pragma once

#ifdef USING_ATLAS_NS
#undef USING_ATLAS_NS
#endif

/**
 *	\def USING_ATLAS_NS
 *	Shortened version for using the atlas namespace.
 */
#define USING_ATLAS_NS using namespace atlas;

#ifdef USING_GLM_NS
#undef USING_GLM_NS
#endif

/**
 *	\def USING_GLM_NS
 *	Shortened version for using the glm namespace.
 */
#define USING_GLM_NS using namespace glm;

#ifdef USING_GLI_NS
#undef USING_GLI_NS
#endif

/**
 *	\def USING_GLI_NS
 *	Shortened version for using the gli namespace.
 */
#define USING_GLI_NS using namespace gli;

#endif