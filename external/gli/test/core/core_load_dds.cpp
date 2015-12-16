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
/// @file gli/test/core/core_load_dds.cpp
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
	std::string path(const char* filename)
	{
		return std::string(SOURCE_DIR) + "/data/" + filename;
	}
}//namespace

namespace load_file
{
	int test(std::string const & Filename)
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds(path(Filename.c_str())));
		gli::save_dds(TextureA, Filename.c_str());
		gli::texture2D TextureB(gli::load_dds(Filename.c_str()));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_file

namespace load_mem
{
	int test(std::string const & Filename)
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds(path(Filename.c_str())));
		gli::save_dds(TextureA, Filename.c_str());
		gli::texture2D TextureB(gli::load_dds(Filename.c_str()));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_mem

namespace load_mem_only
{
	int test(std::vector<char> const & Data, std::string const & Filename)
	{
		int Error(0);

		gli::texture2D TextureA(gli::load_dds(&Data[0], Data.size()));
		std::vector<char> Memory;
		gli::save_dds(TextureA, Memory);
		gli::texture2D TextureB(gli::load_dds(&Memory[0], Memory.size()));

		Error += TextureA == TextureB ? 0 : 1;

		return Error;
	}
}//namespace load_mem_only

int main()
{
	std::vector<std::string> Filenames;
	Filenames.push_back("kueken7_rgb8_unorm.dds");
	Filenames.push_back("kueken7_bgrx8_unorm.dds");
	Filenames.push_back("kueken7_rgba_dxt5_srgb.dds");
	Filenames.push_back("kueken7_rgb_dxt1_srgb.dds");
	Filenames.push_back("kueken7_rgba8_srgb.dds");
	Filenames.push_back("kueken7_rgb8_srgb.dds");
	Filenames.push_back("kueken7_bgra8_srgb.dds");
	Filenames.push_back("kueken7_a8_unorm.dds");
	Filenames.push_back("kueken7_bgra8_unorm.dds");
	Filenames.push_back("kueken7_l8_unorm.dds");
	//Filenames.push_back("kueken7_la8_unorm.dds");
	Filenames.push_back("kueken7_r_ati1n_unorm.dds");
	Filenames.push_back("kueken7_r5g6b5_unorm.dds");
	Filenames.push_back("kueken7_r8_snorm.dds");
	Filenames.push_back("kueken7_r8_unorm.dds");
	Filenames.push_back("kueken7_r16_unorm.dds");
	Filenames.push_back("kueken7_rg_ati2n_unorm.dds");
	Filenames.push_back("kueken7_rg11b10_ufloat.dds");
	Filenames.push_back("kueken7_rgb_atc_unorm.dds");
	Filenames.push_back("kueken7_rgb_dxt1_unorm.dds");
	Filenames.push_back("kueken7_rgb_etc1_unorm.dds");
	//Filenames.push_back("kueken7_rgb_etc2_srgb.dds");
	//Filenames.push_back("kueken7_rgb_etc2_unorm.dds");
	Filenames.push_back("kueken7_rgb_pvrtc_2bpp_unorm.dds");
	Filenames.push_back("kueken7_rgb_pvrtc_4bpp_unorm.dds");
	Filenames.push_back("kueken7_rgb9e5_ufloat.dds");
	Filenames.push_back("kueken7_rgb10a2_unorm.dds");
	Filenames.push_back("kueken7_rgb10a2u.dds");
	//Filenames.push_back("kueken7_rgba_astc8x8.dds");
	Filenames.push_back("kueken7_rgba_atc_explicit_unorm.dds");
	Filenames.push_back("kueken7_rgba_atc_interpolate_unorm.dds");
	Filenames.push_back("kueken7_rgba_dxt5_unorm.dds");
	//Filenames.push_back("kueken7_rgba_pvrtc2_4bpp_unorm.dds");
	Filenames.push_back("kueken7_rgba8_snorm.dds");
	Filenames.push_back("kueken7_rgba16_sfloat.dds");

	int Error(0);

	std::clock_t TimeFileStart = std::clock();
	{
		for(std::size_t Index = 0; Index < Filenames.size(); ++Index)
			Error += load_file::test(Filenames[Index]);
	}
	std::clock_t TimeFileEnd = std::clock();

	std::clock_t TimeMemStart = std::clock();
	{
		for(std::size_t Index = 0; Index < Filenames.size(); ++Index)
			Error += load_mem::test(Filenames[Index]);
	}
	std::clock_t TimeMemEnd = std::clock();

	std::clock_t TimeMemOnlyStart = 0;
	{
		std::vector<std::vector<char> > Memory(Filenames.size());

		for(std::size_t Index = 0; Index < Filenames.size(); ++Index)
		{
			FILE* File = std::fopen(Filenames[Index].c_str(), "rb");
			assert(File);

			long Beg = std::ftell(File);
			std::fseek(File, 0, SEEK_END);
			long End = std::ftell(File);
			std::fseek(File, 0, SEEK_SET);

			Memory[Index].resize(End - Beg);

			std::fread(&Memory[Index][0], 1, Memory[Index].size(), File);
			std::fclose(File);
		}

		TimeMemOnlyStart = std::clock();

		for(std::size_t Index = 0; Index < Filenames.size(); ++Index)
			Error += load_mem_only::test(Memory[Index], Filenames[Index]);
	}
	std::clock_t TimeMemOnlyEnd = std::clock();

	std::printf("File: %lu, Mem: %lu, Mem Only: %lu\n", TimeFileEnd - TimeFileStart, TimeMemEnd - TimeMemStart, TimeMemOnlyEnd - TimeMemOnlyStart);

	return Error;
}
