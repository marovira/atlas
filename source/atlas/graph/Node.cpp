#include "atlas/graph/Node.hpp"
#include "atlas/core/Macros.hpp"

namespace atlas
{
    namespace graph
    {
        Node::Node()
        { }

        Node::~Node()
        { }

        void Node::addChild(NodePointer child)
        {
            if (!child || !child->getParent())
            {
                return;
            }

            child->setParent(this);
            mChildren.push_back(child);
        }

        void Node::addChild(NodePointer child, std::string const& name)
        {
            if (!child || !child->getParent())
            {
                return;
            }

            child->setParent(this);
            child->name() = name;
            mChildren.push_back(child);
        }

        NodePointer Node::getChild(int i) const
        {
            if (i < 0 || i >= mChildren.size())
            {
                return nullptr;
            }

            return mChildren[i];
        }

        NodePointer Node::getChild(std::string const& name)
        {
            for (auto& child : mChildren)
            {
                if (child->name().compare(name) == 0)
                {
                    return child;
                }
            }

            return nullptr;
        }

        std::vector<NodePointer>& Node::children()
        {
            return mChildren;
        }

        void Node::setParent(Node* parent)
        {
            mParent.reset(parent);
        }

        NodePointer Node::getParent() const
        {
            return mParent;
        }

        bool& Node::isVisible()
        {
            return mIsVisible;
        }

        std::string& Node::name()
        {
            return mName;
        }
    }
}