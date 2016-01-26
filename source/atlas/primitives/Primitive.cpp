#include "atlas/primitives/Primitive.hpp"

namespace atlas
{
    namespace primitives
    {
        Primitive::Primitive() :
            mVao(0),
            mVertexBuffer(0),
            mNormalBuffer(0),
            mIndexBuffer(0)
        { }

        Primitive::~Primitive()
        {
            glDeleteVertexArrays(1, &mVao);
            glDeleteBuffers(1, &mVertexBuffer);
            glDeleteBuffers(1, &mNormalBuffer);
            glDeleteBuffers(1, &mIndexBuffer);
        }

        void Primitive::createBuffers()
        { 
            USING_ATLAS_MATH_NS;

            glGenVertexArrays(1, &mVao);
            glBindVertexArray(mVao);

            glGenBuffers(1, &mVertexBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
            glBufferData(GL_ARRAY_BUFFER,
                mVertices.size() * sizeof(Vector),
                mVertices.data(), GL_STATIC_DRAW);

            glGenBuffers(1, &mNormalBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, mNormalBuffer);
            glBufferData(GL_ARRAY_BUFFER,
                mNormals.size() * sizeof(Normal),
                mNormals.data(), GL_STATIC_DRAW);

            glGenBuffers(1, &mIndexBuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                mIndices.size() * sizeof(GLuint),
                mIndices.data(), GL_STATIC_DRAW);
        }

        void Primitive::bindBuffers()
        {
            glBindVertexArray(mVao);
            
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, mNormalBuffer);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
        }

        void Primitive::drawBuffers()
        {
            glDrawElements(GL_TRIANGLES, (int)mIndices.size(),
                GL_UNSIGNED_INT, (void*)0);
        }

        void Primitive::unBindBuffers()
        {
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
        }

        void Primitive::drawPrimitive()
        {
            bindBuffers();
            drawBuffers();
            unBindBuffers();
        }
    }
}