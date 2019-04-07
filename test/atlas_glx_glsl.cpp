#include "TestDataPaths.hpp"

#include <GL/gl3w.h>
#include <atlas/glx/GLSL.hpp>

#include <GLFW/glfw3.h>
#include <catch2/catch.hpp>

#include <functional>

bool operator==(atlas::glx::FileData const& lhs,
                atlas::glx::FileData const& rhs)
{
    return lhs.filename == rhs.filename && lhs.parent == rhs.parent &&
           lhs.fileKey == rhs.fileKey && lhs.lastWrite == rhs.lastWrite;
}

bool operator!=(atlas::glx::FileData const& lhs,
                atlas::glx::FileData const& rhs)
{
    return !(lhs == rhs);
}

std::size_t stringHash(std::string const& str)
{
    std::hash<std::string> hash;
    return hash(str);
}

std::time_t getFileTimestamp(std::string const& filename)
{
    namespace fs = std::experimental::filesystem;
    fs::path filePath{filename};
    auto     ftime = fs::last_write_time(filePath);
    return decltype(ftime)::clock::to_time_t(ftime);
}

TEST_CASE("loadShaderFile: Non-existent file", "[glx]")
{
    using namespace atlas::glx;
    try
    {
        auto result = loadShaderFile("foo.glsl");
    }
    catch (std::runtime_error const& e)
    {
        std::string_view msg{e.what()};
        REQUIRE(msg.empty() != true);
    }
}

TEST_CASE("loadShaderFile: Empty file", "[glx]")
{
    using namespace atlas::glx;

    std::string filename{TestData[glx_empty_file]};
    auto        result = loadShaderFile(filename);
    REQUIRE(result.sourceString.empty() == true);
    REQUIRE(result.includedFiles.size() == 1);

    auto     includedFile = result.includedFiles.front();
    FileData expectedFile{filename, -1, stringHash(filename),
                          getFileTimestamp(filename)};
    REQUIRE((includedFile == expectedFile) == true);
}

TEST_CASE("loadShaderFile: Single line", "[glx]")
{
    using namespace atlas::glx;

    std::string filename{TestData[glx_single_line]};
    auto        result = loadShaderFile(filename);
    REQUIRE(result.sourceString.empty() == false);
    REQUIRE(result.includedFiles.size() == 1);

    auto     includedFile = result.includedFiles.front();
    FileData expectedFile{filename, -1, stringHash(filename),
                          getFileTimestamp(filename)};
    REQUIRE((includedFile == expectedFile) == true);
}
