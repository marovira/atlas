#include "atlas/primitives/Disk.hpp"

#include "atlas/math/Coordinates.hpp"

namespace atlas
{
    namespace primitives
    {
        struct Disk::DiskImpl
        {
            DiskImpl(float r, int sd) :
                radius(r),
                subDivs(sd)
            { }

            DiskImpl(DiskImpl const& imp) = default;

            ~DiskImpl()
            { }

            DiskImpl* clone()
            {
                return new DiskImpl(*this);
            }

            void createVertices(std::vector<atlas::math::Point>& vertices)
            {
                USING_ATLAS_MATH_NS;

                float theta = 0.0f;
                float thetaDelta = glm::two_pi<float>() / subDivs;

                vertices.push_back(Point(0));

                for (int x = 0; x < subDivs; ++x)
                {
                    theta = x * thetaDelta;
                    vertices.push_back(
                        cylindricalToCartesian(Point(radius, theta, 0)));
                }

                vertices.push_back(Point(0));
                for (int x = 0; x < subDivs; ++x)
                {
                    theta = x * thetaDelta;
                    vertices.push_back(
                        cylindricalToCartesian(Point(radius, theta, 0)));
                }

            }

            void createNormals(std::vector<atlas::math::Normal>& normals,
                size_t numVerts)
            {
                for (size_t i = 0; i < numVerts / 2; ++i)
                {
                    normals.push_back(atlas::math::Normal(0, 0, 1));
                }

                for (size_t i = numVerts / 2; i < numVerts; ++i)
                {
                    normals.push_back(atlas::math::Normal(0, 0, -1));
                }
            }

            void createIndices(std::vector<GLuint>& indices, size_t numVerts)
            {
                std::vector<GLuint> upperLoop(subDivs, 0);
                std::vector<GLuint> lowerLoop(subDivs, 0);

                for (int i = 0; i < subDivs; ++i)
                {
                    lowerLoop[i] = i + 1;
                }

                for (size_t i = 0; i < subDivs; ++i)
                {
                    indices.push_back(upperLoop[i]);
                    indices.push_back(lowerLoop[i]);
                    indices.push_back(lowerLoop[(i + 1) % subDivs]);
                }

                GLuint half = (GLuint)numVerts / 2;
                for (int i = 0; i < subDivs; ++i)
                {
                    upperLoop[i] = half;
                    lowerLoop[i] = half + i + 1;
                }

                for (size_t i = 0; i < subDivs; ++i)
                {
                    indices.push_back(lowerLoop[i]);
                    indices.push_back(upperLoop[i]);
                    indices.push_back(upperLoop[(i + 1) % subDivs]);
                }
            }

            float radius;
            int subDivs;
        };

        Disk::Disk(float radius, int subDivs) :
            mImpl(new DiskImpl(radius, subDivs))
        {
            mImpl->createVertices(mVertices);
            mImpl->createNormals(mNormals, mVertices.size());
            mImpl->createIndices(mIndices, mVertices.size());
        }

        Disk::Disk(Disk const& d) :
            mImpl(d.mImpl->clone())
        { }

        Disk::~Disk()
        { }
    }
}