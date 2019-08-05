#include "atlas/utils/LoadObjFile.hpp"

#include <fmt/printf.h>

namespace atlas::utils
{
    std::optional<Mesh> loadObjMesh(std::string const& fileName)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;

        std::string warn;
        std::string err;

        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                                    fileName.c_str());

        if (!warn.empty())
        {
            fmt::print(stderr, "warning: in function loadOjbMesh: {}\n");
        }

        if (!err.empty())
        {
            fmt::print(stderr, "error: in function loadObjMesh: {}\n");
        }

        if (!ret)
        {
            return {};
        }

        Mesh mesh;
        std::vector<math::Point> vertices;
        std::vector<math::Normal> normals;
        std::vector<math::Point2> texCoords;
        std::vector<std::uint32_t> indices;

        std::size_t totalDataCount{0};
        for (std::size_t s{0}; s < shapes.size(); ++s)
        {
            std::size_t indexOffset{0};
            std::size_t numVertices{0};
            std::size_t numNormals{0};
            std::size_t numTex{0};
            std::size_t numIndices{0};

            std::vector<std::uint32_t> tmpIndices;
            std::vector<Face> faces;
            for (std::size_t f{0}; f < shapes[s].mesh.num_face_vertices.size();
                 ++f)
            {
                std::size_t fv = shapes[s].mesh.num_face_vertices[f];

                Face face{indices.size() + tmpIndices.size(), fv,
                          shapes[s].mesh.material_ids[f]};
                face.materialId = (face.materialId == -1) ? 0 : face.materialId;

                for (std::size_t v{0}; v < fv; ++v)
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
                        math::Normal norm;
                        norm.x = attrib.normals[3 * index.normal_index + 0];
                        norm.y = attrib.normals[3 * index.normal_index + 1];
                        norm.z = attrib.normals[3 * index.normal_index + 2];
                        normals.push_back(norm);
                        numNormals += 3;
                    }

                    if (!attrib.texcoords.empty())
                    {
                        math::Point2 t;
                        t.x = attrib.texcoords[2 * index.texcoord_index + 0];
                        t.y = attrib.texcoords[2 * index.texcoord_index + 1];
                        texCoords.push_back(t);
                        numTex += 2;
                    }

                    tmpIndices.push_back(
                        static_cast<std::uint32_t>(tmpIndices.size()));
                    ++numIndices;
                }

                indexOffset += fv;
            }

            std::vector<Face> uniqueFaces;
            Face firstFace = faces[0];
            for (std::size_t i{0}; i < faces.size(); ++i)
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

            Shape shape{totalDataCount, uniqueFaces};
            mesh.shapes.push_back(shape);
            mesh.indices.insert(mesh.indices.end(), tmpIndices.begin(),
                                tmpIndices.end());
            totalDataCount += numVertices + numNormals + numTex;
        }

        mesh.vertices  = vertices;
        mesh.normals   = normals;
        mesh.texCoords = texCoords;

        return {mesh};
    }
} // namespace atlas::utils
