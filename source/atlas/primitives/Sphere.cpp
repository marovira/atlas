#include "atlas/primitives/Sphere.hpp"

#include "atlas/math/Vector.hpp"
#include "atlas/math/Coordinates.hpp"
#include <vector>

namespace atlas
{
    namespace primitives
    {
        struct Sphere::SphereImpl
        {
            SphereImpl(float r, int sdx, int sdy) :
                radius(r),
                subDivsX(sdx),
                subDivsY(sdy)
            { }

            SphereImpl(SphereImpl const& imp) = default;

            ~SphereImpl()
            { }

            SphereImpl* clone()
            {
                return new SphereImpl(*this);
            }

            void createVertices(std::vector<atlas::math::Point>& vertices)
            {
                USING_ATLAS_MATH_NS;

                float theta = 0.0f, phi = 0.0f;
                float phiDelta = glm::pi<float>() / (subDivsY + 1);
                float thetaDelta = glm::two_pi<float>() / subDivsX;

                vertices.push_back(Point(0, 0, radius));

                for (int y = 0; y < subDivsY; ++y)
                {
                    phi += phiDelta;
                    theta = 0.0f;

                    for (int x = 0; x < subDivsX; ++x)
                    {
                        theta = x * thetaDelta;
                        vertices.push_back(
                            sphericalToCartesian(Point(radius, theta, phi)));
                    }
                }

                vertices.push_back(Point(0, 0, -radius));
            }

            void createIndices(std::vector<GLuint>& indices, int numVertices)
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

                for (int i = 0; i < subDivsX; ++i)
                {
                    lowerLoop[i] = numVertices - 1;
                    upperLoop[i] = numVertices - 1 - subDivsX + i;
                }

                for (int i = 0; i < subDivsX; ++i)
                {
                    indices.push_back(lowerLoop[i]);
                    indices.push_back(upperLoop[i]);
                    indices.push_back(upperLoop[(i + 1) % subDivsX]);
                }
            }

            float radius;
            int subDivsX, subDivsY;
        };

        Sphere::Sphere(float radius, int subDivsX, int subDivsY) :
            mImpl(new SphereImpl(radius, subDivsX, subDivsY)),
            Primitive()
        {
            USING_ATLAS_MATH_NS;

            mImpl->createVertices(mVertices);

            mNormals = mVertices;

            mImpl->createIndices(mIndices, (int)mVertices.size());

        }

        Sphere::Sphere(Sphere const& s) :
            mImpl(s.mImpl->clone())
        { }

        Sphere::~Sphere()
        { }
    }
}