/**
 * \file Node.hpp
 * \brief Defines a node class for a scene graph.
 *
 */

#ifndef ATLAS_INCLUDE_ATLAS_GRAPH_NODE_HPP
#define ATLAS_INCLUDE_ATLAS_GRAPH_NODE_HPP

#pragma once

#include "Graph.hpp"

#include "atlas/core/Time.hpp"

#include <vector>
#include <memory>
#include <string>

namespace atlas
{
    namespace graph
    {
        using NodePointer = std::shared_ptr<Node>;

        class Node
        {
        public:
            Node();
            ~Node();

            void addChild(NodePointer child);
            void addChild(NodePointer child, std::string const& name);

            NodePointer getChild(int i) const;
            NodePointer getChild(std::string const& name);
            std::vector<NodePointer>& children();

            void setParent(Node* parent);
            NodePointer getParent() const;

            bool& isVisible();

            std::string& name();

        private:
            NodePointer mParent;
            std::vector<NodePointer> mChildren;
            std::string mName;
            bool mIsVisible;
        };
    }
}

#endif