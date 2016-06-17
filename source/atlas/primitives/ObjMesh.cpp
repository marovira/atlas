#include "atlas/primitives/ObjMesh.hpp"
#include "atlas/core/Log.hpp"
#include "atlas/core/TinyObjLoader.hpp"


namespace atlas
{
    namespace primitives
    {
        ObjMesh::ObjMesh()
        { }

        ObjMesh::~ObjMesh()
        { }

        bool ObjMesh::loadFromFile(std::string const& filename)
        {
            USING_ATLAS_MATH_NS;

            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> materials;

            std::string errorString;
            bool ret = tinyobj::LoadObj(shapes, materials, errorString, 
                filename.c_str());

            if (!errorString.empty())
            {
                ERROR_LOG(errorString);
                if (!ret)
                {
                    return ret;
                }
            }

            if (shapes.size() > 1)
            {
                ERROR_LOG("Only single meshes are currently supported!");
                return false;
            }

            for (size_t v = 0; v < shapes[0].mesh.positions.size() / 3; ++v)
            {
                Vector vertex;
                vertex.x = shapes[0].mesh.positions[3 * v + 0];
                vertex.y = shapes[0].mesh.positions[3 * v + 1];
                vertex.z = shapes[0].mesh.positions[3 * v + 2];

                mVertices.push_back(vertex);

                Normal normal;
                normal.x = shapes[0].mesh.normals[3 * v + 0];
                normal.y = shapes[0].mesh.normals[3 * v + 1];
                normal.z = shapes[0].mesh.normals[3 * v + 2];

                mNormals.push_back(normal);
            }

            for (auto index : shapes[0].mesh.indices)
            {
                mIndices.push_back(index);
            }

            return true;
        }
    }
}