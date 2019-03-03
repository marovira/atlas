#pragma once

#include "GL.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace atlas::glx
{
    struct ShaderFile
    {
        ShaderFile() = default;

        std::string filePath;
        std::string fileContents;
        std::vector<ShaderFile> includedFiles;
        std::uint16_t hash;
    };

    ShaderFile loadFile(std::string const& filename);
    std::string assembleShader(ShaderFile const& shader);
    GLuint compileShader(std::string const& shaderFile);

} // namespace atlas::glx
