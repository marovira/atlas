#include "atlas/utils/ObjLoader.hpp"
#include "atlas/core/Log.hpp"
#include "atlas/gl/GL.hpp"

namespace atlas
{
    namespace utils
    {
        bool loadMeshFromFile(std::string const& file, Mesh& mesh,
            std::string const& mtl, bool triangulate)
        {
            tinyobj::attrib_t attrib;
            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> materials;

            std::string errorString;
            bool ret = tinyobj::LoadObj(&attrib, &shapes,
                &materials, &errorString, file.c_str(), 
                mtl.c_str(), triangulate);

            if (!errorString.empty())
            {
                ERROR_LOG(errorString);
            }

            if (!ret)
            {
                return false;
            }

            std::vector<GLuint> indices;
            std::vector<math::Point> vertices;
            std::vector<math::Normal> normals;
            std::vector<math::Point2> texCoords;

            using std::size_t;

            for (size_t s = 0; s < shapes.size(); ++s)
            {
                size_t indexOffset = 0;
                size_t numVertices = 0, numNormals = 0, numTex = 0,
                    numIndices = 0;

                std::vector<GLuint> tmpIndices;
                std::vector<Face> faces;

                for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size();
                    ++f)
                {
                    size_t fv = shapes[s].mesh.num_face_vertices[f];

                    Face face = { indices.size() + tmpIndices.size(),
                    fv, shapes[s].mesh.material_ids[f] };
                    face.materialId = (face.materialId == -1) ? 
                        0 : face.materialId;
                    faces.push_back(face);

                    for (size_t v = 0; v < fv; ++v)
                    {
                        tinyobj::index_t index =
                            shapes[s].mesh.indices[indexOffset + v];

                        math::Point vertex;
                        vertex.x = attrib.vertices[3 * index.vertex_index + 0];
                        vertex.y = attrib.vertices[3 * index.vertex_index + 1];
                        vertex.z = attrib.vertices[3 * index.vertex_index + 2];

                        vertices.push_back(vertex);
                        numVertices += 3;

                        if (!attrib.normals.empty())
                        {
                            math::Normal normal;
                            normal.x = attrib.normals[3 * index.normal_index + 0];
                            normal.y = attrib.normals[3 * index.normal_index + 1];
                            normal.z = attrib.normals[3 * index.normal_index + 2];
                            normals.push_back(normal);
                            numNormals += 3;
                        }

                        if (!attrib.texcoords.empty())
                        {
                            math::Point2 tex;
                            tex.x = attrib.texcoords[2 * index.texcoord_index + 0];
                            tex.y = attrib.texcoords[2 * index.texcoord_index + 1];
                            texCoords.push_back(tex);
                            numTex += 2;
                        }

                        tmpIndices.push_back(tmpIndices.size());
                        numIndices++;
                    }
                    indexOffset += fv;
                }

                std::vector<Face> uniqueFaces;

                Face firstFace = faces[0];
                for (size_t i = 1; i < faces.size(); ++i)
                {
                    if (faces[i].materialId == firstFace.materialId)
                    {
                        firstFace.size += faces[i].size;
                    }
                    else
                    {
                        uniqueFaces.push_back(firstFace);
                        firstFace = faces[i];
                    }
                }
                uniqueFaces.push_back(firstFace);

                // Add the shapes to the mesh here.

            }

            return true;
        }
    }
}