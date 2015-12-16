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
/// @file gli/core/generate_mipmaps.cpp
/// @date 2013-11-25 / 2013-11-25
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#include <gli/gli.hpp>
#include <gli/core/generate_mipmaps.hpp>

namespace texture2d
{
	int test()
	{
		int Error(0);

		{
			gli::texture2D Texture(gli::FORMAT_RGB8_UNORM, gli::texture2D::dim_type(2, 2));
			gli::texel_write<glm::u8vec3>(Texture, gli::texture2D::dim_type(0, 0), 0, glm::u8vec3(255,   0,   0));
			gli::texel_write<glm::u8vec3>(Texture, gli::texture2D::dim_type(1, 0), 0, glm::u8vec3(255, 255,   0));
			gli::texel_write<glm::u8vec3>(Texture, gli::texture2D::dim_type(1, 1), 0, glm::u8vec3(  0, 255,   0));
			gli::texel_write<glm::u8vec3>(Texture, gli::texture2D::dim_type(0, 1), 0, glm::u8vec3(  0,   0, 255));
			
			gli::generate_mipmaps(Texture);

			gli::texture2D::size_type const Level(Texture.levels() - 1);
			glm::u8vec3 const Color(*Texture[Level].data<glm::u8vec3>());
			
			Error += glm::all(glm::equal(glm::u8vec3(0), Color)) ? 0 : 1;
		}

		{
			gli::texture2D Texture(gli::FORMAT_RGB8_UNORM, gli::texture2D::dim_type(8, 8));
			Texture.clear(glm::u8vec3(255, 128, 0));

			gli::texture2D::size_type const Level(Texture.levels() - 1);
			Error += Level == 3 ? 0 : 1;
		}

		{
			gli::texture2D Texture(gli::FORMAT_RGBA8_UNORM, gli::texture2D::dim_type(8, 8));
			Texture.clear(glm::u8vec4(255, 128, 0, 255));

			gli::generate_mipmaps(Texture);

			gli::texture2D::size_type const Level(Texture.levels() - 1);
			glm::u8vec4 const Color(*Texture[Level].data<glm::u8vec4>());

			Error += glm::all(glm::equal(glm::u8vec4(255, 128, 0, 255), Color)) ? 0 : 1;
		}

		return Error;
	}
}//namespace test

int main()
{
	int Error(0);

	//Error += texture2d::test();

	return Error;
}
