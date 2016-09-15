#include "atlas/utils/GUI.hpp"
#include "atlas/gl/GL.hpp"
#include "atlas/core/Time.hpp"
#include "atlas/utils/Application.hpp"
#include "atlas/core/Platform.hpp"
#include "atlas/core/Macros.hpp"

#ifdef ATLAS_PLATFORM_WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#endif
#include <GLFW/glfw3native.h>

namespace atlas
{
    namespace utils
    {
        void renderDrawLists(ImDrawData* drawData)
        {
            GuiData& data = Gui::getInstance().getData();

            ImGuiIO& io = ImGui::GetIO();
            int fb_width = 
                (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
            int fb_height = 
                (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
            if (fb_width == 0 || fb_height == 0)
                return;
            drawData->ScaleClipRects(io.DisplayFramebufferScale);

            // Backup GL state
            GLint last_program; 
            glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
            GLint last_texture; 
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
            GLint last_active_texture; 
            glGetIntegerv(GL_ACTIVE_TEXTURE, &last_active_texture);
            GLint last_array_buffer; 
            glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
            GLint last_element_array_buffer;
            glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, 
                &last_element_array_buffer);
            GLint last_vertex_array; 
            glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
            GLint last_blend_src; 
            glGetIntegerv(GL_BLEND_SRC, &last_blend_src);
            GLint last_blend_dst; 
            glGetIntegerv(GL_BLEND_DST, &last_blend_dst);
            GLint last_blend_equation_rgb; 
            glGetIntegerv(GL_BLEND_EQUATION_RGB, &last_blend_equation_rgb);
            GLint last_blend_equation_alpha; 
            glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &last_blend_equation_alpha);
            GLint last_viewport[4]; 
            glGetIntegerv(GL_VIEWPORT, last_viewport);

            GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
            GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
            GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
            GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDisable(GL_CULL_FACE);
            glDisable(GL_DEPTH_TEST);
            glEnable(GL_SCISSOR_TEST);
            glActiveTexture(GL_TEXTURE0);

            // Setup viewport, orthographic projection matrix
            glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
            const float ortho_projection[4][4] =
            {
                { 2.0f / io.DisplaySize.x, 0.0f,                   0.0f, 0.0f },
                { 0.0f,                  2.0f / -io.DisplaySize.y, 0.0f, 0.0f },
                { 0.0f,                  0.0f,                  -1.0f, 0.0f },
                { -1.0f,                  1.0f,                   0.0f, 1.0f },
            };
            glUseProgram(data.shaderHandle);
            glUniform1i(data.attribLocationTex, 0);
            glUniformMatrix4fv(data.attribLocationProjMtx, 1, GL_FALSE, 
                &ortho_projection[0][0]);
            glBindVertexArray(data.vaoHandle);

            for (int n = 0; n < drawData->CmdListsCount; n++)
            {
                const ImDrawList* cmd_list = drawData->CmdLists[n];
                const ImDrawIdx* idx_buffer_offset = 0;

                glBindBuffer(GL_ARRAY_BUFFER, data.vboHandle);
                glBufferData(GL_ARRAY_BUFFER, 
                    (GLsizeiptr)cmd_list->VtxBuffer.size() * sizeof(ImDrawVert), 
                    (GLvoid*)&cmd_list->VtxBuffer.front(), GL_STREAM_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.elementsHandle);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                    (GLsizeiptr)cmd_list->IdxBuffer.size() * sizeof(ImDrawIdx), 
                    (GLvoid*)&cmd_list->IdxBuffer.front(), GL_STREAM_DRAW);

                for (const ImDrawCmd* pcmd = cmd_list->CmdBuffer.begin(); 
                    pcmd != cmd_list->CmdBuffer.end(); pcmd++)
                {
                    if (pcmd->UserCallback)
                    {
                        pcmd->UserCallback(cmd_list, pcmd);
                    }
                    else
                    {
                        glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                        glScissor((int)pcmd->ClipRect.x, 
                            (int)(fb_height - pcmd->ClipRect.w), 
                            (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), 
                            (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                        glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, 
                            sizeof(ImDrawIdx) == 2 ? 
                            GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, 
                            idx_buffer_offset);
                    }
                    idx_buffer_offset += pcmd->ElemCount;
                }
            }

            glUseProgram(last_program);
            glActiveTexture(last_active_texture);
            glBindTexture(GL_TEXTURE_2D, last_texture);
            glBindVertexArray(last_vertex_array);
            glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
            glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
            glBlendFunc(last_blend_src, last_blend_dst);

            if (last_enable_blend)
            {
                glEnable(GL_BLEND);
            }
            else
            {
                glDisable(GL_BLEND);
            }

            if (last_enable_cull_face)
            {
                glEnable(GL_CULL_FACE);
            }
            else
            {
                glDisable(GL_CULL_FACE);
            }

            if (last_enable_depth_test)
            {
                glEnable(GL_DEPTH_TEST);
            }
            else
            {
                glDisable(GL_DEPTH_TEST);
            }

            if (last_enable_scissor_test)
            {
                glEnable(GL_SCISSOR_TEST); 
            }
            else
            {
                glDisable(GL_SCISSOR_TEST);
            }

            glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
        }

        void setClipboardText(const char* text)
        {
            glfwSetClipboardString(Gui::getInstance().getData().window, text);
        }

        const char* getClipboardText()
        {
            return glfwGetClipboardString(Gui::getInstance().getData().window);
        }

        Gui::Gui()
        {
            mData.window = Application::getInstance().getCurrentWindow();

            ImGuiIO& io = ImGui::GetIO();
            io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
            io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
            io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
            io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
            io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
            io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
            io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
            io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
            io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
            io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
            io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
            io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
            io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
            io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
            io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
            io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
            io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
            io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
            io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

            io.RenderDrawListsFn = renderDrawLists;
            io.SetClipboardTextFn = setClipboardText;
            io.GetClipboardTextFn = getClipboardText;

#ifdef ATLAS_PLATFORM_WIN32
            io.ImeWindowHandle = glfwGetWin32Window(mData.window);
#endif
        }

        Gui::~Gui()
        {
            invalidateDeviceObjects();
            ImGui::Shutdown();
        }

        Gui& Gui::getInstance()
        {
            static Gui instance;
            return instance;
        }

        void Gui::mousePressed(int button, int action, int mods)
        {
            UNUSED(mods);

            ImGuiIO& io = ImGui::GetIO();

            if (action == GLFW_PRESS && button >= 0 && button < 3)
            {
                io.MouseDown[button] = true;
            }
            else if (action == GLFW_RELEASE)
            {
                io.MouseDown[button] = false;
            }
        }

        void Gui::mouseMoved(double xPos, double yPos)
        {
            ImGuiIO& io = ImGui::GetIO();
            io.MousePos = ImVec2((float)xPos, (float)yPos);
        }

        void Gui::mouseScroll(double xPos, double yPos)
        {
            UNUSED(xPos);
            mData.mouseWheel += (float)yPos;
        }

        void Gui::keyPress(int key, int scancode, int action, int mods)
        {
            UNUSED(mods);
            UNUSED(scancode);

            ImGuiIO& io = ImGui::GetIO();
            if (action == GLFW_PRESS)
                io.KeysDown[key] = true;
            if (action == GLFW_RELEASE)
                io.KeysDown[key] = false;

            io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
            io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
            io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
            io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
        }

        void Gui::screenResize(int width, int height)
        {
            ImGuiIO& io = ImGui::GetIO();
            int w, h;
            glfwGetWindowSize(mData.window, &w, &h);
            io.DisplaySize = ImVec2((float)w, (float)h);
            io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)width / w) : 0,
                h > 0 ? ((float)height / h) : 0);
        }

        void Gui::update(atlas::core::Time<> const& t)
        {
            ImGuiIO& io = ImGui::GetIO();
            io.DeltaTime = (t.deltaTime > 0.0f) ? t.deltaTime : 1.0f / 60.0f;
        }

        void Gui::newFrame()
        {
            if (!mData.fontTexture)
                createDeviceObjects();

            ImGuiIO& io = ImGui::GetIO();

            io.MouseWheel = mData.mouseWheel;

            ImGui::NewFrame();
        }

        GuiData& Gui::getData()
        {
            return mData;
        }

        void Gui::createFontsTexture()
        {
            ImGuiIO& io = ImGui::GetIO();
            unsigned char* pixels;
            int width, height;
            io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   

            GLint last_texture;
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
            glGenTextures(1, &mData.fontTexture);
            glBindTexture(GL_TEXTURE_2D, mData.fontTexture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

            io.Fonts->TexID = (void *)(intptr_t)mData.fontTexture;

            glBindTexture(GL_TEXTURE_2D, last_texture);
        }

        void Gui::createDeviceObjects()
        {
            GLint last_texture, last_array_buffer, last_vertex_array;
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
            glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
            glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);

            const GLchar *vertex_shader =
                "#version 330\n"
                "uniform mat4 ProjMtx;\n"
                "in vec2 Position;\n"
                "in vec2 UV;\n"
                "in vec4 Color;\n"
                "out vec2 Frag_UV;\n"
                "out vec4 Frag_Color;\n"
                "void main()\n"
                "{\n"
                "	Frag_UV = UV;\n"
                "	Frag_Color = Color;\n"
                "	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
                "}\n";

            const GLchar* fragment_shader =
                "#version 330\n"
                "uniform sampler2D Texture;\n"
                "in vec2 Frag_UV;\n"
                "in vec4 Frag_Color;\n"
                "out vec4 Out_Color;\n"
                "void main()\n"
                "{\n"
                "	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
                "}\n";

            mData.shaderHandle = glCreateProgram();
            mData.vertHandle = glCreateShader(GL_VERTEX_SHADER);
            mData.fragHandle = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(mData.vertHandle, 1, &vertex_shader, 0);
            glShaderSource(mData.fragHandle, 1, &fragment_shader, 0);
            glCompileShader(mData.vertHandle);
            glCompileShader(mData.fragHandle);
            glAttachShader(mData.shaderHandle, mData.vertHandle);
            glAttachShader(mData.shaderHandle, mData.fragHandle);
            glLinkProgram(mData.shaderHandle);

            mData.attribLocationTex = glGetUniformLocation(mData.shaderHandle, "Texture");
            mData.attribLocationProjMtx = glGetUniformLocation(mData.shaderHandle, "ProjMtx");
            mData.attribLocationPosition = glGetAttribLocation(mData.shaderHandle, "Position");
            mData.attribLocationUV = glGetAttribLocation(mData.shaderHandle, "UV");
            mData.attribLocationColor = glGetAttribLocation(mData.shaderHandle, "Color");

            glGenBuffers(1, &mData.vboHandle);
            glGenBuffers(1, &mData.elementsHandle);

            glGenVertexArrays(1, &mData.vaoHandle);
            glBindVertexArray(mData.vaoHandle);
            glBindBuffer(GL_ARRAY_BUFFER, mData.vboHandle);
            glEnableVertexAttribArray(mData.attribLocationPosition);
            glEnableVertexAttribArray(mData.attribLocationUV);
            glEnableVertexAttribArray(mData.attribLocationColor);

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
            glVertexAttribPointer(mData.attribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
            glVertexAttribPointer(mData.attribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
            glVertexAttribPointer(mData.attribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF

            createFontsTexture();

            // Restore modified GL state
            glBindTexture(GL_TEXTURE_2D, last_texture);
            glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
            glBindVertexArray(last_vertex_array);
        }

        void Gui::invalidateDeviceObjects()
        {
            if (mData.vaoHandle) glDeleteVertexArrays(1, &mData.vaoHandle);
            if (mData.vboHandle) glDeleteBuffers(1, &mData.vboHandle);
            if (mData.elementsHandle) glDeleteBuffers(1, &mData.elementsHandle);
            mData.vaoHandle = mData.vboHandle = mData.elementsHandle = 0;

            glDetachShader(mData.shaderHandle, mData.vertHandle);
            glDeleteShader(mData.vertHandle);
            mData.vertHandle = 0;

            glDetachShader(mData.shaderHandle, mData.fragHandle);
            glDeleteShader(mData.fragHandle);
            mData.fragHandle = 0;

            glDeleteProgram(mData.shaderHandle);
            mData.shaderHandle = 0;

            if (mData.fontTexture)
            {
                glDeleteTextures(1, &mData.fontTexture);
                ImGui::GetIO().Fonts->TexID = 0;
                mData.fontTexture = 0;
            }
        }

    }
}