#include "atlas/tools/Grid.hpp"
#include "atlas/gl/Shader.hpp"

namespace atlas
{
    namespace tools
    {
        constexpr auto gridVs =
            R"(#version 430 core

layout (location = 0) in vec3 vPosition;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(vPosition, 1.0);
}
)";

        constexpr auto gridFS =
            R"(#version 430 core

uniform vec4 colour;

out vec4 fragColour;

void main()
{
    fragColour = colour;
}
)";

        Grid::Grid() :
            mBuffer(GL_ARRAY_BUFFER),
            mNumIndices(100)
        {
            mModel = math::Matrix4(1.0f);

            std::vector<math::Vector> vertices(mNumIndices);

            int n = (int)mNumIndices / 4;
            for (int i = 0; i < n; ++i)
            {
                vertices[4 * i + 0] = { -12.0f + i, 0.0f, -12.0f };
                vertices[4 * i + 1] = { -12.0f + i, 0.0f,  12.0f };
                vertices[4 * i + 2] = { -12.0f, 0.0f, -12.0f + i };
                vertices[4 * i + 3] = {  12.0f, 0.0f, -12.0f + i };
            }

            std::vector<gl::ShaderUnit> shaders
            {
                { gridVs, GL_VERTEX_SHADER, true },
                { gridFS, GL_FRAGMENT_SHADER, true }
            };

            mShaders.push_back(gl::Shader(shaders));
            mShaders[0].compileShaders();
            mShaders[0].linkShaders();

            mVao.bindVertexArray();
            mBuffer.bindBuffer();
            mBuffer.bufferData(gl::size<math::Vector>(mNumIndices),
                vertices.data(), GL_STATIC_DRAW);
            mBuffer.vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
                gl::bufferOffset<float>(0));
            mVao.enableVertexAttribArray(0);

            auto var = mShaders[0].getUniformVariable("projection");
            mUniforms.insert(UniformKey("projection", var));
            
            var = mShaders[0].getUniformVariable("view");
            mUniforms.insert(UniformKey("view", var));

            var = mShaders[0].getUniformVariable("model");
            mUniforms.insert(UniformKey("model", var));

            var = mShaders[0].getUniformVariable("colour");
            mUniforms.insert(UniformKey("colour", var));

            mBuffer.unBindBuffer();
            mVao.unBindVertexArray();
            mShaders[0].disableShaders();
        }

        void Grid::renderGeometry(math::Matrix4 const& projection, 
            math::Matrix4 const& view)
        {
            if (!mShaders[0].shaderProgramValid())
            {
                return;
            }

            mShaders[0].enableShaders();
            mVao.bindVertexArray();


            glUniformMatrix4fv(mUniforms["projection"], 1, GL_FALSE,
                &projection[0][0]);
            glUniformMatrix4fv(mUniforms["view"], 1, GL_FALSE, &view[0][0]);
            glUniformMatrix4fv(mUniforms["model"], 1, GL_FALSE, &mModel[0][0]);

            glUniform4f(mUniforms["colour"], 0, 0, 0, 1);
            int n = (int)(mNumIndices / 2) - 2;
            glDrawArrays(GL_LINES, n, 4);

            glPointSize(5.0f);
            float rgb = (64.0f / 255.0f);
            glUniform4f(mUniforms["colour"], rgb, rgb, rgb, 1);
            glDrawArrays(GL_LINES, 0, (int)mNumIndices);

            mVao.unBindVertexArray();
            mShaders[0].disableShaders();

        }
    }
}