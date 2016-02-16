#include "atlas/primitives/Cube.hpp"

#include "atlas/math/Vector.hpp"
#include <vector>

namespace atlas
{
    namespace primitives
    {
        struct Cube::CubeImpl
        {
            CubeImpl(float w, float h, float d) :
                width(w),
                height(h),
                depth(d)
            { }

            CubeImpl(CubeImpl const& impl) = default;

            ~CubeImpl()
            { }

            CubeImpl* clone()
            {
                return new CubeImpl(*this);
            }

            void createVertices(std::vector<atlas::math::Point>& vertices)
            {
                USING_ATLAS_MATH_NS;

                float halfWidth = width / 2.0f,
                    halfHeight = height / 2.0f,
                    halfDepth = depth / 2.0f;

                vertices = std::vector<Point>
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
            }

            void createNormals(std::vector<atlas::math::Normal>& normals,
                std::vector<atlas::math::Point> const& vertices)
            {
                USING_ATLAS_MATH_NS;

                std::vector<Normal> normalList;
                for (size_t i = 0; i < vertices.size(); i += 4)
                {
                    Vector v1 = vertices[i + 1] - vertices[i];
                    Vector v2 = vertices[i + 3] - vertices[i];
                    Normal n = glm::cross(v1, v2);
                    n *= ((normalList.size() % 2) == 1) ? -1.0f : 1.0f;
                    normalList.push_back(n);
                }

                normals.resize(vertices.size());
                for (size_t i = 0, j = 0; i < normals.size(); i += 4, ++j)
                {
                    normals[i] = normalList[j];
                    normals[i + 1] = normalList[j];
                    normals[i + 2] = normalList[j];
                    normals[i + 3] = normalList[j];
                }
            }

            void createIndices(std::vector<GLuint>& indices)
            {
                indices = std::vector<GLuint>
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

            float width, height, depth;
        };

        Cube::Cube(float width, float height, float depth) :
            mImpl(new CubeImpl(width, height, depth)),
            Primitive()
        {
            mImpl->createVertices(mVertices);
            mImpl->createNormals(mNormals, mVertices);
            mImpl->createIndices(mIndices);
        }

        Cube::Cube(Cube const& c) :
            mImpl(c.mImpl->clone())
        { }

        Cube::~Cube()
        { }
    }
}