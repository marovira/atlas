#include "atlas/utils/LoadObjFile.hpp"

#include <fmt/printf.h>

#include <glm/gtx/hash.hpp>

#include <algorithm>
#include <functional>
#include <unordered_map>

namespace atlas::utils
{
    bool operator==(Vertex const& rhs, Vertex const& lhs)
    {
        return rhs.position == lhs.position;
    }

    struct VertexHash
    {
        std::size_t operator()(Vertex const& vertex) const
        {
            auto posHash    = std::hash<glm::vec3>()(vertex.position);
            auto normalHash = std::hash<glm::vec3>()(vertex.normal);
            auto texHash    = std::hash<glm::vec2>()(vertex.texCoord);
            return ((posHash ^ (normalHash << 1)) >> 1) ^ (texHash << 1);
        }
    };

    std::optional<ObjMesh> loadObjMesh(std::string const& filename)
    {
        using namespace atlas::math;
        tinyobj::ObjReader reader;
        tinyobj::ObjReaderConfig config;
        config.triangulate  = true;
        config.vertex_color = false;

        bool ret = reader.ParseFromFile(filename, config);
        if (!ret || !reader.Valid())
        {
            return {};
        }

        if (!reader.Warning().empty())
        {
            fmt::print("warning: in function loadObjMesh: {}\n",
                       reader.Warning());
        }

        if (!reader.Error().empty())
        {
            fmt::print("error: in function loadObjMesh: {}\n", reader.Error());
        }

        auto attrib    = reader.GetAttrib();
        auto shapes    = reader.GetShapes();
        auto materials = reader.GetMaterials();

        if (materials.empty())
        {
            materials.push_back(tinyobj::material_t{});
        }

        ObjMesh retMesh;
        for (std::size_t s{0}; s < shapes.size(); ++s)
        {
            Shape shape{};
            std::unordered_map<Vertex, std::size_t, VertexHash> vertexMap{};
            std::size_t indexOffset{0};

            for (std::size_t face{0};
                 face < shapes[s].mesh.num_face_vertices.size(); ++face)
            {
                int numFaceVertices = shapes[s].mesh.num_face_vertices[face];
                for (int v{0}; v < numFaceVertices; ++v)
                {
                    Point p{};
                    Normal n{};
                    Point2 tex{};

                    tinyobj::index_t idx =
                        shapes[s].mesh.indices[indexOffset + v];

                    p.x = attrib.vertices[3 * idx.vertex_index + 0];
                    p.y = attrib.vertices[3 * idx.vertex_index + 1];
                    p.z = attrib.vertices[3 * idx.vertex_index + 2];

                    if (idx.normal_index != -1)
                    {
                        n.x = attrib.normals[3 * idx.normal_index + 0];
                        n.y = attrib.normals[3 * idx.normal_index + 1];
                        n.z = attrib.normals[3 * idx.normal_index + 2];
                    }

                    if (idx.texcoord_index != -1)
                    {
                        tex.x = attrib.texcoords[2 * idx.texcoord_index + 0];
                        tex.y = attrib.texcoords[2 * idx.texcoord_index + 1];
                    }

                    Vertex vertex{};
                    vertex.position = p;
                    vertex.normal   = n;
                    vertex.texCoord = tex;

                    if (vertexMap.count(vertex) == 0)
                    {
                        vertex.index      = vertexMap.size();
                        vertex.faceId     = face;
                        vertexMap[vertex] = vertex.index;
                    }

                    shape.indices.push_back(vertexMap[vertex]);
                }
                indexOffset += numFaceVertices;
            }

            for (auto& [vertex, index] : vertexMap)
            {
                shape.vertices.push_back(vertex);
            }

            shape.materialIds       = shapes[s].mesh.material_ids;
            shape.smoothingGroupIds = shapes[s].mesh.smoothing_group_ids;

            retMesh.shapes.push_back(shape);
        }

        retMesh.materials = materials;
        return {retMesh};
    }
} // namespace atlas::utils
