#include "atlas/primitives/Primitive.hpp"

namespace atlas
{
    namespace primitives
    {
        Primitive::Primitive() :
            mVao(),
            mDataBuffer(GL_ARRAY_BUFFER),
            mIndexBuffer(GL_ELEMENT_ARRAY_BUFFER)
        { }

        Primitive::~Primitive()
        { }

        void Primitive::createBuffers()
        { 
            USING_ATLAS_MATH_NS;

            // Compile all the data into a single vector.
            std::vector<float> data;

            // Interleave vector and normal data.
            for (size_t i = 0; i < mVertices.size(); ++i)
            {
                data.push_back(mVertices[i].x);
                data.push_back(mVertices[i].y);
                data.push_back(mVertices[i].z);

                data.push_back(mNormals[i].x);
                data.push_back(mNormals[i].y);
                data.push_back(mNormals[i].z);
            }

            mVao.bindVertexArray();

            mDataBuffer.bindBuffer();
            mDataBuffer.bufferData(data.size() * sizeof(float),
                data.data(), GL_STATIC_DRAW);

            // Verticis go in location 0.
            mDataBuffer.vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                STRIDE(6, float), BUFFER_OFFSET(0));

            // Normals go in location 1.
            mDataBuffer.vertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                STRIDE(6, float), BUFFER_OFFSET(3 * sizeof(float)));

            mIndexBuffer.bindBuffer();
            mIndexBuffer.bufferData(mIndices.size() * sizeof(GLuint),
                mIndices.data(), GL_STATIC_DRAW);

            mVao.enableVertexAttribArray(0);
            mVao.enableVertexAttribArray(1);

            mVao.unBindVertexArray();
        }

        void Primitive::drawPrimitive()
        {
            mVao.bindVertexArray();
            glDrawElements(GL_TRIANGLES, (int)mIndices.size(),
                GL_UNSIGNED_INT, (void*)0);
            mVao.unBindVertexArray();
        }
    }
}