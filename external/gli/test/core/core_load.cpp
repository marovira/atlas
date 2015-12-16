//////////////////////////////////////////////////////////////////////////////////
/// OpenGL Image (gli.g-truc.net)
///
/// Copyright (c) 2008 - 2015 G-Truc Creation (www.g-truc.net)
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///
/// @ref core
/// @file gli/test/core/core_load.cpp
/// @date 2013-11-25 / 2015-08-08
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include <gli/gli.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/vec1.hpp>
#include <glm/gtc/packing.hpp>
#include <glm/gtc/color_space.hpp>
#include <ctime>

namespace
{
	std::string path(std::string const & filename, char const * ext)
	{
		return std::string(SOURCE_DIR) + "/data/" + filename + ext;
	}
}//namespace

namespace load_file
{
	int test(std::string const & Filename)
	{
		int Error(0);

		gli::texture2D TextureKTX(gli::load(path(Filename, ".ktx")));
		gli::save(TextureKTX, Filename + ".dds");
		gli::texture2D TextureSavedDDS(gli::load(Filename + ".dds"));
		gli::save(TextureKTX, Filename + ".ktx");
		gli::texture2D TextureSavedKTX(gli::load(Filename + ".ktx"));

		Error += TextureSavedDDS == TextureKTX ? 0 : 1;
		Error += TextureSavedDDS == TextureSavedKTX ? 0 : 1;

		return Error;
	}
}//namespace load_file

int main()
{
	std::vector<std::string> Filenames;
	Filenames.push_back("kueken7_rgba8_srgb");
	Filenames.push_back("kueken8_rgba8_srgb");
	Filenames.push_back("kueken7_rgb8_unorm");
	Filenames.push_back("kueken7_rgba_dxt5_srgb");
	Filenames.push_back("kueken7_rgb_dxt1_srgb");
	Filenames.push_back("kueken7_rgb8_srgb");

	int Error(0);

	{
		for(std::size_t Index = 0; Index < Filenames.size(); ++Index)
			Error += load_file::test(Filenames[Index]);
	}

	return Error;
}
