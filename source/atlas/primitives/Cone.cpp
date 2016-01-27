#include "atlas/primitives/Cone.hpp"

#include "atlas/math/Coordinates.hpp"

namespace atlas
{
    namespace primitives
    {
        struct Cone::ConeImpl
        {
            ConeImpl(float r, float h, int sdx, int sdy) :
                radius(r),
                height(h),
                subDivsX(sdx),
                subDivsY(sdy)
            { }

            ConeImpl(ConeImpl const& imp) = default;

            ~ConeImpl()
            { }

            ConeImpl* clone()
            {
                return new ConeImpl(*this);
            }

            void createVertices(std::vector<atlas::math::Point>& vertices)
            {
                USING_ATLAS_MATH_NS;

                float theta = 0.0f, r = 0.0f, z = height;
                float thetaDelta = glm::two_pi<float>() / subDivsX;
                float radDelta = radius / subDivsY;
                float zDelta = height / subDivsY;

                vertices.push_back(Point(0, 0, height));

                for (int y = 0; y < subDivsY; ++y)
                {
                    theta = 0.0f;
                    r += radDelta;
                    z -= zDelta;

                    for (int x = 0; x < subDivsX; ++x)
                    {
                        theta = x * thetaDelta;
                        vertices.push_back(
                            cylindricalToCartesian(Point(r, theta, z)));
                    }
                }
            }

            void createNormals(std::vector<atlas::math::Normal>& normals,
                std::vector<atlas::math::Point> const& vertices)
            {
                USING_ATLAS_MATH_NS;

                normals.push_back(Normal(0, 0, height));

                float zDelta = height / subDivsY;
                Point centre(0, 0, height);
                int i = 1;

                for (int y = 0; y < subDivsY; ++y)
                {
                    centre.y -= zDelta;

                    for (int x = 0; x < subDivsX; ++x)
                    {
                        normals.push_back(vertices[i] - centre);
                        i++;
                    }
                }
            }

            void createIndices(std::vector<GLuint>& indices)
            {
                std::vector<GLuint> upperLoop(subDivsX, 0);
                std::vector<GLuint> lowerLoop(subDivsX, 0);

                for (int i = 0; i < subDivsX; ++i)
                {
                    lowerLoop[i] = i + 1;
                }

                for (int i = 0; i < subDivsX; ++i)
                {
                    indices.push_back(upperLoop[i]);
                    indices.push_back(lowerLoop[i]);
                    indices.push_back(lowerLoop[(i + 1) % subDivsX]);
                }

                for (int y = 0; y < subDivsY - 1; ++y)
                {
                    for (int i = 0; i < subDivsX; ++i)
                    {
                        upperLoop[i] = (y * subDivsX + 1) + i;
                        lowerLoop[i] = (y * subDivsX + 1) + subDivsX + i;
                    }

                    for (int x = 0, i = 0; x < subDivsX; ++x, ++i)
                    {
                        int j = (i + 1) % subDivsX;
                        indices.push_back(upperLoop[i]);
                        indices.push_back(lowerLoop[i]);
                        indices.push_back(lowerLoop[j]);

                        indices.push_back(lowerLoop[j]);
                        indices.push_back(upperLoop[j]);
                        indices.push_back(upperLoop[i]);
                    }
                }
            }

            float radius, height;
            int subDivsX, subDivsY;

        };

        Cone::Cone(float radius, float height, int subDivsX, int subDivsY) :
            mImpl(new ConeImpl(radius, height, subDivsX, subDivsY))
        {
            mImpl->createVertices(mVertices);
            mImpl->createNormals(mNormals, mVertices);
            mImpl->createIndices(mIndices);
        }

        Cone::Cone(Cone const& c) :
            mImpl(c.mImpl->clone())
        { }

        Cone::~Cone()
        { }
    }
}