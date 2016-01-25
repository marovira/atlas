#include "atlas/primitives/Cube.hpp"

#include "atlas/math/Vector.hpp"
#include <vector>

namespace atlas
{
    namespace primitives
    {
        struct Cube::CubeImpl
        {
            CubeImpl(float w = 1.0f, float h = 1.0f, float d = 1.0f) :
                vao(0),
                vertexBuffer(0),
                normalBuffer(0),
                width(w),
                height(h),
                depth(d)
            { }

            CubeImpl(CubeImpl const& impl) = default;

            ~CubeImpl()
            {
                glDeleteVertexArrays(1, &vao);
                glDeleteBuffers(1, &vertexBuffer);
                glDeleteBuffers(1, &normalBuffer);
            }

            CubeImpl* clone()
            {
                return new CubeImpl(*this);
            }

            GLuint vao;
            GLuint vertexBuffer;
            GLuint indexBuffer;
            GLuint normalBuffer;

            float width, height, depth;
            std::vector<atlas::math::Point> vertices;
            std::vector<atlas::math::Normal> normals;
            std::vector<GLuint> indices;
        };

        Cube::Cube(float width, float height, float depth) :
            mImpl(new CubeImpl(width, height, depth)),
            Primitive()
        {
            USING_ATLAS_MATH_NS;

            float halfWidth = width / 2.0f,
                halfHeight = height / 2.0f,
                halfDepth = depth / 2.0f;

            mImpl->vertices = std::vector<Point>
            {
                // Top face.
                {  halfWidth, halfHeight, -halfDepth },
                { -halfWidth, halfHeight, -halfDepth },
                { -halfWidth, halfHeight,  halfDepth },
                {  halfWidth, halfHeight,  halfDepth },

                // Bottom face.
                {  halfWidth, -halfHeight, -halfDepth },
                { -halfWidth, -halfHeight, -halfDepth },
                { -halfWidth, -halfHeight,  halfDepth },
                {  halfWidth, -halfHeight,  halfDepth },

                // Left face.
                { -halfWidth,  halfHeight,  halfDepth },
                { -halfWidth,  halfHeight, -halfDepth },
                { -halfWidth, -halfHeight, -halfDepth },
                { -halfWidth, -halfHeight,  halfDepth },

                // Right face.
                { halfWidth,  halfHeight,  halfDepth },
                { halfWidth,  halfHeight, -halfDepth },
                { halfWidth, -halfHeight, -halfDepth },
                { halfWidth, -halfHeight,  halfDepth },

                // Front face.
                {  halfWidth,  halfHeight, halfDepth },
                { -halfWidth,  halfHeight, halfDepth },
                { -halfWidth, -halfHeight, halfDepth },
                {  halfWidth, -halfHeight, halfDepth },

                // Back face.
                {  halfWidth,  halfHeight, -halfDepth },
                { -halfWidth,  halfHeight, -halfDepth },
                { -halfWidth, -halfHeight, -halfDepth },
                {  halfWidth, -halfHeight, -halfDepth }
            };

            std::vector<Normal> normalList;
            for (size_t i = 0; i < mImpl->vertices.size(); i += 4)
            {
                Vector v1 = mImpl->vertices[i + 1] - mImpl->vertices[i];
                Vector v2 = mImpl->vertices[i + 3] - mImpl->vertices[i];
                Normal n = glm::cross(v1, v2);
                n *= ((normalList.size() % 2) == 1) ? -1.0f : 1.0f;
                normalList.push_back(n);
            }

            mImpl->normals.resize(mImpl->vertices.size());
            for (size_t i = 0, j = 0; i < mImpl->normals.size(); i += 4, ++j)
            {
                mImpl->normals[i] = normalList[j];
                mImpl->normals[i + 1] = normalList[j];
                mImpl->normals[i + 2] = normalList[j];
                mImpl->normals[i + 3] = normalList[j];
            }

            mImpl->indices = std::vector<GLuint>
            {
                // Top face.
                0, 1, 2,
                2, 3, 0,

                // Bottom face.
                4, 5, 6,
                6, 7, 4,

                // Left face.
                8, 9, 10,
                10, 11, 8,

                // Right face.
                12, 13, 14,
                14, 15, 12,

                // Front face.
                16, 17, 18,
                18, 19, 16,

                // Back face.
                20, 21, 22,
                22, 23, 20
            };
        }

        Cube::Cube(Cube const& c) :
            mImpl(c.mImpl->clone())
        { }

        Cube::~Cube()
        { }

        void Cube::createBuffers()
        {
            USING_ATLAS_MATH_NS;

            glGenVertexArrays(1, &mImpl->vao);
            glBindVertexArray(mImpl->vao);

            glGenBuffers(1, &mImpl->vertexBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, mImpl->vertexBuffer);
            glBufferData(GL_ARRAY_BUFFER,
                mImpl->vertices.size() * sizeof(Vector),
                mImpl->vertices.data(), GL_STATIC_DRAW);

            glGenBuffers(1, &mImpl->normalBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, mImpl->normalBuffer);
            glBufferData(GL_ARRAY_BUFFER,
                mImpl->normals.size() * sizeof(Normal),
                mImpl->normals.data(), GL_STATIC_DRAW);

            glGenBuffers(1, &mImpl->indexBuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mImpl->indexBuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                mImpl->indices.size() * sizeof(GLuint),
                mImpl->indices.data(), GL_STATIC_DRAW);
        }

        void Cube::drawBuffers()
        {
            glBindVertexArray(mImpl->vao);

            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, mImpl->vertexBuffer);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, mImpl->normalBuffer);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mImpl->indexBuffer);

            glDrawElements(GL_TRIANGLES, (int)mImpl->indices.size(), 
                GL_UNSIGNED_INT, (void*)0);

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
        }
    }
}