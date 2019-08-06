#include "TestDataPaths.hpp"
#include "TestExpectedPaths.hpp"

#include <GL/gl3w.h>
#include <atlas/core/Platform.hpp>
#include <atlas/core/Filesystem.hpp>
#include <atlas/glx/GLSL.hpp>

#include <GLFW/glfw3.h>
#include <catch2/catch.hpp>
#include <fmt/printf.h>

#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <sstream>

using namespace atlas::glx;
using namespace atlas::core;
namespace fs = std::experimental::filesystem;

bool operator==(FileData const& lhs, FileData const& rhs)
{
    return lhs.filename == rhs.filename && lhs.parent == rhs.parent &&
           lhs.lastWrite == rhs.lastWrite;
}

bool operator!=(FileData const& lhs, FileData const& rhs)
{
    return !(lhs == rhs);
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

std::string normalizePath(std::string const& path)
{
    fs::path p{path};
    p = p.make_preferred();
    return p.string();
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
    std::string filename = normalizePath(TestData[glx_empty_file]);
    auto result          = readShaderSource(filename);
    REQUIRE(result.sourceString.empty() == true);
    REQUIRE(result.includedFiles.size() == 1);

    auto includedFile = result.includedFiles.front();
    FileData expectedFile{filename, -1, getFileLastWrite(filename)};
    REQUIRE(includedFile == expectedFile);
}

TEST_CASE("loadShaderFile: Single line", "[glx]")
{
    std::string filename = normalizePath(TestData[glx_single_line]);
    std::string expectedFilename{ExpectedFiles[glx_single_line_expected]};

    // Load the expected string.
    auto expectedString = loadExpectedString(expectedFilename);

    auto result = readShaderSource(filename);
    REQUIRE(result.sourceString == expectedString);
    REQUIRE(result.includedFiles.size() == 1);

    auto includedFile = result.includedFiles.front();
    FileData expectedFile{filename, -1, getFileLastWrite(filename)};
    REQUIRE(includedFile == expectedFile);
}

TEST_CASE("loadShaderFile: Simple file", "[glx]")
{
    std::string filename = normalizePath(TestData[glx_simple_file]);
    std::string expectedFilename{ExpectedFiles[glx_simple_file_expected]};

    auto expectedString = loadExpectedString(expectedFilename);

    auto result = readShaderSource(filename);
    REQUIRE(result.sourceString == expectedString);
    REQUIRE(result.includedFiles.size() == 1);

    auto includedFile = result.includedFiles.front();
    FileData expectedFile{filename, -1, getFileLastWrite(filename)};
    REQUIRE(includedFile == expectedFile);
}

TEST_CASE("loadShaderFile: Single include", "[glx]")
{
    std::string filename = normalizePath(TestData[glx_single_include]);
    std::string expectedFilename{ExpectedFiles[glx_single_include_expected]};
    auto expectedString = loadExpectedString(expectedFilename);

    auto result = readShaderSource(filename);
    REQUIRE(result.sourceString == expectedString);
    REQUIRE(result.includedFiles.size() == 2);

    auto includedFile = result.includedFiles[0];
    FileData expectedFile{filename, -1, getFileLastWrite(filename)};
    REQUIRE(includedFile == expectedFile);

    includedFile           = result.includedFiles[1];
    expectedFile.filename  = normalizePath(TestData[uniform_matrices]);
    expectedFile.parent    = 0;
    expectedFile.lastWrite = getFileLastWrite(TestData[uniform_matrices]);
    REQUIRE(includedFile == expectedFile);
}
