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
/// @file gli/core/texture_2d_array.cpp
/// @date 2011-10-11 / 2013-11-25
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include <gli/gli.hpp>

namespace
{
	std::string path(std::string const & filename)
	{
		return std::string(SOURCE_DIR) + "/data/" + filename;
	}
}//namespace

namespace alloc
{
	int test()
	{
		int Error(0);

		std::vector<gli::format> Formats;
		Formats.push_back(gli::FORMAT_RGBA8_UNORM);
		Formats.push_back(gli::FORMAT_RGB8_UNORM);
		Formats.push_back(gli::FORMAT_R8_SNORM);
		Formats.push_back(gli::FORMAT_RGB_DXT1_UNORM);
		Formats.push_back(gli::FORMAT_RGB_BP_UNORM);
		Formats.push_back(gli::FORMAT_RGBA32_SFLOAT);

		std::vector<gli::texture2DArray::dim_type::value_type> Sizes;
		Sizes.push_back(16);
		Sizes.push_back(32);
		Sizes.push_back(15);
		Sizes.push_back(17);
		Sizes.push_back(1);

		for(std::size_t FormatIndex = 0; FormatIndex < Formats.size(); ++FormatIndex)
		for(std::size_t SizeIndex = 0; SizeIndex < Sizes.size(); ++SizeIndex)
		{
			gli::texture2DArray::dim_type Size(Sizes[SizeIndex]);

			gli::texture2DArray TextureA(Formats[FormatIndex], Size, 1, gli::levels(Size));
			gli::texture2DArray TextureB(Formats[FormatIndex], Size, 1);

			Error += TextureA == TextureB ? 0 : 1;
		}

		return Error;
	}
}//namespace alloc

namespace clear
{
	int test()
	{
		int Error(0);

		glm::u8vec4 const Orange(255, 127, 0, 255);

		gli::texture2DArray::dim_type Size(16u);
		gli::texture2DArray Texture(gli::FORMAT_RGBA8_UINT, Size, 2);

		Texture.clear<glm::u8vec4>(Orange);

		return Error;
	}
}//namespace clear

namespace query
{
	int test()
	{
		int Error(0);

		gli::texture2DArray::size_type Layers(2);
		gli::texture2DArray::size_type Levels(2);

		gli::texture2DArray Texture(gli::FORMAT_RGBA8_UINT, gli::texture2DArray::dim_type(2), Layers, Levels);

		gli::texture2DArray::size_type Size = Texture.size();

		Error += Size == sizeof(glm::u8vec4) * 5 * Layers ? 0 : 1;
		Error += Texture.format() == gli::FORMAT_RGBA8_UINT ? 0 : 1;
		Error += Texture.layers() == Layers ? 0 : 1;
		Error += Texture.levels() == Levels ? 0 : 1;
		Error += !Texture.empty() ? 0 : 1;
		Error += Texture.dimensions().x == 2 ? 0 : 1;
		Error += Texture.dimensions().y == 2 ? 0 : 1;

		return Error;
	}
}//namespace query

namespace access_map
{
	int test()
	{
		int Error(0);

		{
			gli::texture2DArray Texture2DArray(gli::FORMAT_RGBA8_UINT, gli::texture2DArray::dim_type(2), 2, 1);
			assert(!Texture2DArray.empty());

			gli::texture2D Texture0 = Texture2DArray[0];
			gli::texture2D Texture1 = Texture2DArray[1];
		
			std::size_t Size0 = Texture0.size();
			std::size_t Size1 = Texture1.size();

			Error += Size0 == sizeof(glm::u8vec4) * 4 ? 0 : 1;
			Error += Size1 == sizeof(glm::u8vec4) * 4 ? 0 : 1;

			for(std::size_t i = 0; i < 4; ++i)
			{
				*(Texture0.data<glm::u8vec4>() + i) = glm::u8vec4(255, 127, 0, 255);
				*(Texture1.data<glm::u8vec4>() + i) = glm::u8vec4(0, 127, 255, 255);
			}

			glm::u8vec4 * PointerA = Texture0.data<glm::u8vec4>();
			glm::u8vec4 * PointerB = Texture1.data<glm::u8vec4>();

			glm::u8vec4 * Pointer0 = Texture2DArray.data<glm::u8vec4>() + 0;
			glm::u8vec4 * Pointer1 = Texture2DArray.data<glm::u8vec4>() + 4;

			Error += PointerA == Pointer0 ? 0 : 1;
			Error += PointerB == Pointer1 ? 0 : 1;

			glm::u8vec4 ColorA = *Texture0.data<glm::u8vec4>();
			glm::u8vec4 ColorB = *Texture1.data<glm::u8vec4>();

			glm::u8vec4 Color0 = *Pointer0;
			glm::u8vec4 Color1 = *Pointer1;

			Error += glm::all(glm::equal(Color0, glm::u8vec4(255, 127, 0, 255))) ? 0 : 1;
			Error += glm::all(glm::equal(Color1, glm::u8vec4(0, 127, 255, 255))) ? 0 : 1;
		}

		return Error;
	}
}//namespace access_map

namespace size
{
	struct entry
	{
		entry(
			gli::format const & Format,
			gli::texture2DArray::dim_type const & Dimensions,
			gli::texture2DArray::size_type const & Size) :
				Format(Format),
				Dimensions(Dimensions),
				Size(Size)
		{}

		gli::format Format;
		gli::texture2DArray::dim_type Dimensions;
		gli::texture2DArray::size_type Size;
	};

	int test()
	{
		int Error(0);

		std::vector<entry> Tests;
		Tests.push_back(entry(gli::FORMAT_RGBA8_UINT, gli::texture2DArray::dim_type(4), 64 * 2));
		Tests.push_back(entry(gli::FORMAT_R8_UINT, gli::texture2DArray::dim_type(4), 16 * 2));
		Tests.push_back(entry(gli::FORMAT_RGB_DXT1_UNORM, gli::texture2DArray::dim_type(4), 8 * 2));
		Tests.push_back(entry(gli::FORMAT_RGB_DXT1_UNORM, gli::texture2DArray::dim_type(2), 8 * 2));
		Tests.push_back(entry(gli::FORMAT_RGB_DXT1_UNORM, gli::texture2DArray::dim_type(1), 8 * 2));
		Tests.push_back(entry(gli::FORMAT_RGBA_DXT5_UNORM, gli::texture2DArray::dim_type(4), 16 * 2));

		for(std::size_t i = 0; i < Tests.size(); ++i)
		{
			gli::texture2DArray Texture2DArray(
				Tests[i].Format,
				gli::texture2DArray::dim_type(4),
				gli::texture2DArray::size_type(2),
				gli::texture2DArray::size_type(1));

			Error += Texture2DArray.size() == Tests[i].Size ? 0 : 1;
			assert(!Error);
		}

		for(std::size_t i = 0; i < Tests.size(); ++i)
		{
			gli::texture2DArray Texture2DArray(
				Tests[i].Format,
				gli::texture2DArray::dim_type(4),
				gli::texture2DArray::size_type(2),
				gli::texture2DArray::size_type(1));

			gli::texture2D Texture2D = Texture2DArray[0];

			Error += Texture2DArray.size() == Tests[i].Size ? 0 : 1;
			assert(!Error);
		}

		return Error;
	}
}//namespace size

namespace loader
{
	int test()
	{
		int Error(0);

		gli::texture2DArray TextureArrayRGBA8(gli::FORMAT_RGBA8_UNORM, gli::texture2DArray::dim_type(1), 2, 1);

		{
			std::vector<glm::u8vec4> Color;
			Color.push_back(glm::u8vec4(255, 128,   0, 255));
			Color.push_back(glm::u8vec4(  0, 128, 255, 255));

			for(gli::texture2DArray::size_type LayerIndex = 0; LayerIndex < TextureArrayRGBA8.layers(); ++LayerIndex)
			for(gli::texture2DArray::size_type TexelIndex = 0; TexelIndex < TextureArrayRGBA8[LayerIndex].size<glm::u8vec4>(); ++TexelIndex)
				*(TextureArrayRGBA8[LayerIndex].data<glm::u8vec4>() + TexelIndex) = Color[LayerIndex];

			gli::save_dds(TextureArrayRGBA8, "texture2DArray_rgba8_unorm.dds");
		}

		{
			gli::texture2D Texture0(gli::FORMAT_RGBA8_UNORM, gli::texture2D::dim_type(1), 1);
			gli::texture2D Texture1(gli::FORMAT_RGBA8_UNORM, gli::texture2D::dim_type(1), 1);

			*Texture0.data<glm::u8vec4>() = glm::u8vec4(255, 128, 0, 255);
			*Texture1.data<glm::u8vec4>() = glm::u8vec4(  0, 128, 255, 255);

			gli::save_dds(Texture0, "texture2D0_rgba8_unorm.dds");
			gli::save_dds(Texture1, "texture2D1_rgba8_unorm.dds");
		}

		{
			gli::texture2DArray TextureArrayRGBA8Loaded(gli::load_dds("texture2DArray_rgba8_unorm.dds"));
			gli::texture2D Texture0(gli::load_dds("texture2D0_rgba8_unorm.dds"));
			gli::texture2D Texture1(gli::load_dds("texture2D1_rgba8_unorm.dds"));

			Error += TextureArrayRGBA8Loaded[0] == Texture0 ? 0 : 1;
			Error += TextureArrayRGBA8Loaded[1] == Texture1 ? 0 : 1;
		}

		return Error;
	}
}//namespace loader

namespace load
{
	int test()
	{
		int Error(0);

		{
			gli::texture const Texture = gli::load(path("array_r8_unorm.ktx"));
			Error += Texture.target() == gli::TARGET_2D_ARRAY ? 0 : 1;
		}

		{
			gli::texture const Texture = gli::load(path("array_r8_unorm.dds"));
			Error += Texture.target() == gli::TARGET_2D_ARRAY ? 0 : 1;
		}

		return Error;
	}
}//namespace load

int main()
{
	int Error(0);

	Error += alloc::test();
	Error += size::test();
	Error += query::test();
	Error += clear::test();
	Error += access_map::test();
	Error += loader::test();
	Error += load::test();

	return Error;
}

