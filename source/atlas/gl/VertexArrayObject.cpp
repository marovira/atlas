#include "atlas/gl/VertexArrayObject.hpp"

#include <vector>

namespace atlas
{
    namespace gl
    {
        struct VertexArray::VertexArrayImpl
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
            std::vector<Buffer> buffers;
        };

        VertexArray::VertexArray() :
            mImpl(new VertexArrayImpl)
        {
            glGenVertexArrays(1, &mImpl->handle);
        }

        VertexArray::VertexArray(VertexArray const& vao) :
            mImpl(vao.mImpl->clone())
        { }

        VertexArray::~VertexArray()
        {
            glDeleteVertexArrays(1, &mImpl->handle);
        }

        void VertexArray::bindVertexArray()
        {
            glBindVertexArray(mImpl->handle);
        }

        void VertexArray::unBindVertexArray()
        {
            glBindVertexArray(0);
        }

        Buffer& VertexArray::addBuffer(GLenum target)
        {
            mImpl->buffers.push_back(Buffer(target));
            return mImpl->buffers.back();
        }

        void VertexArray::deleteBufferAt(int index)
        {
            mImpl->buffers.erase(mImpl->buffers.begin() + index);
        }

        Buffer& VertexArray::operator[](int index)
        {
            return mImpl->buffers[index];
        }

        Buffer VertexArray::operator[](int index) const
        {
            return mImpl->buffers[index];
        }

        std::vector<Buffer>::iterator VertexArray::begin()
        {
            return mImpl->buffers.begin();
        }

        std::vector<Buffer>::const_iterator VertexArray::begin() const
        {
            return mImpl->buffers.cbegin();
        }

        std::vector<Buffer>::iterator VertexArray::end()
        {
            return mImpl->buffers.end();
        }

        std::vector<Buffer>::const_iterator VertexArray::end() const
        {
            return mImpl->buffers.cend();
        }
    }
}