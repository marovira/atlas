#include "atlas/utils/Mesh.hpp"
#include "atlas/gl/Buffer.hpp"
#include "atlas/gl/VertexArrayObject.hpp"
#include "atlas/core/Log.hpp"

#include <unordered_set>

namespace atlas
{
    namespace utils
    {
        struct Mesh::MeshImpl
        {
            MeshImpl()
            { }

            MeshImpl(MeshImpl&& impl) = default;
            ~MeshImpl() = default;

            MeshImpl& operator=(MeshImpl&& rhs) = default;

            gl::Buffer vertexBuffer;
            gl::Buffer normalBuffer;
            gl::Buffer textureBuffer;
            gl::Buffer indexBUffer;
            gl::VertexArrayObject vao;
            
            GLuint numIndices;
            bool isValid;
            bool hasNormals;
            bool hasTextures;
        };

        Mesh::Mesh() :
            mImpl(std::make_unique<MeshImpl>())
        { }

        Mesh::Mesh(Mesh&& rhs) :
            mImpl(std::make_unique<MeshImpl>(std::move(*rhs.mImpl)))
        { }

        Mesh& Mesh::operator=(Mesh&& rhs)
        {
            *mImpl = std::move(*rhs.mImpl);
            return *this;
        }

        Mesh::~Mesh()
        { }

        Mesh& Mesh::fromTriangleSoup(
            std::vector<atlas::math::Point> const& vertices,
            std::vector<GLuint> const& indices,
            std::vector<atlas::math::Normal> const& normals,
            std::vector<atlas::math::Point2> const& uvs)
        {
            return Mesh();
        }

        std::vector<atlas::math::Point>& Mesh::vertices()
        {
            return mVertices;
        }

        std::vector<atlas::math::Normal>& Mesh::normals()
        {
            return mNormals;
        }

        std::vector<atlas::math::Point2>& Mesh::uvs()
        {
            return mUvs;
        }

        std::vector<GLuint>& Mesh::indices()
        {
            return mIndices;
        }

        bool Mesh::isValid() const
        {
            return mImpl->isValid;
        }

        void Mesh::setVertexAttribArrays(std::vector<GLuint> const& arrays)
        {

        }

        void Mesh::updateMesh()
        {

        }

        void Mesh::updateIndices()
        {

        }

        void Mesh::renderMesh()
        {

        }
    }
}