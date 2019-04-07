#include <GL/gl3w.h>
#include <atlas/glx/GLSL.hpp>

#include <GLFW/glfw3.h>
#include <catch2/catch.hpp>

#include <functional>

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

TEST_CASE("loadShaderFile: Empty file", "[glx]")
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

TEST_CASE("loadShaderFile: Single line file", "[glx]")
{
    using namespace atlas::glx;

    std::string const expectedSource = "#version 450 core";
    std::string const testName       = "single_line.vs.glsl";

    auto shaderFile = loadShaderFile("single_line.vs.glsl");

    REQUIRE(shaderFile.sourceString == expectedSource);
    // REQUIRE(shaderFile.includedFiles.size() == 1);

    // auto includedFile = shaderFile.includedFiles[0];

    // REQUIRE(includedFile.filename == testName);
    // REQUIRE(includedFile.parent == -1);
    // REQUIRE(includedFile.fileKey == stringHash(testName));
    // REQUIRE(includedFile.lastWrite == getFileTimestamp(testName));
}
