#pragma once

#include <GL/gl3w.h>

#include <cstdint>
#include <experimental/filesystem>
#include <string>
#include <vector>

namespace atlas::glx
{
    struct FileData
    {
        FileData(std::string const& name, int p, std::size_t key,
                 std::time_t time) :
            filename{name},
            parent{p},
            fileKey{key},
            lastWrite{time}
        {
        }

        std::string filename;
        int         parent;
        std::size_t fileKey;
        std::time_t lastWrite;
    };

    struct ShaderFile
    {
        std::string           sourceString;
        std::vector<FileData> includedFiles;
    };

    ShaderFile loadShaderFile(std::string const& filename);

} // namespace atlas::glx
