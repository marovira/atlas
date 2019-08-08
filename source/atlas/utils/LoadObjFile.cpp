#include "atlas/utils/LoadObjFile.hpp"

#include <fmt/printf.h>

namespace atlas::utils
{
    std::optional<ObjMesh> loadObjMesh(std::string const& filename)
    {
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

        ObjMesh mesh;
        mesh.attrib    = reader.GetAttrib();
        mesh.shapes    = reader.GetShapes();
        mesh.materials = reader.GetMaterials();

        if (mesh.materials.empty())
        {
            mesh.materials.push_back(tinyobj::material_t{});
        }

        return mesh;
    }
} // namespace atlas::utils
