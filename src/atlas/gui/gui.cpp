#include "gui.hpp"

#include <atlas/glx/buffer.hpp>
#include <atlas/glx/glsl.hpp>
#include <zeus/platform.hpp>

#include <fmt/printf.h>

#if defined(ZEUS_PLATFORM_WINDOWS)
#    define GLFW_EXPOSE_NATIVE_WIN32
#    define GLFW_EXPOSE_NATIVE_WGL
#endif
#include <GLFW/glfw3native.h>

#include <algorithm>
#include <array>

namespace atlas::gui
{
    void render_draw_data(UIRenderData const& data, ImDrawData* drawData);
    bool create_fonts_texture(UIRenderData& data);
    void destroy_fonts_texture(UIRenderData& data);
    bool create_device_objects(UIRenderData& data);
    void destroy_device_objects(UIRenderData& data);

    void update_mouse_pos_and_buttons(UIWindowData& data);
    void update_mouse_cursor(UIWindowData& data);

    const char* get_clipboard_text(void* user_data)
    {
        return glfwGetClipboardString(static_cast<GLFWwindow*>(user_data));
    }

    void set_clipboard_text(void* user_data, char const* text)
    {
        glfwSetClipboardString(static_cast<GLFWwindow*>(user_data), text);
    }

    bool initialize_ui_render_data(UIRenderData& data)
    {
        auto& io               = ImGui::GetIO();
        io.BackendRendererName = "atlas_opengl";
        create_device_objects(data);
        return true;
    }

    void destroy_ui_render_data(UIRenderData& data)
    {
        destroy_device_objects(data);
    }

    void render_ui_frame(UIRenderData const& data)
    {
        render_draw_data(data, ImGui::GetDrawData());
    }

    bool initialize_ui_window_data(UIWindowData& data)
    {
        std::fill(
            data.mouse_cursors.begin(), data.mouse_cursors.end(), nullptr);

        auto& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
        io.BackendPlatformName = "atlas";

        io.KeyMap[ImGuiKey_Tab]        = GLFW_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow]  = GLFW_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow]    = GLFW_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow]  = GLFW_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp]     = GLFW_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown]   = GLFW_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home]       = GLFW_KEY_HOME;
        io.KeyMap[ImGuiKey_End]        = GLFW_KEY_END;
        io.KeyMap[ImGuiKey_Insert]     = GLFW_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete]     = GLFW_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace]  = GLFW_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space]      = GLFW_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter]      = GLFW_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape]     = GLFW_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_A]          = GLFW_KEY_A;
        io.KeyMap[ImGuiKey_C]          = GLFW_KEY_C;
        io.KeyMap[ImGuiKey_V]          = GLFW_KEY_V;
        io.KeyMap[ImGuiKey_X]          = GLFW_KEY_X;
        io.KeyMap[ImGuiKey_Y]          = GLFW_KEY_Y;
        io.KeyMap[ImGuiKey_Z]          = GLFW_KEY_Z;

        io.SetClipboardTextFn = set_clipboard_text;
        io.GetClipboardTextFn = get_clipboard_text;

        data.mouse_cursors[ImGuiMouseCursor_Arrow] =
            glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
        data.mouse_cursors[ImGuiMouseCursor_TextInput] =
            glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
        data.mouse_cursors[ImGuiMouseCursor_ResizeAll] =
            glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
        data.mouse_cursors[ImGuiMouseCursor_ResizeNS] =
            glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
        data.mouse_cursors[ImGuiMouseCursor_ResizeEW] =
            glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
        data.mouse_cursors[ImGuiMouseCursor_ResizeNESW] =
            glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
        data.mouse_cursors[ImGuiMouseCursor_ResizeNWSE] =
            glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
        data.mouse_cursors[ImGuiMouseCursor_Hand] =
            glfwCreateStandardCursor(GLFW_HAND_CURSOR);

        return true;
    }

    void set_ui_window(UIWindowData& data, GLFWwindow* window)
    {
        data.window = window;
        auto& io    = ImGui::GetIO();
#if defined(ATLAS_PLATFORM_WINDOWS)
        io.ImeWindowHandle =
            reinterpret_cast<void*>(glfwGetWin32Window(window));
#endif
        io.ClipboardUserData = window;
    }

    void start_ui_window_frame(UIWindowData& data)
    {
        auto& io = ImGui::GetIO();
        if (!io.Fonts->IsBuilt())
        {
            throw std::runtime_error("error: font atlas not built. Did you "
                                     "miss a call to initializeGuiRenderData?");
        }

        int w, h;
        int display_w, display_h;
        glfwGetWindowSize(data.window, &w, &h);
        glfwGetFramebufferSize(data.window, &display_w, &display_h);
        io.DisplaySize = ImVec2(static_cast<float>(w), static_cast<float>(h));
        if (w > 0 && h > 0)
        {
            io.DisplayFramebufferScale =
                ImVec2(static_cast<float>(display_w) / w,
                       static_cast<float>(display_h) / h);
        }

        double current_time = glfwGetTime();
        io.DeltaTime        = data.time > 0.0
                           ? static_cast<float>(current_time - data.time)
                           : 1.0f / 60.0f;
        data.time = current_time;
    }

    void update_ui_window_frame(UIWindowData& data)
    {
        update_mouse_pos_and_buttons(data);
        update_mouse_cursor(data);
    }

    void destroy_ui_window(UIWindowData& data)
    {
        for (ImGuiMouseCursor i{0}; i < ImGuiMouseCursor_COUNT; ++i)
        {
            if (!data.mouse_cursors[i])
            {
                glfwDestroyCursor(data.mouse_cursors[i]);
            }
            data.mouse_cursors[i] = nullptr;
        }
        data.window = nullptr;
    }

    void mouse_pressed_callback(UIWindowData& data,
                                int button,
                                int action,
                                [[maybe_unused]] int mode)
    {
        if (action == GLFW_PRESS && button >= 0 &&
            button < static_cast<int>(data.mouse_just_pressed.size()))
        {
            data.mouse_just_pressed[button] = true;
        }
    }

    void mouse_scroll_callback(double xOffset, double yOffset)
    {
        auto& io = ImGui::GetIO();
        io.MouseWheelH += static_cast<float>(xOffset);
        io.MouseWheel += static_cast<float>(yOffset);
    }

    void key_press_callback(int key,
                            [[maybe_unused]] int scancode,
                            int action,
                            [[maybe_unused]] int mods)
    {
        auto& io = ImGui::GetIO();
        if (action == GLFW_PRESS)
        {
            io.KeysDown[key] = true;
        }
        if (action == GLFW_RELEASE)
        {
            io.KeysDown[key] = false;
        }

        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] ||
                     io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] ||
                      io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt =
            io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] ||
                      io.KeysDown[GLFW_KEY_RIGHT_SUPER];
    }

    void char_callback(unsigned int c)
    {
        auto& io = ImGui::GetIO();
        io.AddInputCharacter(c);
    }

    void new_frame(UIWindowData& data)
    {
        start_ui_window_frame(data);
        ImGui::NewFrame();
    }

    void end_frame(UIWindowData& window_data, UIRenderData& render_data)
    {
        ImGui::Render();
        render_ui_frame(render_data);
        update_ui_window_frame(window_data);
    }

    static void setupRenderState(UIRenderData const& render_data,
                                 ImDrawData* draw_data,
                                 int fb_width,
                                 int fb_height,
                                 GLuint vao)
    {
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_SCISSOR_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glViewport(0,
                   0,
                   static_cast<GLsizei>(fb_width),
                   static_cast<GLsizei>(fb_height));
        float L = draw_data->DisplayPos.x;
        float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
        float T = draw_data->DisplayPos.y;
        float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;

        // clang-format off
        const std::array<std::array<float, 4>, 4> ortho_projection{{
            {{2.0f / (R - L),    0.0f,               0.0f,  0.0f}},
            {{0.0f,              2.0f / (T - B),     0.0f,  0.0f}},
            {{0.0f,              0.0f,              -1.0f,  0.0f}},
            {{(R + L) / (L - R), (T + B) / (B - T), 0.0f,   1.0f}}
        }};
        // clang-format on

        glUseProgram(render_data.shader_handle);
        glUniform1i(render_data.tex_attrib_location, 0);
        glUniformMatrix4fv(render_data.proj_mtx_attrib_location,
                           1,
                           GL_FALSE,
                           &ortho_projection[0][0]);
        glBindSampler(0, 0);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, render_data.vbo_handle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_data.elements_handle);
        glEnableVertexAttribArray(render_data.vtx_pos_attrib_location);
        glEnableVertexAttribArray(render_data.vts_uv_attrib_location);
        glEnableVertexAttribArray(render_data.vtx_colour_attrib_location);
        glVertexAttribPointer(
            render_data.vtx_pos_attrib_location,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(ImDrawVert),
            reinterpret_cast<GLvoid*>(IM_OFFSETOF(ImDrawVert, pos)));
        glVertexAttribPointer(
            render_data.vts_uv_attrib_location,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(ImDrawVert),
            reinterpret_cast<GLvoid*>(IM_OFFSETOF(ImDrawVert, uv)));
        glVertexAttribPointer(
            render_data.vtx_colour_attrib_location,
            4,
            GL_UNSIGNED_BYTE,
            GL_TRUE,
            sizeof(ImDrawVert),
            reinterpret_cast<GLvoid*>(IM_OFFSETOF(ImDrawVert, col)));
    }

    void render_draw_data(UIRenderData const& render_data,
                          ImDrawData* draw_data)
    {
        int fb_width  = static_cast<int>(draw_data->DisplaySize.x *
                                        draw_data->FramebufferScale.x);
        int fb_height = static_cast<int>(draw_data->DisplaySize.y *
                                         draw_data->FramebufferScale.y);
        if (fb_width <= 0 || fb_height <= 0)
        {
            return;
        }

        GLint last_active_texture;
        glGetIntegerv(GL_ACTIVE_TEXTURE, &last_active_texture);
        glActiveTexture(GL_TEXTURE0);

        GLint last_program;
        glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
        GLint last_texture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
        GLint last_sampler;
        glGetIntegerv(GL_SAMPLER_BINDING, &last_sampler);
        GLint last_array_buffer;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_array_buffer);
        GLint last_vertex_array_object;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array_object);

        std::array<GLint, 2> last_polygon_mode;
        glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode.data());
        std::array<GLint, 4> last_viewport;
        glGetIntegerv(GL_VIEWPORT, last_viewport.data());
        std::array<GLint, 4> last_scissor_box;
        glGetIntegerv(GL_VIEWPORT, last_scissor_box.data());

        GLint last_blend_src_rgb;
        glGetIntegerv(GL_BLEND_SRC_RGB, &last_blend_src_rgb);
        GLint last_blend_dst_rgb;
        glGetIntegerv(GL_BLEND_DST_RGB, &last_blend_dst_rgb);
        GLint last_blend_src_alpha;
        glGetIntegerv(GL_BLEND_SRC_ALPHA, &last_blend_src_alpha);
        GLint last_blend_dst_alpha;
        glGetIntegerv(GL_BLEND_DST_ALPHA, &last_blend_dst_alpha);
        GLint last_blend_equation_rgb;
        glGetIntegerv(GL_BLEND_EQUATION_RGB, &last_blend_equation_rgb);
        GLint last_blend_equation_alpha;
        glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &last_blend_equation_alpha);

        GLboolean last_enable_blend        = glIsEnabled(GL_BLEND);
        GLboolean last_enable_cull_face    = glIsEnabled(GL_CULL_FACE);
        GLboolean last_enabe_depth_test    = glIsEnabled(GL_DEPTH_TEST);
        GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

        GLint last_clip_origin{0};
        glGetIntegerv(GL_CLIP_ORIGIN, &last_clip_origin);
        bool is_clip_origin_lower_left = (last_clip_origin != GL_UPPER_LEFT);

        GLuint vao{0};
        glGenVertexArrays(1, &vao);
        setupRenderState(render_data, draw_data, fb_width, fb_height, vao);

        ImVec2 clip_off   = draw_data->DisplayPos;
        ImVec2 clip_scale = draw_data->FramebufferScale;

        for (int n{0}; n < draw_data->CmdListsCount; ++n)
        {
            const ImDrawList* cmd_list = draw_data->CmdLists[n];

            glBufferData(GL_ARRAY_BUFFER,
                         glx::size<ImDrawVert>(cmd_list->VtxBuffer.Size),
                         static_cast<const GLvoid*>(cmd_list->VtxBuffer.Data),
                         GL_STREAM_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         glx::size<ImDrawIdx>(cmd_list->IdxBuffer.Size),
                         static_cast<const GLvoid*>(cmd_list->IdxBuffer.Data),
                         GL_STREAM_DRAW);

            for (int cmdI{0}; cmdI < cmd_list->CmdBuffer.Size; ++cmdI)
            {
                const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmdI];
                if (pcmd->UserCallback != nullptr)
                {
                    if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                    {
                        setupRenderState(
                            render_data, draw_data, fb_width, fb_height, vao);
                    }
                    else
                    {
                        pcmd->UserCallback(cmd_list, pcmd);
                    }
                }
                else
                {
                    ImVec4 clip_rect;
                    clip_rect.x =
                        (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
                    clip_rect.y =
                        (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
                    clip_rect.z =
                        (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
                    clip_rect.w =
                        (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

                    if (clip_rect.x < fb_width && clip_rect.y < fb_height &&
                        clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
                    {
                        if (is_clip_origin_lower_left)
                        {
                            glScissor(
                                static_cast<int>(clip_rect.x),
                                static_cast<int>(fb_height - clip_rect.w),
                                static_cast<int>(clip_rect.z - clip_rect.x),
                                static_cast<int>(clip_rect.w - clip_rect.y));
                        }
                        else
                        {
                            glScissor(static_cast<int>(clip_rect.x),
                                      static_cast<int>(clip_rect.y),
                                      static_cast<int>(clip_rect.z),
                                      static_cast<int>(clip_rect.w));
                        }
                        glBindTexture(
                            GL_TEXTURE_2D,
                            static_cast<GLuint>(
                                reinterpret_cast<intptr_t>(pcmd->TextureId)));
                        glDrawElementsBaseVertex(
                            GL_TRIANGLES,
                            static_cast<GLsizei>(pcmd->ElemCount),
                            sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT
                                                   : GL_UNSIGNED_INT,
                            glx::buffer_offset<ImDrawIdx>(pcmd->IdxOffset),
                            static_cast<GLint>(pcmd->VtxOffset));
                    }
                }
            }
        }

        glDeleteVertexArrays(1, &vao);

        glUseProgram(last_program);
        glBindTexture(GL_TEXTURE_2D, last_texture);
        glBindSampler(0, last_sampler);
        glActiveTexture(last_active_texture);
        glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
        glBindVertexArray(last_vertex_array_object);

        glBlendEquationSeparate(last_blend_equation_rgb,
                                last_blend_equation_alpha);
        glBlendFuncSeparate(last_blend_src_rgb,
                            last_blend_dst_rgb,
                            last_blend_src_alpha,
                            last_blend_dst_alpha);

        if (last_enable_blend != 0)
        {
            glEnable(GL_BLEND);
        }
        else
        {
            glDisable(GL_BLEND);
        }

        if (last_enable_cull_face != 0)
        {
            glEnable(GL_CULL_FACE);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }

        if (last_enabe_depth_test != 0)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }

        if (last_enable_scissor_test != 0)
        {
            glEnable(GL_SCISSOR_TEST);
        }
        else
        {
            glDisable(GL_SCISSOR_TEST);
        }

        glPolygonMode(GL_FRONT_AND_BACK,
                      static_cast<GLenum>(last_polygon_mode[0]));
        glViewport(last_viewport[0],
                   last_viewport[1],
                   static_cast<GLsizei>(last_viewport[2]),
                   static_cast<GLsizei>(last_viewport[3]));
        glScissor(last_scissor_box[0],
                  last_scissor_box[1],
                  static_cast<GLsizei>(last_scissor_box[2]),
                  static_cast<GLsizei>(last_scissor_box[3]));
    }

    bool create_fonts_texture(UIRenderData& data)
    {
        auto& io = ImGui::GetIO();
        unsigned char* pixels;
        int width;
        int height;
        io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

        GLint lastTexture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);
        glGenTextures(1, &data.font_texture);
        glBindTexture(GL_TEXTURE_2D, data.font_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA,
                     width,
                     height,
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     pixels);

        io.Fonts->TexID = reinterpret_cast<ImTextureID>(
            static_cast<intptr_t>(data.font_texture));

        glBindTexture(GL_TEXTURE_2D, lastTexture);
        return true;
    }

    void destroy_fonts_texture(UIRenderData& data)
    {
        if (data.font_texture != 0u)
        {
            auto& io = ImGui::GetIO();
            glDeleteTextures(1, &data.font_texture);
            io.Fonts->TexID   = nullptr;
            data.font_texture = 0;
        }
    }

    bool create_device_objects(UIRenderData& data)
    {
        GLint last_texture;
        GLint last_array_buffer;
        GLint last_vertex_array;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);

        const GLchar* vertex_shader_string =
            "#version 450 core\n"
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
        const GLchar* frag_shader_string =
            "#version 450 core\n"
            "in vec2 Frag_UV;\n"
            "in vec4 Frag_Color;\n"
            "uniform sampler2D Texture;\n"
            "layout (location = 0) out vec4 Out_Color;\n"
            "void main()\n"
            "{\n"
            "    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
            "}\n";

        data.vert_handle = glCreateShader(GL_VERTEX_SHADER);
        if (auto ret =
                glx::compile_shader(vertex_shader_string, data.vert_handle);
            ret)
        {
            fmt::print(stderr,
                       "error: GUI vertex shader failed to compile: {}\n",
                       ret.value());
            return false;
        }

        data.frag_handle = glCreateShader(GL_FRAGMENT_SHADER);
        if (auto ret =
                glx::compile_shader(frag_shader_string, data.frag_handle);
            ret)
        {
            fmt::print(stderr,
                       "error: GUI fragment shader failed to compile: {}\n",
                       ret.value());
            return false;
        }

        data.shader_handle = glCreateProgram();
        glAttachShader(data.shader_handle, data.vert_handle);
        glAttachShader(data.shader_handle, data.frag_handle);
        if (auto ret = glx::link_shaders(data.shader_handle); ret)
        {
            fmt::print(stderr,
                       "error: GUI shader program failed to link: {}\n",
                       ret.value());
            return false;
        }

        auto& handle                  = data.shader_handle;
        data.tex_attrib_location      = glGetUniformLocation(handle, "Texture");
        data.proj_mtx_attrib_location = glGetUniformLocation(handle, "ProjMtx");
        data.vtx_pos_attrib_location  = glGetAttribLocation(handle, "Position");
        data.vts_uv_attrib_location   = glGetAttribLocation(handle, "UV");
        data.vtx_colour_attrib_location = glGetAttribLocation(handle, "Color");

        glGenBuffers(1, &data.vbo_handle);
        glGenBuffers(1, &data.elements_handle);

        create_fonts_texture(data);

        glBindTexture(GL_TEXTURE_2D, last_texture);
        glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
        glBindVertexArray(last_vertex_array);

        return true;
    }

    void destroy_device_objects(UIRenderData& data)
    {
        if (data.vbo_handle != 0u)
        {
            glDeleteBuffers(1, &data.vbo_handle);
            data.vbo_handle = 0;
        }

        if (data.elements_handle != 0u)
        {
            glDeleteBuffers(1, &data.elements_handle);
            data.elements_handle = 0;
        }

        if (data.shader_handle != 0u && data.vert_handle != 0u)
        {
            glDetachShader(data.shader_handle, data.vert_handle);
        }
        if (data.vert_handle != 0u)
        {
            glDeleteShader(data.vert_handle);
            data.vert_handle = 0;
        }

        if (data.shader_handle != 0u && data.frag_handle != 0u)
        {
            glDetachShader(data.shader_handle, data.frag_handle);
        }
        if (data.frag_handle != 0u)
        {
            glDeleteShader(data.frag_handle);
            data.frag_handle = 0;
        }

        if (data.shader_handle != 0u)
        {
            glDeleteProgram(data.shader_handle);
            data.shader_handle = 0;
        }

        destroy_fonts_texture(data);
    }

    void update_mouse_pos_and_buttons(UIWindowData& data)
    {
        auto& io = ImGui::GetIO();
        for (int i{0}; i < IM_ARRAYSIZE(io.MouseDown); ++i)
        {
            io.MouseDown[i] = data.mouse_just_pressed[i] ||
                              glfwGetMouseButton(data.window, i) != 0;
            data.mouse_just_pressed[i] = false;
        }

        const ImVec2 mouse_pos_backup = io.MousePos;
        io.MousePos = ImVec2(-std::numeric_limits<float>::max(),
                             std::numeric_limits<float>::max());
        const bool is_focused =
            glfwGetWindowAttrib(data.window, GLFW_FOCUSED) != 0;
        if (is_focused)
        {
            if (io.WantSetMousePos)
            {
                glfwSetCursorPos(
                    data.window, mouse_pos_backup.x, mouse_pos_backup.y);
            }
            else
            {
                double mouse_x, mouse_y;
                glfwGetCursorPos(data.window, &mouse_x, &mouse_y);
                io.MousePos = ImVec2(static_cast<float>(mouse_x),
                                     static_cast<float>(mouse_y));
            }
        }
    }

    void update_mouse_cursor(UIWindowData& data)
    {
        auto& io = ImGui::GetIO();
        if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) ||
            glfwGetInputMode(data.window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
        {
            return;
        }

        ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
        if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
        {
            glfwSetInputMode(data.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }
        else
        {
            glfwSetCursor(data.window,
                          data.mouse_cursors[imgui_cursor]
                              ? data.mouse_cursors[imgui_cursor]
                              : data.mouse_cursors[ImGuiMouseCursor_Arrow]);
            glfwSetInputMode(data.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
} // namespace atlas::gui
