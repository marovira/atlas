#include "TestDataPaths.hpp"
#include "TestExpectedPaths.hpp"

#include <GL/gl3w.h>
#include <atlas/core/FMT.hpp>
#include <atlas/core/Platform.hpp>
#include <atlas/glx/GLSL.hpp>

#include <GLFW/glfw3.h>
#include <catch2/catch.hpp>

#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <sstream>

using namespace atlas::glx;

#if defined(ATLAS_PLATFORM_WINDOWS)
namespace fs = std::experimental::filesystem;
#else
namespace fs = std::filesystem;
#endif

bool operator==(FileData const& lhs, FileData const& rhs)
{
    return lhs.filename == rhs.filename && lhs.parent == rhs.parent &&
           lhs.lastWrite == rhs.lastWrite;
}

bool operator!=(FileData const& lhs, FileData const& rhs)
{
    return !(lhs == rhs);
}

std::time_t getFileTimestamp(std::string const& filename)
{
    fs::path filePath{filename};
    auto ftime = fs::last_write_time(filePath);
    return decltype(ftime)::clock::to_time_t(ftime);
}

std::string loadExpectedString(std::string const& filename)
{
    std::ifstream inStream{filename};
    std::stringstream outString;

    if (!inStream)
    {
        auto message =
            fmt::format("Could not open expected test file: {}", filename);
        throw std::runtime_error{message.c_str()};
    }

    std::string line;
    while (std::getline(inStream, line))
    {
        // Substitute the placeholder hash for the real one.
        outString << line + "\n";
    }
    return outString.str();
}

TEST_CASE("loadShaderFile: Non-existent file", "[glx]")
{
    try
    {
        auto result = readShaderSource("foo.glsl");
    }
    catch (std::runtime_error const& e)
    {
        std::string_view msg{e.what()};
        REQUIRE(msg.empty() != true);
    }
}

TEST_CASE("loadShaderFile: Empty file", "[glx]")
{
    std::string filename{TestData[glx_empty_file]};
    auto result = readShaderSource(filename);
    REQUIRE(result.sourceString.empty() == true);
    REQUIRE(result.includedFiles.size() == 1);

    auto includedFile = result.includedFiles.front();
    FileData expectedFile{filename, -1, getFileTimestamp(filename)};
    REQUIRE(includedFile == expectedFile);
}

TEST_CASE("loadShaderFile: Single line", "[glx]")
{
    std::string filename{TestData[glx_single_line]};
    std::string expectedFilename{ExpectedFiles[glx_single_line_expected]};

    // Load the expected string.
    auto expectedString = loadExpectedString(expectedFilename);

    auto result = readShaderSource(filename);
    REQUIRE(result.sourceString == expectedString);
    REQUIRE(result.includedFiles.size() == 1);

    auto includedFile = result.includedFiles.front();
    FileData expectedFile{filename, -1, getFileTimestamp(filename)};
    REQUIRE(includedFile == expectedFile);
}

TEST_CASE("loadShaderFile: Simple file", "[glx]")
{
    std::string filename{TestData[glx_simple_file]};
    std::string expectedFilename{ExpectedFiles[glx_simple_file_expected]};

    auto expectedString = loadExpectedString(expectedFilename);

    auto result = readShaderSource(filename);
    REQUIRE(result.sourceString == expectedString);
    REQUIRE(result.includedFiles.size() == 1);

    auto includedFile = result.includedFiles.front();
    FileData expectedFile{filename, -1, getFileTimestamp(filename)};
    REQUIRE(includedFile == expectedFile);
}

TEST_CASE("loadShaderFile: Single include", "[glx]")
{
    std::string filename{TestData[glx_single_include]};
    std::string expectedFilename{ExpectedFiles[glx_single_include_expected]};
    auto expectedString = loadExpectedString(expectedFilename);

    auto result = readShaderSource(filename);
    REQUIRE(result.sourceString == expectedString);
    REQUIRE(result.includedFiles.size() == 2);

    auto includedFile = result.includedFiles[0];
    FileData expectedFile{filename, -1, getFileTimestamp(filename)};
    REQUIRE(includedFile == expectedFile);

    fs::path p{TestData[uniform_matrices]};
    p = p.make_preferred();

    includedFile           = result.includedFiles[1];
    expectedFile.filename  = p.string();
    expectedFile.parent    = 0;
    expectedFile.lastWrite = getFileTimestamp(TestData[uniform_matrices]);
    REQUIRE(includedFile == expectedFile);
}
