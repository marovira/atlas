/**
 * \file Graph.hpp
 * \brief Main header for the graph module.
 * 
 *	This header defines the macro for using its namespaces. All headers
 *	within this module include this header.
 */

#ifndef ATLAS_INCLUDE_ATLAS_GRAPH_GRAPH_HPP
#define ATLAS_INCLUDE_ATLAS_GRAPH_GRAPH_HPP

#pragma once

#include "atlas/Atlas.hpp"

#ifdef USING_ATLAS_GRAPH_NS
#undef USING_ATLAS_GRAPH_NS
#endif

/**
 * \def USING_ATLAS_GRAPH_NS
 * Shortened version for using the atlas::graph namespace.
 */
#define USING_ATLAS_GRAPH_NS using namespace atlas::graph

namespace atlas
{
    namespace graph
    {
        class Node;
    }
}

#endif