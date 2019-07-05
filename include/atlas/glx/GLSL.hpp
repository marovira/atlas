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
        FileData(std::string const& name, int p, std::uint32_t key,
                 std::time_t time) :
            filename{name},
            parent{p},
            fileKey{key},
            lastWrite{time}
        {}

        std::string filename;
        int parent;
        std::uint32_t fileKey;
        std::time_t lastWrite;
    };

    struct ShaderFile
    {
        std::string sourceString;
        std::vector<FileData> includedFiles;
    };

    constexpr std::uint32_t createFileKey(std::size_t hash)
    {
        std::uint32_t h1 = static_cast<std::uint32_t>(hash);
        std::uint32_t h2 = static_cast<std::uint32_t>(hash >> 0x20);
        return h1 ^ h2;
    }

    ShaderFile
    loadShaderFile(std::string const& filename,
                   std::vector<std::string> const& includeDirectories = {});

} // namespace atlas::glx
