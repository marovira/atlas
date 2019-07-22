#include "atlas/gui/GUI.hpp"

#include <atlas/core/FMT.hpp>
#include <atlas/core/Platform.hpp>
#include <atlas/glx/Buffer.hpp>
#include <atlas/glx/GLSL.hpp>

#if defined(ATLAS_PLATFORM_WINDOWS)
#    define GLFW_EXPOSE_NATIVE_WIN32
#    define GLFW_EXPOSE_NATIVE_WGL
#endif
#include <GLFW/glfw3native.h>

#include <array>

namespace atlas::gui
{
    void renderDrawData(GuiRenderData const& data, ImDrawData* drawData);
    bool createFontsTexture(GuiRenderData& data);
    void destroyFontsTexture(GuiRenderData& data);
    bool createDeviceObjects(GuiRenderData& data);
    void destroyDeviceObjects(GuiRenderData& data);

    bool initializeGUI(GuiRenderData& data)
    {
        auto& io               = ImGui::GetIO();
        io.BackendRendererName = "atlas_opengl";
        createDeviceObjects(data);
        return true;
    }

    void shutdownGUI(GuiRenderData& data)
    {
        destroyDeviceObjects(data);
    }

    void renderGUIFrame(GuiRenderData const& data)
    {
        renderDrawData(data, ImGui::GetDrawData());
    }

    static void setupRenderState(GuiRenderData const& renderData,
                                 ImDrawData* drawData, int fbWidth,
                                 int fbHeight, GLuint vao)
    {
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_SCISSOR_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glViewport(0, 0, static_cast<GLsizei>(fbWidth),
                   static_cast<GLsizei>(fbHeight));
        float L = drawData->DisplayPos.x;
        float R = drawData->DisplayPos.x + drawData->DisplaySize.x;
        float T = drawData->DisplayPos.y;
        float B = drawData->DisplayPos.y + drawData->DisplaySize.y;
        // clang-format off
        const std::array<std::array<float, 4>, 4> orthoProjection{{
            {{2.0f / (R - L),    0.0f,               0.0f,  0.0f}},
            {{0.0f,              2.0f / (T - B),     0.0f,  0.0f}},
            {{0.0f,              0.0f,              -1.0f,  0.0f}},
            {{(R + L) / (L - R), (T + B) / (B - T), 0.0f,   1.0f}}
        }};
        // clang-format on

        glUseProgram(renderData.shaderHandle);
        glUniform1i(renderData.texAttribLocation, 0);
        glUniformMatrix4fv(renderData.projMtxAttribLocation, 1, GL_FALSE,
                           &orthoProjection[0][0]);
        glBindSampler(0, 0);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, renderData.vboHandle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderData.elementsHandle);
        glEnableVertexAttribArray(renderData.vtxPosAttribLocation);
        glEnableVertexAttribArray(renderData.vtxUVAttribLocation);
        glEnableVertexAttribArray(renderData.vtxColorAttribLocation);
        glVertexAttribPointer(
            renderData.vtxPosAttribLocation, 2, GL_FLOAT, GL_FALSE,
            sizeof(ImDrawVert),
            reinterpret_cast<GLvoid*>(IM_OFFSETOF(ImDrawVert, pos)));
        glVertexAttribPointer(
            renderData.vtxUVAttribLocation, 2, GL_FLOAT, GL_FALSE,
            sizeof(ImDrawVert),
            reinterpret_cast<GLvoid*>(IM_OFFSETOF(ImDrawVert, uv)));
        glVertexAttribPointer(
            renderData.vtxColorAttribLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE,
            sizeof(ImDrawVert),
            reinterpret_cast<GLvoid*>(IM_OFFSETOF(ImDrawVert, col)));
    }

    void renderDrawData(GuiRenderData const& renderData, ImDrawData* drawData)
    {
        int fbWidth  = static_cast<int>(drawData->DisplaySize.x *
                                       drawData->FramebufferScale.x);
        int fbHeight = static_cast<int>(drawData->DisplaySize.y *
                                        drawData->FramebufferScale.y);
        if (fbWidth <= 0 || fbHeight <= 0)
        {
            return;
        }

        GLint lastActiveTexture;
        glGetIntegerv(GL_ACTIVE_TEXTURE, &lastActiveTexture);
        glActiveTexture(GL_TEXTURE0);

        GLint lastProgram;
        glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram);
        GLint lastTexture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);
        GLint lastSampler;
        glGetIntegerv(GL_SAMPLER_BINDING, &lastSampler);
        GLint lastArrayBuffer;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &lastArrayBuffer);
        GLint lastVertexArrayObject;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &lastVertexArrayObject);

        std::array<GLint, 2> lastPolygonMode;
        glGetIntegerv(GL_POLYGON_MODE, lastPolygonMode.data());
        std::array<GLint, 4> lastViewport;
        glGetIntegerv(GL_VIEWPORT, lastViewport.data());
        std::array<GLint, 4> lastScissorBox;
        glGetIntegerv(GL_VIEWPORT, lastScissorBox.data());

        GLint lastBlendSrcRGB;
        glGetIntegerv(GL_BLEND_SRC_RGB, &lastBlendSrcRGB);
        GLint lastBlendDstRGB;
        glGetIntegerv(GL_BLEND_DST_RGB, &lastBlendDstRGB);
        GLint lastBlendSrcAlpha;
        glGetIntegerv(GL_BLEND_SRC_ALPHA, &lastBlendSrcAlpha);
        GLint lastBlendDstAlpha;
        glGetIntegerv(GL_BLEND_DST_ALPHA, &lastBlendDstAlpha);
        GLint lastBlendEquationRGB;
        glGetIntegerv(GL_BLEND_EQUATION_RGB, &lastBlendEquationRGB);
        GLint lastBlendEquationAlpha;
        glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &lastBlendEquationAlpha);

        GLboolean lastEnableBlend       = glIsEnabled(GL_BLEND);
        GLboolean lastEnableCullFace    = glIsEnabled(GL_CULL_FACE);
        GLboolean lastEnableDepthTest   = glIsEnabled(GL_DEPTH_TEST);
        GLboolean lastEnableScissorTest = glIsEnabled(GL_SCISSOR_TEST);

        GLint lastClipOrigin{0};
        glGetIntegerv(GL_CLIP_ORIGIN, &lastClipOrigin);
        bool clipOriginLowerLeft = (lastClipOrigin != GL_UPPER_LEFT);

        GLuint vao{0};
        glGenVertexArrays(1, &vao);
        setupRenderState(renderData, drawData, fbWidth, fbHeight, vao);

        ImVec2 clipOff   = drawData->DisplayPos;
        ImVec2 clipScale = drawData->FramebufferScale;

        for (int n{0}; n < drawData->CmdListsCount; ++n)
        {
            const ImDrawList* cmdList = drawData->CmdLists[n];

            glBufferData(GL_ARRAY_BUFFER,
                         glx::size<ImDrawVert>(cmdList->VtxBuffer.Size),
                         static_cast<const GLvoid*>(cmdList->VtxBuffer.Data),
                         GL_STREAM_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         glx::size<ImDrawIdx>(cmdList->IdxBuffer.Size),
                         static_cast<const GLvoid*>(cmdList->IdxBuffer.Data),
                         GL_STREAM_DRAW);

            for (int cmdI{0}; cmdI < cmdList->CmdBuffer.Size; ++cmdI)
            {
                const ImDrawCmd* pcmd = &cmdList->CmdBuffer[cmdI];
                if (pcmd->UserCallback != nullptr)
                {
                    if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                    {
                        setupRenderState(renderData, drawData, fbWidth,
                                         fbHeight, vao);
                    }
                    else
                    {
                        pcmd->UserCallback(cmdList, pcmd);
                    }
                }
                else
                {
                    ImVec4 clipRect;
                    clipRect.x = (pcmd->ClipRect.x - clipOff.x) * clipScale.x;
                    clipRect.y = (pcmd->ClipRect.y - clipOff.y) * clipScale.y;
                    clipRect.z = (pcmd->ClipRect.z - clipOff.x) * clipScale.x;
                    clipRect.w = (pcmd->ClipRect.w - clipOff.y) * clipScale.y;

                    if (clipRect.x < fbWidth && clipRect.y < fbHeight &&
                        clipRect.z >= 0.0f && clipRect.w >= 0.0f)
                    {
                        if (clipOriginLowerLeft)
                        {
                            glScissor(
                                static_cast<int>(clipRect.x),
                                static_cast<int>(fbHeight - clipRect.w),
                                static_cast<int>(clipRect.z - clipRect.x),
                                static_cast<int>(clipRect.w - clipRect.y));
                        }
                        else
                        {
                            glScissor(static_cast<int>(clipRect.x),
                                      static_cast<int>(clipRect.y),
                                      static_cast<int>(clipRect.z),
                                      static_cast<int>(clipRect.w));
                        }
                        glBindTexture(
                            GL_TEXTURE_2D,
                            static_cast<GLuint>(
                                reinterpret_cast<intptr_t>(pcmd->TextureId)));
                        glDrawElementsBaseVertex(
                            GL_TRIANGLES, static_cast<GLsizei>(pcmd->ElemCount),
                            sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT
                                                   : GL_UNSIGNED_INT,
                            glx::bufferOffset<ImDrawIdx>(pcmd->IdxOffset),
                            static_cast<GLint>(pcmd->VtxOffset));
                    }
                }
            }
        }

        glDeleteVertexArrays(1, &vao);

        glUseProgram(lastProgram);
        glBindTexture(GL_TEXTURE_2D, lastTexture);
        glBindSampler(0, lastSampler);
        glActiveTexture(lastActiveTexture);
        glBindBuffer(GL_ARRAY_BUFFER, lastArrayBuffer);
        glBindVertexArray(lastVertexArrayObject);

        glBlendEquationSeparate(lastBlendEquationRGB, lastBlendEquationAlpha);
        glBlendFuncSeparate(lastBlendSrcRGB, lastBlendDstRGB, lastBlendSrcAlpha,
                            lastBlendDstAlpha);

        if (lastEnableBlend != 0u)
        {
            glEnable(GL_BLEND);
        }
        else
        {
            glDisable(GL_BLEND);
        }

        if (lastEnableCullFace != 0u)
        {
            glEnable(GL_CULL_FACE);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }

        if (lastEnableDepthTest != 0u)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }

        if (lastEnableScissorTest != 0u)
        {
            glEnable(GL_SCISSOR_TEST);
        }
        else
        {
            glDisable(GL_SCISSOR_TEST);
        }

        glPolygonMode(GL_FRONT_AND_BACK,
                      static_cast<GLenum>(lastPolygonMode[0]));
        glViewport(lastViewport[0], lastViewport[1],
                   static_cast<GLsizei>(lastViewport[2]),
                   static_cast<GLsizei>(lastViewport[3]));
        glScissor(lastScissorBox[0], lastScissorBox[1],
                  static_cast<GLsizei>(lastScissorBox[2]),
                  static_cast<GLsizei>(lastScissorBox[3]));
    }

    bool createFontsTexture(GuiRenderData& data)
    {
        auto& io = ImGui::GetIO();
        unsigned char* pixels;
        int width;
        int height;
        io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

        GLint lastTexture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);
        glGenTextures(1, &data.fontTexture);
        glBindTexture(GL_TEXTURE_2D, data.fontTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, pixels);

        io.Fonts->TexID = reinterpret_cast<ImTextureID>(
            static_cast<intptr_t>(data.fontTexture));

        glBindTexture(GL_TEXTURE_2D, lastTexture);
        return true;
    }

    void destroyFontsTexture(GuiRenderData& data)
    {
        if (data.fontTexture != 0u)
        {
            auto& io = ImGui::GetIO();
            glDeleteTextures(1, &data.fontTexture);
            io.Fonts->TexID  = nullptr;
            data.fontTexture = 0;
        }
    }

    bool createDeviceObjects(GuiRenderData& data)
    {
        GLint lastTexture;
        GLint lastArrayBuffer;
        GLint lastVertexArray;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &lastArrayBuffer);
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &lastVertexArray);

        const GLchar* vertexShader =
            "#version 450 core"
            "layout (location = 0) in vec2 Position;\n"
            "layout (location = 1) in vec2 UV;\n"
            "layout (location = 2) in vec4 Color;\n"
            "uniform mat4 ProjMtx;\n"
            "out vec2 Frag_UV;\n"
            "out vec4 Frag_Color;\n"
            "void main()\n"
            "{\n"
            "    Frag_UV = UV;\n"
            "    Frag_Color = Color;\n"
            "    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
            "}\n";
        const GLchar* fragmentShader =
            "#version 450 core"
            "in vec2 Frag_UV;\n"
            "in vec4 Frag_Color;\n"
            "uniform sampler2D Texture;\n"
            "layout (location = 0) out vec4 Out_Color;\n"
            "void main()\n"
            "{\n"
            "    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
            "}\n";

        data.vertHandle = glCreateShader(GL_VERTEX_SHADER);
        if (auto ret = glx::compileShader(vertexShader, data.vertHandle); ret)
        {
            fmt::print(stderr,
                       "error: GUI vertex shader failed to compile: {}\n",
                       ret.value());
            return false;
        }

        data.fragHandle = glCreateShader(GL_FRAGMENT_SHADER);
        if (auto ret = glx::compileShader(fragmentShader, data.fragHandle); ret)
        {
            fmt::print(stderr,
                       "error: GUI fragment shader failed to compile: {}\n",
                       ret.value());
            return false;
        }

        data.shaderHandle = glCreateProgram();
        glAttachShader(data.shaderHandle, data.vertHandle);
        glAttachShader(data.shaderHandle, data.fragHandle);
        if (auto ret = glx::linkShaders(data.shaderHandle); ret)
        {
            fmt::print(stderr, "error: GUI shader program failed to link: {}\n",
                       ret.value());
            return false;
        }

        auto& handle                = data.shaderHandle;
        data.texAttribLocation      = glGetUniformLocation(handle, "Texture");
        data.projMtxAttribLocation  = glGetUniformLocation(handle, "ProjMtx");
        data.vtxPosAttribLocation   = glGetUniformLocation(handle, "Position");
        data.vtxUVAttribLocation    = glGetUniformLocation(handle, "UV");
        data.vtxColorAttribLocation = glGetUniformLocation(handle, "Color");

        glGenBuffers(1, &data.vboHandle);
        glGenBuffers(1, &data.elementsHandle);

        createFontsTexture(data);

        glBindTexture(GL_TEXTURE_2D, lastTexture);
        glBindBuffer(GL_ARRAY_BUFFER, lastArrayBuffer);
        glBindVertexArray(lastVertexArray);

        return true;
    }

    void destroyDeviceObjects(GuiRenderData& data)
    {
        if (data.vboHandle != 0u)
        {
            glDeleteBuffers(1, &data.vboHandle);
            data.vboHandle = 0;
        }

        if (data.elementsHandle != 0u)
        {
            glDeleteBuffers(1, &data.elementsHandle);
            data.elementsHandle = 0;
        }

        if (data.shaderHandle != 0u && data.vertHandle != 0u)
        {
            glDetachShader(data.shaderHandle, data.vertHandle);
        }
        if (data.vertHandle != 0u)
        {
            glDeleteShader(data.vertHandle);
            data.vertHandle = 0;
        }

        if (data.shaderHandle != 0u && data.fragHandle != 0u)
        {
            glDetachShader(data.shaderHandle, data.fragHandle);
        }
        if (data.fragHandle != 0u)
        {
            glDeleteShader(data.fragHandle);
            data.fragHandle = 0;
        }

        if (data.shaderHandle != 0u)
        {
            glDeleteProgram(data.shaderHandle);
            data.shaderHandle = 0;
        }

        destroyFontsTexture(data);
    }

} // namespace atlas::gui
