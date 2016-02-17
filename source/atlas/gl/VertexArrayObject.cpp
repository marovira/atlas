#include "atlas/gl/VertexArrayObject.hpp"
#include "atlas/gl/ErrorCheck.hpp"
#include "atlas/core/Log.hpp"

#include <vector>
#include <algorithm>

namespace atlas
{
    namespace gl
    {
        struct VertexArrayObject::VertexArrayImpl
        {
            VertexArrayImpl() :
                handle(0)
            { }

            VertexArrayImpl(VertexArrayImpl const& impl) = default;

            ~VertexArrayImpl()
            { }

            VertexArrayImpl* clone()
            {
                return new VertexArrayImpl(*this);
            }

            GLuint handle;
            std::vector<GLuint> vertexArrays;
        };

        VertexArrayObject::VertexArrayObject() :
            mImpl(new VertexArrayImpl)
        {
            glGenVertexArrays(1, &mImpl->handle);
        }

        VertexArrayObject::VertexArrayObject(VertexArrayObject const& vao) :
            mImpl(vao.mImpl->clone())
        { }

        VertexArrayObject::~VertexArrayObject()
        {
            for (auto index : mImpl->vertexArrays)
            {
                glDisableVertexAttribArray(index);
            }

            glDeleteVertexArrays(1, &mImpl->handle);
        }

        void VertexArrayObject::bindVertexArray()
        {
            glBindVertexArray(mImpl->handle);
            GL_ERROR_CHECK();
        }

        void VertexArrayObject::unBindVertexArray()
        {
            glBindVertexArray(0);
        }

        void VertexArrayObject::enableVertexAttribArray(GLuint index)
        {
            std::vector<GLuint>::iterator ind = std::find(
                mImpl->vertexArrays.begin(), mImpl->vertexArrays.end(), index);
            if (ind != mImpl->vertexArrays.end())
            {
                WARN_LOG("The vertex attribute array at index " +
                    std::to_string(index) + " has already been bound");
                return;
            }

            glEnableVertexAttribArray(index);
            mImpl->vertexArrays.push_back(index);
            GL_ERROR_CHECK();
        }

        void VertexArrayObject::disableVertexAttribArray(GLuint index)
        {
            glDisableVertexAttribArray(index);
            std::vector<GLuint>::iterator ind = std::find(
                mImpl->vertexArrays.begin(), mImpl->vertexArrays.end(), index);
            mImpl->vertexArrays.erase(ind);
            GL_ERROR_CHECK();
        }
    }
}