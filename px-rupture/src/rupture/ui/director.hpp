// name: director_works.hpp
// type: c++ header

#pragma once

#include <imgui/imgui.h>

#include "../draw/glew_options.hpp"

#include <px/rgl/rgl.hpp>
#include <px/rgl/compilation.hpp>

namespace px {

    class director final {
    public:
        void resize(unsigned int w, unsigned int h) {
            width = w;
            height = h;

            auto & io = ImGui::GetIO();

            io.DisplaySize = { static_cast<float>(width), static_cast<float>(height) };
            io.DisplayFramebufferScale = { 1, 1 };
        }

        void begin(double delta_time) {
            ImGui::GetIO().DeltaTime = static_cast<float>(delta_time);
            ImGui::NewFrame();
        }

        void end() {
            ImGui::Render();
            render();
        }

        bool click(unsigned int mouse_button, bool is_down) {
            if (mouse_button > 5) return false;
            bool hovered = ImGui::IsAnyWindowHovered();

            if (!hovered && is_down) return false; // always process button release

            ImGui::GetIO().MouseDown[mouse_button] = is_down;
            return hovered;
        }

        bool text(unsigned int codepoint) {
            if (!ImGui::IsAnyItemActive() || codepoint >= 256) return false;

            ImGui::GetIO().AddInputCharacter(static_cast<unsigned char>(codepoint));
            return true;
        }

        bool hover(unsigned int x, unsigned int y) {
            ImGui::GetIO().MousePos = { static_cast<float>(x), static_cast<float>(y) };
            return ImGui::IsAnyWindowHovered();
        }

        bool scroll(double horisontal, double vertical) {
            ImGui::GetIO().MouseWheel = static_cast<float>(horisontal + vertical);
            return ImGui::IsAnyWindowHovered();
        }

        bool takes_input() {
            return ImGui::IsAnyItemActive();
        }

    public:
        ~director() {
            ImGui::Shutdown();
            release_pipeline();
        }

        director(unsigned int w, unsigned int h) {
            resize(w, h);
            create_pipeline();
            load_font();

            auto & io = ImGui::GetIO();
            io.IniFilename = nullptr;
            io.DeltaTime = 0;
            //io.FontDefault->Scale = 2;
            io.FontGlobalScale = 2.0f;

            auto & style = ImGui::GetStyle();
            style.WindowRounding = 0;
            style.ScrollbarRounding = 0;
            style.AntiAliasedLines = false;
            style.GrabMinSize = 20.0f;
            style.WindowPadding = { 0, 0 };
            style.Colors[ImGuiCol_SliderGrabActive] = { 1.0f, 1.0f, 1.0f, 0.75f };
            style.Colors[ImGuiCol_WindowBg] = { 0, 0, 0, 0.67f };
        }

    private:
        void render() {
            // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
            auto const& io = ImGui::GetIO();
            int const framebuffer_width = static_cast<int>(io.DisplaySize.x * io.DisplayFramebufferScale.x);
            int const framebuffer_height = static_cast<int>(io.DisplaySize.y * io.DisplayFramebufferScale.y);
            if (framebuffer_width == 0 || framebuffer_height == 0) return;

            // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, polygon fill
            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDisable(GL_CULL_FACE);
            glDisable(GL_DEPTH_TEST);
            glEnable(GL_SCISSOR_TEST);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            // Setup viewport, orthographic projection matrix
            glViewport(0, 0, static_cast<GLsizei>(framebuffer_width), static_cast<GLsizei>(framebuffer_height));
            float const ortho_projection[4][4] = {
                {  2.0f / io.DisplaySize.x, 0.0f,                     0.0f, 0.0f },
                {  0.0f,                    2.0f / -io.DisplaySize.y, 0.0f, 0.0f },
                {  0.0f,                    0.0f,                    -1.0f, 0.0f },
                { -1.0f,                    1.0f,                     0.0f, 1.0f },
            };

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glActiveTexture(GL_TEXTURE0);
            glUseProgram(shader);
            glUniform1i(location_texture, 0);
            glUniformMatrix4fv(location_projection, 1, GL_FALSE, &ortho_projection[0][0]);
            glBindVertexArray(vao);
            glBindSampler(0, 0); // Rely on combined texture/sampler state.

            auto draw_data = ImGui::GetDrawData();
            draw_data->ScaleClipRects(io.DisplayFramebufferScale);
            for (int n = 0, size = draw_data->CmdListsCount; n != size; ++n) {
                ImDrawList const* const cmd_list = draw_data->CmdLists[n];
                ImDrawIdx const* idx_buffer_offset = 0;

                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(cmd_list->VtxBuffer.Size * sizeof(ImDrawVert)), static_cast<GLvoid const*>(cmd_list->VtxBuffer.Data), GL_STREAM_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx)), static_cast<GLvoid const*>(cmd_list->IdxBuffer.Data), GL_STREAM_DRAW);

                for (int cmd_i = 0, cmd_size = cmd_list->CmdBuffer.Size; cmd_i != cmd_size; ++cmd_i) {
                    ImDrawCmd const* pcmd = &cmd_list->CmdBuffer[cmd_i];
                    if (pcmd->UserCallback) {
                        pcmd->UserCallback(cmd_list, pcmd);
                    }
                    else {
                        glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(reinterpret_cast<size_t>(pcmd->TextureId)));
                        glScissor(static_cast<GLint>(pcmd->ClipRect.x), static_cast<GLint>(framebuffer_height - pcmd->ClipRect.w), static_cast<GLsizei>(pcmd->ClipRect.z - pcmd->ClipRect.x), static_cast<GLsizei>(pcmd->ClipRect.w - pcmd->ClipRect.y));
                        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(pcmd->ElemCount), sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);
                    }
                    idx_buffer_offset += pcmd->ElemCount;
                }
            }

            glScissor(0, 0, framebuffer_width, framebuffer_height); // stop scissors! this somehow can interfere with video recording software
        }

        void create_pipeline() {
            shader = compile_program("data/shaders/ui");
            location_texture = glGetUniformLocation(shader, "Texture");
            location_projection = glGetUniformLocation(shader, "ProjMtx");
            location_position = glGetAttribLocation(shader, "Position");
            location_uv = glGetAttribLocation(shader, "UV");
            location_color = glGetAttribLocation(shader, "Color");

            glGenBuffers(1, &vbo);
            glGenBuffers(1, &elements);

            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glEnableVertexAttribArray(location_position);
            glEnableVertexAttribArray(location_uv);
            glEnableVertexAttribArray(location_color);

#define PX_OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
            glVertexAttribPointer(location_position, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid const*)PX_OFFSETOF(ImDrawVert, pos));
            glVertexAttribPointer(location_uv, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid const*)PX_OFFSETOF(ImDrawVert, uv));
            glVertexAttribPointer(location_color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid const*)PX_OFFSETOF(ImDrawVert, col));
#undef PX_OFFSETOF
        }

        void release_pipeline() {
            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
            glDeleteBuffers(1, &elements);
        }

        void load_font() {
            auto & io = ImGui::GetIO();
            unsigned char * pixels;
            int texture_width;
            int texture_height;
            io.Fonts->GetTexDataAsRGBA32(&pixels, &texture_width, &texture_height);

            font.image2d(GL_RGBA, GL_RGBA, static_cast<GLsizei>(texture_width), static_cast<GLsizei>(texture_height), 0, GL_UNSIGNED_BYTE, pixels);
            font.filters(GL_NEAREST, GL_NEAREST); // required

            io.Fonts->TexID = reinterpret_cast<void*>(static_cast<size_t>(font));
        }

    private:
        unsigned int    width;
        unsigned int    height;
        gl_texture      font;
        gl_program      shader;
        int             location_texture;
        int             location_projection;
        int             location_position;
        int             location_uv;
        int             location_color;
        unsigned int    vbo;
        unsigned int    vao;
        unsigned int    elements;
    };
}