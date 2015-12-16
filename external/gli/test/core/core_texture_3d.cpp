///////////////////////////////////////////////////////////////////////////////////
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
/// @file test/core/texture_3d.cpp
/// @date 2013-01-13 / 2013-01-13
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

	std::vector<gli::texture3D::dim_type::value_type> Sizes;
	Sizes.push_back(16);
	Sizes.push_back(32);
	Sizes.push_back(15);
	Sizes.push_back(17);
	Sizes.push_back(1);

	for(std::size_t FormatIndex = 0; FormatIndex < Formats.size(); ++FormatIndex)
	for(std::size_t SizeIndex = 0; SizeIndex < Sizes.size(); ++SizeIndex)
	{
		gli::texture3D::dim_type Size(Sizes[SizeIndex]);

		gli::texture3D TextureA(Formats[FormatIndex], Size, gli::levels(Size));
		gli::texture3D TextureB(Formats[FormatIndex], Size);

		Error += TextureA == TextureB ? 0 : 1;
	}

	return Error;
}

int test_texture3d_clear()
{
	int Error(0);

	glm::u8vec4 const Orange(255, 127, 0, 255);

	gli::texture3D::dim_type Size(16);

	gli::texture3D Texture(gli::FORMAT_RGBA8_UINT, Size);

	Texture.clear<glm::u8vec4>(Orange);

	return Error;
}

int test_texture3d_query()
{
	int Error(0);

	gli::texture3D::dim_type Size(2);

	gli::texture3D Texture(gli::FORMAT_RGBA8_UINT, Size);

	Error += Texture.size() == sizeof(glm::u8vec4) * 9 ? 0 : 1;
	Error += Texture.format() == gli::FORMAT_RGBA8_UINT ? 0 : 1;
	Error += Texture.levels() == 2 ? 0 : 1;
	Error += !Texture.empty() ? 0 : 1;
	Error += Texture.dimensions().x == 2 ? 0 : 1;
	Error += Texture.dimensions().y == 2 ? 0 : 1;
	Error += Texture.dimensions().z == 2 ? 0 : 1;

	return Error;
}

int test_texture3d_access()
{
	int Error(0);

	{
		glm::u8vec4 const Orange(255, 127, 0, 255);

		gli::image Image(gli::FORMAT_RGBA8_UINT, gli::image::dim_type(2));

		gli::texture3D Texture(gli::FORMAT_RGBA8_UINT, gli::texture3D::dim_type(2), gli::texture3D::size_type(1));

		Error += Image.size() == Texture.size() ? 0 : 1;
	}

	{
		gli::texture3D Texture(
			gli::FORMAT_RGBA8_UINT,
			gli::texture3D::dim_type(2),
			gli::texture3D::size_type(2));
		assert(!Texture.empty());

		gli::image Image0 = Texture[0];
		gli::image Image1 = Texture[1];
		
		std::size_t Size0 = Image0.size();
		std::size_t Size1 = Image1.size();

		Error += Size0 == sizeof(glm::u8vec4) * 8 ? 0 : 1;
		Error += Size1 == sizeof(glm::u8vec4) * 1 ? 0 : 1;

		*Image0.data<glm::u8vec4>() = glm::u8vec4(255, 127, 0, 255);
		*Image1.data<glm::u8vec4>() = glm::u8vec4(0, 127, 255, 255);

		glm::u8vec4 * PointerA = Image0.data<glm::u8vec4>();
		glm::u8vec4 * PointerB = Image1.data<glm::u8vec4>();

		glm::u8vec4 * Pointer0 = Texture.data<glm::u8vec4>() + 0;
		glm::u8vec4 * Pointer1 = Texture.data<glm::u8vec4>() + 8;

		Error += PointerA == Pointer0 ? 0 : 1;
		Error += PointerB == Pointer1 ? 0 : 1;

		glm::u8vec4 ColorA = *Image0.data<glm::u8vec4>();
		glm::u8vec4 ColorB = *Image1.data<glm::u8vec4>();

		glm::u8vec4 Color0 = *Pointer0;
		glm::u8vec4 Color1 = *Pointer1;

		Error += glm::all(glm::equal(Color0, glm::u8vec4(255, 127, 0, 255))) ? 0 : 1;
		Error += glm::all(glm::equal(Color1, glm::u8vec4(0, 127, 255, 255))) ? 0 : 1;
	}

	return Error;
}

struct test
{
	test(
		gli::format const & Format,
		gli::texture3D::dim_type const & Dimensions,
		gli::texture3D::size_type const & Size) :
		Format(Format),
		Dimensions(Dimensions),
		Size(Size)
	{}

	gli::format Format;
	gli::texture3D::dim_type Dimensions;
	gli::texture3D::size_type Size;
};

int test_texture3d_size()
{
	int Error(0);

	std::vector<test> Tests;
	Tests.push_back(test(gli::FORMAT_RGBA8_UINT, gli::texture3D::dim_type(4), 256));
	Tests.push_back(test(gli::FORMAT_R8_UINT, gli::texture3D::dim_type(4), 64));
	Tests.push_back(test(gli::FORMAT_RGBA_DXT1_UNORM, gli::texture3D::dim_type(4), 32));
	Tests.push_back(test(gli::FORMAT_RGBA_DXT1_UNORM, gli::texture3D::dim_type(2), 32));
	Tests.push_back(test(gli::FORMAT_RGBA_DXT1_UNORM, gli::texture3D::dim_type(1), 32));
	Tests.push_back(test(gli::FORMAT_RGBA_DXT5_UNORM, gli::texture3D::dim_type(4), 64));

	for(std::size_t i = 0; i < Tests.size(); ++i)
	{
		gli::texture3D Texture(
			Tests[i].Format,
			gli::texture3D::dim_type(4),
			1);

		Error += Texture.size() == Tests[i].Size ? 0 : 1;
		assert(!Error);
	}

	for(std::size_t i = 0; i < Tests.size(); ++i)
	{
		gli::texture3D Texture(
			Tests[i].Format,
			gli::texture3D::dim_type(4),
			1);

		gli::image Image = Texture[0];

		Error += Image.size() == Tests[i].Size ? 0 : 1;
		assert(!Error);
	}

	return Error;
}

int main()
{
	int Error(0);

	Error += test_alloc();
	Error += test_texture3d_size();
	Error += test_texture3d_query();
	Error += test_texture3d_clear();
	Error += test_texture3d_access();

	return Error;
}

