#include "atlas/glx/GLSL.hpp"

#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <sstream>

namespace atlas::glx
{
    ShaderFile loadFile(std::string const& filename)
    {
        std::ifstream inFile(filename);

        if (!inFile)
        {
            // Possibly throw?
        }
    }
} // namespace atlas::glx
