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
/// @file gli/core/texture_cube.cpp
/// @date 2011-01-10 / 2013-11-25
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include <gli/gli.hpp>

int test_alloc()
{
	int Error(0);

	std::vector<gli::format> Formats;
	Formats.push_back(gli::FORMAT_RGBA8_UNORM);
	Formats.push_back(gli::FORMAT_RGB8_UNORM);
	Formats.push_back(gli::FORMAT_R8_SNORM);
	Formats.push_back(gli::FORMAT_RGB_DXT1_UNORM);
	Formats.push_back(gli::FORMAT_RGB_BP_UNORM);
	Formats.push_back(gli::FORMAT_RGBA32_SFLOAT);

	std::vector<gli::textureCube::dim_type::value_type> Sizes;
	Sizes.push_back(16);
	Sizes.push_back(32);
	Sizes.push_back(15);
	Sizes.push_back(17);
	Sizes.push_back(1);

	for(std::size_t FormatIndex = 0; FormatIndex < Formats.size(); ++FormatIndex)
	for(std::size_t SizeIndex = 0; SizeIndex < Sizes.size(); ++SizeIndex)
	{
		gli::textureCube::dim_type Size(Sizes[SizeIndex]);

		gli::textureCube TextureA(Formats[FormatIndex], Size, gli::levels(Size));

		gli::textureCube TextureB(Formats[FormatIndex], Size);

		Error += TextureA == TextureB ? 0 : 1;
	}

	return Error;
}

int test_textureCube_query()
{
	int Error(0);

	gli::textureCube Texture(gli::FORMAT_RGBA8_UINT, gli::textureCube::dim_type(2), 2);

	Error += Texture.size() == sizeof(glm::u8vec4) * 5 * 6 ? 0 : 1;
	Error += Texture.format() == gli::FORMAT_RGBA8_UINT ? 0 : 1;
	Error += Texture.levels() == 2 ? 0 : 1;
	Error += !Texture.empty() ? 0 : 1;
	Error += Texture.dimensions().x == 2 ? 0 : 1;
	Error += Texture.dimensions().y == 2 ? 0 : 1;

	return Error;
}

int test_textureCube_texture2D_access()
{
	int Error(0);

	{
		gli::texture2D Texture2DA(gli::FORMAT_RGBA8_UINT, gli::texture2D::dim_type(2, 2), 1);
		for(std::size_t i = 0; i < Texture2DA.size(); ++i)
			*(Texture2DA.data<glm::byte>() + i) = glm::byte(i);

		gli::texture2D Texture2DB(gli::FORMAT_RGBA8_UINT, gli::texture2D::dim_type(2, 2), 1);
		for(std::size_t i = 0; i < Texture2DB.size(); ++i)
			*(Texture2DB.data<glm::byte>() + i) = glm::byte(i + 100);

		gli::textureCube TextureCube(gli::FORMAT_RGBA8_UINT, gli::textureCube::dim_type(2), 2);

		/// Todo
		/// gli::copy(TextureCube, 0, Texture2DA);
		/// gli::copy(TextureCube, 1, Texture2DB);

		/// Error += TextureCube[0] == Texture2DA ? 0 : 1;
		/// Error += TextureCube[1] == Texture2DB ? 0 : 1;
	}

	{
		gli::textureCube TextureCube(gli::FORMAT_RGBA8_UINT, gli::textureCube::dim_type(2), 1);
		assert(!TextureCube.empty());

		std::vector<glm::u8vec4> Colors;
		Colors.push_back(glm::u8vec4(255,   0,   0, 255));
		Colors.push_back(glm::u8vec4(255, 255,   0, 255));
		Colors.push_back(glm::u8vec4(  0, 255,   0, 255));
		Colors.push_back(glm::u8vec4(  0, 255, 255, 255));
		Colors.push_back(glm::u8vec4(  0,   0, 255, 255));
		Colors.push_back(glm::u8vec4(255, 255,   0, 255));

		for(std::size_t ColorIndex = 0; ColorIndex < Colors.size(); ++ColorIndex)
		{
			gli::texture2D Texture2D = TextureCube[ColorIndex];
			for(std::size_t PixelIndex = 0; PixelIndex < 4; ++PixelIndex)
			{
				glm::u8vec4 Color = Colors[ColorIndex];
				*(Texture2D.data<glm::u8vec4>() + PixelIndex) = Color;
			}
		}

		for(std::size_t TexelIndex = 0; TexelIndex < TextureCube.size() / sizeof(glm::u8vec4); ++TexelIndex)
			Error += glm::all(glm::equal(*(TextureCube.data<glm::u8vec4>() + TexelIndex), Colors[TexelIndex / 4])) ? 0 : 1;
	}

	{
		gli::textureCube TextureCube(gli::FORMAT_RGBA8_UINT, gli::textureCube::dim_type(2), 2);
		assert(!TextureCube.empty());

		gli::texture2D TextureA = TextureCube[0];
		gli::texture2D TextureB = TextureCube[1];
		
		std::size_t Size0 = TextureA.size();
		std::size_t Size1 = TextureB.size();

		Error += Size0 == sizeof(glm::u8vec4) * 5 ? 0 : 1;
		Error += Size1 == sizeof(glm::u8vec4) * 5 ? 0 : 1;

		*TextureA.data<glm::u8vec4>() = glm::u8vec4(255, 127, 0, 255);
		*TextureB.data<glm::u8vec4>() = glm::u8vec4(0, 127, 255, 255);

		glm::u8vec4 * PointerA = TextureA.data<glm::u8vec4>();
		glm::u8vec4 * PointerB = TextureB.data<glm::u8vec4>();

		glm::u8vec4 * Pointer0 = TextureCube.data<glm::u8vec4>() + 0;
		glm::u8vec4 * Pointer1 = TextureCube.data<glm::u8vec4>() + 5;

		Error += PointerA == Pointer0 ? 0 : 1;
		Error += PointerB == Pointer1 ? 0 : 1;

		glm::u8vec4 ColorA = *TextureA.data<glm::u8vec4>();
		glm::u8vec4 ColorB = *TextureB.data<glm::u8vec4>();

		glm::u8vec4 Color0 = *Pointer0;
		glm::u8vec4 Color1 = *Pointer1;

		Error += glm::all(glm::equal(Color0, glm::u8vec4(255, 127, 0, 255))) ? 0 : 1;
		Error += glm::all(glm::equal(Color1, glm::u8vec4(0, 127, 255, 255))) ? 0 : 1;
	}

	{
		gli::textureCube TextureCube(
			gli::FORMAT_RGBA8_UINT,
			gli::textureCube::dim_type(2),
			1);

		std::size_t SizeA = TextureCube.size();
		Error += SizeA == sizeof(glm::u8vec4) * 4 * 6 ? 0 : 1;

		gli::texture2D Texture2D = TextureCube[0];
		
		std::size_t Size0 = Texture2D.size();
		Error += Size0 == sizeof(glm::u8vec4) * 4 ? 0 : 1;

		*Texture2D.data<glm::u8vec4>() = glm::u8vec4(255, 127, 0, 255);

		glm::u8vec4 * PointerA = Texture2D.data<glm::u8vec4>();
		glm::u8vec4 * Pointer0 = TextureCube.data<glm::u8vec4>();
		Error += PointerA == Pointer0 ? 0 : 1;

		glm::u8vec4 ColorA = *PointerA;
		glm::u8vec4 Color0 = *Pointer0;
		Error += glm::all(glm::equal(ColorA, glm::u8vec4(255, 127, 0, 255))) ? 0 : 1;
		Error += glm::all(glm::equal(Color0, glm::u8vec4(255, 127, 0, 255))) ? 0 : 1;
	}

	return Error;
}

struct test
{
	test(
		gli::format const & Format,
		gli::textureCube::dim_type const & Dimensions,
		gli::textureCube::size_type const & Size) :
		Format(Format),
		Dimensions(Dimensions),
		Size(Size)
	{}

	gli::format Format;
	gli::textureCube::dim_type Dimensions;
	gli::textureCube::size_type Size;
};

int test_textureCube_texture2D_size()
{
	int Error(0);

	std::vector<test> Tests;
	Tests.push_back(test(gli::FORMAT_RGBA8_UINT, gli::textureCube::dim_type(4), 384));
	Tests.push_back(test(gli::FORMAT_R8_UINT, gli::textureCube::dim_type(4), 96));
	Tests.push_back(test(gli::FORMAT_RGBA_DXT1_UNORM, gli::textureCube::dim_type(4), 48));
	Tests.push_back(test(gli::FORMAT_RGBA_DXT1_UNORM, gli::textureCube::dim_type(2), 48));
	Tests.push_back(test(gli::FORMAT_RGBA_DXT1_UNORM, gli::textureCube::dim_type(1), 48));
	Tests.push_back(test(gli::FORMAT_RGBA_DXT5_UNORM, gli::textureCube::dim_type(4), 96));

	for(std::size_t i = 0; i < Tests.size(); ++i)
	{
		gli::textureCube Texture(
			Tests[i].Format,
			gli::textureCube::dim_type(4),
			gli::textureCube::size_type(1));

		gli::textureCube::size_type Size = Texture.size();
		Error += Size == Tests[i].Size ? 0 : 1;
		assert(!Error);
	}

	return Error;
}

namespace clear
{
	int test()
	{
		int Error(0);

		glm::u8vec4 const Orange(255, 127, 0, 255);

		gli::textureCube Texture(gli::FORMAT_RGBA8_UINT, gli::textureCube::dim_type(4), 1);

		Texture.clear<glm::u8vec4>(Orange);

		return Error;
	}
}//namespace clear

namespace loader
{
	int test()
	{
		int Error(0);

		gli::textureCube TextureA(gli::FORMAT_RGBA8_UNORM, gli::textureCube::dim_type(256), 1);

		{
			std::vector<glm::u8vec4> Color;
			Color.push_back(glm::u8vec4(255,   0,   0, 255));
			Color.push_back(glm::u8vec4(255, 128,   0, 255));
			Color.push_back(glm::u8vec4(255, 255,   0, 255));
			Color.push_back(glm::u8vec4(  0, 255,   0, 255));
			Color.push_back(glm::u8vec4(  0, 128, 255, 255));
			Color.push_back(glm::u8vec4(  0,   0, 255, 255));

			for(gli::textureCube::size_type FaceIndex = 0; FaceIndex < TextureA.faces(); ++FaceIndex)
			for(gli::texture2D::size_type TexelIndex = 0; TexelIndex < TextureA[FaceIndex].size<glm::u8vec4>(); ++TexelIndex)
				*(TextureA[FaceIndex].data<glm::u8vec4>() + TexelIndex) = Color[FaceIndex];

			gli::save_dds(TextureA, "textureCubeA_rgba8_unorm.dds");
		}

		{
			gli::textureCube TextureB(gli::load_dds("textureCubeA_rgba8_unorm.dds"));
			gli::save_dds(TextureB, "textureCubeB_rgba8_unorm.dds");
			gli::textureCube TextureC(gli::load_dds("textureCubeB_rgba8_unorm.dds"));

			Error += TextureA == TextureB ? 0 : 1;
			Error += TextureA == TextureC ? 0 : 1;
			Error += TextureB == TextureC ? 0 : 1;
		}

		return Error;
	}
}//namespace loader

int main()
{
	int Error(0);

	Error += loader::test();
	Error += test_alloc();
	Error += test_textureCube_texture2D_size();
	Error += test_textureCube_query();
	Error += test_textureCube_texture2D_access();
	Error += clear::test();

	return Error;
}

