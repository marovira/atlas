#include "TestDataPaths.hpp"
#include "TestExpectedPaths.hpp"

#include <GL/gl3w.h>
#include <atlas/glx/GLSL.hpp>

#include <GLFW/glfw3.h>
#include <catch2/catch.hpp>
#include <fmt/format.h>

#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <sstream>

using namespace atlas::glx;

bool operator==(FileData const& lhs, FileData const& rhs)
{
    return lhs.filename == rhs.filename && lhs.parent == rhs.parent &&
           lhs.fileKey == rhs.fileKey && lhs.lastWrite == rhs.lastWrite;
}

bool operator!=(FileData const& lhs, FileData const& rhs)
{
    return !(lhs == rhs);
}

std::uint32_t stringHash(std::string const& str)
{
    std::hash<std::string> hash;
    return createFileKey(hash(str));
}

std::time_t getFileTimestamp(std::string const& filename)
{
    namespace fs = std::experimental::filesystem;
    fs::path filePath{filename};
    auto ftime = fs::last_write_time(filePath);
    return decltype(ftime)::clock::to_time_t(ftime);
}

std::string loadExpectedString(std::string const& filename,
                               std::vector<std::uint32_t> const& hashes,
                               bool substitute)
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
        if (line.find("#line") != std::string::npos && substitute)
        {
            using std::istream_iterator;
            std::istringstream iss{line};
            std::vector<std::string> tokens{istream_iterator<std::string>{iss},
                                            istream_iterator<std::string>{}};
            int idx   = std::stoi(tokens[2]);
            tokens[2] = std::to_string(hashes[idx]);
            line      = tokens[0] + " " + tokens[1] + " " + tokens[2];
        }
        outString << line + "\n";
    }
    return outString.str();
}

TEST_CASE("loadShaderFile: Non-existent file", "[glx]")
{
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
    std::string filename{TestData[glx_empty_file]};
    auto result = loadShaderFile(filename);
    REQUIRE(result.sourceString.empty() == true);
    REQUIRE(result.includedFiles.size() == 1);

    auto includedFile = result.includedFiles.front();
    FileData expectedFile{filename, -1, stringHash(filename),
                          getFileTimestamp(filename)};
    REQUIRE(includedFile == expectedFile);
}

TEST_CASE("loadShaderFile: Single line", "[glx]")
{
    std::string filename{TestData[glx_single_line]};
    std::string expectedFilename{ExpectedFiles[glx_single_line_expected]};

    // Load the expected string.
    auto expectedString =
        loadExpectedString(expectedFilename, {stringHash(filename)}, false);

    auto result = loadShaderFile(filename);
    REQUIRE(result.sourceString == expectedString);
    REQUIRE(result.includedFiles.size() == 1);

    auto includedFile = result.includedFiles.front();
    FileData expectedFile{filename, -1, stringHash(filename),
                          getFileTimestamp(filename)};
    REQUIRE(includedFile == expectedFile);
}

TEST_CASE("loadShaderFile: Simple file", "[glx]")
{
    std::string filename{TestData[glx_simple_file]};
    std::string expectedFilename{ExpectedFiles[glx_simple_file_expected]};

    auto expectedString =
        loadExpectedString(expectedFilename, {stringHash(filename)}, true);

    auto result = loadShaderFile(filename);
    REQUIRE(result.sourceString == expectedString);
    REQUIRE(result.includedFiles.size() == 1);

    auto includedFile = result.includedFiles.front();
    FileData expectedFile{filename, -1, stringHash(filename),
                          getFileTimestamp(filename)};
    REQUIRE(includedFile == expectedFile);
}

TEST_CASE("loadShaderFile: Single include", "[glx]")
{
    std::string filename{TestData[glx_single_include]};
    std::string included{TestData[uniform_matrices]};
    std::string expectedFilename{ExpectedFiles[glx_single_include_expected]};
    auto expectedString = loadExpectedString(
        expectedFilename, {stringHash(filename), stringHash(included)}, true);

    auto result = loadShaderFile(filename);
    REQUIRE(result.sourceString == expectedString);
    REQUIRE(result.includedFiles.size() == 2);
}
