// name: renderer.hpp
// type: c++
// auth: is0urce
// desc: rendering procedures

#pragma once

#include "glew_options.hpp"
#include "glm_options.hpp"

#include <px/rgl/rgl.hpp>
#include <px/rgl/compilation.hpp>
#include <px/rgl/offscreen.hpp>
#include <px/rft/ft_font.hpp>

#include "glyph_vertex.hpp"
#include "sprite_vertex.hpp"
#include "camera_uniform.hpp"

#include "draw_batch.hpp"
#include "message_control.hpp"
#include "lightmap_control.hpp"

#include <px/common/string.hpp>

#include <algorithm>
#include <vector>
#include <stdexcept>

namespace px {

    static const char * font_path = "data/fonts/NotoSansUI-Bold.ttf";
    //static const char * font_path = "data/fonts/TitilliumWeb-Bold.ttf"; //"data/fonts/NotoSansUI-Bold.ttf";
    static const unsigned int font_size = 32;
    static const size_t atlas_size = 512;

    class renderer {
    public:
        void run(double delta_time) {
            if (screen_width == 0 || screen_height == 0) return;
            prepare_resources(delta_time);

            // sprites drawed to offscreen A

            glBindFramebuffer(GL_FRAMEBUFFER, diffuse.framebuffer);
            glClear(GL_COLOR_BUFFER_BIT);
            if (sprite_data) {
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // alpha blending for sprites
                glUseProgram(sprite_program);
                for (size_t i = 0, size = sprite_data->size(); i != size; ++i) {
                    sprites[i].draw_arrays(GL_QUADS, GL_STREAM_DRAW, (*sprite_data)[i]);
                }
            }

            // lightmap drawed to offscreen B

            glBindFramebuffer(GL_FRAMEBUFFER, light.framebuffer);
            glClear(GL_COLOR_BUFFER_BIT);
            glBlendFunc(GL_ONE, GL_ONE); // additive blending for lights
            glUseProgram(light_program);
            light_pass.draw_arrays(GL_QUADS, 8); // two quads for current and last lightmap overlays

            // combining diffuse with lights, postprocessing, writing to main screen

            glUseProgram(postprocess_program);
            glBlendFunc(GL_ONE, GL_ZERO); // overwrite
            postprocess.draw_arrays(GL_QUADS, 4);

            // popup notification message overlay

            glUseProgram(popup_program);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            popups.pass.draw_arrays(GL_QUADS, static_cast<GLsizei>(glyphs.size()));

            gl_assert();
        }

        void resize(unsigned int width, unsigned int height) {
            assign_size(width, height);
            reset_pipeline();
        }

        void assign_sprite_data(std::vector<std::vector<sprite_vertex>> const* data) noexcept {
            sprite_data = data;
        }

        void assign_lightmap_data(lightmap_data const* current, lightmap_data const* last) noexcept {
            light_control.current = current;
            light_control.last = last;
        }

        void assign_message_data(message_data const* data) noexcept {
            msg_ctrl.data = data;
        }

        unsigned int add_texture(unsigned int texture_width, unsigned int texture_height, void const* data) {
            if (!data) throw std::runtime_error("px::renderer::load_texture(...) - data is null");

            sprites.emplace_back();
            auto & batch = sprites.back();

            batch.texture.image2d(GL_RGBA, GL_RGBA, static_cast<GLsizei>(texture_width), static_cast<GLsizei>(texture_height), 0, GL_UNSIGNED_BYTE, data);
            batch.texture.filters(GL_NEAREST, GL_NEAREST); // required

            batch.vertices = GL_ARRAY_BUFFER;
            batch.geometry.swizzle(batch.vertices, sizeof(sprite_vertex), { GL_FLOAT, GL_FLOAT }, { 2, 2 }, { offsetof(sprite_vertex, position), offsetof(sprite_vertex, texture) });

            setup_batch(batch);
            return batch.texture;
        }

        void zoom(bool up) {
            scale_level = up ? std::min<unsigned int>(5, scale_level + 1) : std::max<unsigned int>(1, scale_level - 1);
            reset_scale();
        }

        float get_scale() const noexcept {
            return scale;
        }

        void set_scale(float pan) noexcept {
            scale = pan;
        }

        void pixel_scale(unsigned int level, unsigned int ppu) {
            scale_level = level;
            scale = level * ppu * 2.0f / screen_width;
        }

        unsigned int width() const noexcept {
            return screen_width;
        }

        unsigned int height() const noexcept {
            return screen_height;
        }

    public:
        renderer(unsigned int width, unsigned int height)
            : sprite_data(nullptr)
            , scale(1.0f)
            , scale_level(3)
            , popup_font(font_path, font_size, atlas_size) {

            // output size
            assign_size(width, height);

            // setup rendering
            create_resources();
            reset_pipeline();
        }

    private:
        void create_resources() {
            // set states

            glClearColor(0, 0, 0, 1);
            glEnable(GL_BLEND);

            // compile shaders

            sprite_program = compile_program("data/shaders/sprite", { "Camera" }, { "img" });
            light_program = compile_program("data/shaders/light", { "Camera" }, { "current", "last" });
            popup_program = compile_program("data/shaders/text", { "Camera" }, { "img" });
            postprocess_program = compile_program("data/shaders/process", {}, { "diffuse", "lightmap" });

            // setup textures

            light_current.image2d(GL_RGBA, GL_RGBA, 10, 10);
            light_current.filters(GL_LINEAR, GL_NEAREST); // required
            light_last.image2d(GL_RGBA, GL_RGBA, 10, 10);
            light_last.filters(GL_LINEAR, GL_NEAREST); // required

            popups.texture.image2d(GL_RED, GL_RED, 10, 10);
            popups.texture.filters(GL_LINEAR, GL_LINEAR); // required
            popups.vertices = GL_ARRAY_BUFFER;
            popups.geometry.swizzle(popups.vertices, sizeof(glyph_vertex), { GL_FLOAT, GL_FLOAT, GL_FLOAT }, { 2, 2, 4 }, { offsetof(glyph_vertex, position), offsetof(glyph_vertex, texture), offsetof(glyph_vertex, tint) });
        }

        void reset_pipeline() {
            diffuse.setup(GL_RGBA32F, GL_RGBA, screen_width, screen_height);
            light.setup(GL_RGBA32F, GL_RGBA, screen_width, screen_height);

            for (auto & batch : sprites) {
                setup_batch(batch);
            }

            light_pass.output(light.framebuffer, screen_width, screen_height);
            light_pass.bind_textures({ light_current, light_last });
            light_pass.bind_uniform(camera);

            postprocess = { 0, 0, static_cast<GLsizei>(screen_width), static_cast<GLsizei>(screen_height) }; // main framebuffer, no vao
            postprocess.bind_textures({ diffuse.texture, light.texture });

            popups.pass = { 0, popups.geometry, static_cast<GLsizei>(screen_width), static_cast<GLsizei>(screen_height) };
            popups.pass.bind_texture(popups.texture);
            popups.pass.bind_uniform(camera);

            reset_scale();
        }

        void reset_scale() {
            pixel_scale(scale_level, 32);
        }

        void assign_size(unsigned int new_width, unsigned int new_height) {
            screen_width = new_width;
            screen_height = new_height;
            screen_aspect = static_cast<float>(screen_width) / static_cast<float>(screen_height);
        }

        void setup_batch(draw_batch & batch) const {
            batch.pass = { diffuse.framebuffer, batch.geometry, static_cast<GLsizei>(screen_width), static_cast<GLsizei>(screen_height) };
            batch.pass.bind_texture(batch.texture);
            batch.pass.bind_uniform(camera);
        }

        void prepare_resources(double delta_time) {
            // popups
            if (msg_ctrl.is_dirty()) {
                float const lm = 0.25f; // lines per tile multiplier
                float const mm = lm / 64.0f / font_size; // one / freetype pixel-per-unit / font_size / lines-per-tile = multiplier for font-bitmap to world-space mapping

                glyphs.clear();
                for (auto const& popup : msg_ctrl.data->messages) {
                    float const center = 0.5f + static_cast<float>(popup.position.x());
                    float const baseline = 0.5f + lm + static_cast<float>(popup.position.y() + popup.lift * lm);
                    glm::vec4 const tint = { popup.msg.tint.R, popup.msg.tint.G, popup.msg.tint.B, popup.msg.tint.A };

                    long length = 0;
                    string::enumerate_utf8(popup.msg.text, [&](auto codepoint) {
                        length += popup_font[codepoint].advance_h;
                    });

                    float pen = center - length * mm * 0.5f;
                    string::enumerate_utf8(popup.msg.text, [&](auto codepoint) {
                        auto const& g = popup_font[codepoint];
                        const float left = pen + g.bearing_hx * mm;
                        const float top = baseline + g.bearing_hy * mm;
                        const float w = g.width * mm;
                        const float h = g.height * mm;

                        glyphs.push_back({ { left + 0, top - h }, { g.sx, g.sy }, tint });
                        glyphs.push_back({ { left + 0, top - 0 }, { g.sx, g.dy }, tint });
                        glyphs.push_back({ { left + w, top - 0 }, { g.dx, g.dy }, tint });
                        glyphs.push_back({ { left + w, top - h }, { g.dx, g.sy }, tint });

                        pen += g.advance_h * mm;
                    });
                }
                popups.vertices.load_array(GL_STATIC_DRAW, glyphs);
                msg_ctrl.notify_cashing();
            }

            update_textures();
            fill_uniforms(delta_time);
        }

        void fill_uniforms(double delta_time) {
            double const turn_time = std::min(1.0, delta_time * 5.0);
            camera.load<camera_uniform>(GL_STREAM_DRAW, {
                { scale, scale * screen_aspect },
                { 0.5f, 0.5f },
                { static_cast<float>(screen_width), static_cast<float>(screen_height) },
                { static_cast<float>(light_control.width()), static_cast<float>(light_control.height()) },
                { static_cast<float>(light_control.width() / 2), static_cast<float>(light_control.height() / 2) },
                { static_cast<float>(light_control.dx()), static_cast<float>(light_control.dy()) },
                { static_cast<float>(turn_time), static_cast<float>(1.0 - turn_time) },
                { static_cast<float>(delta_time), 0.0f }
                });
        }

        void update_textures() {
            // lightmap
            if (light_control.is_dirty()) {
                light_current.image2d(GL_RGBA, GL_RGBA, static_cast<GLsizei>(light_control.width()), static_cast<GLsizei>(light_control.height()), 0, GL_FLOAT, light_control.current->raw);
                light_last.image2d(GL_RGBA, GL_RGBA, static_cast<GLsizei>(light_control.width()), static_cast<GLsizei>(light_control.height()), 0, GL_FLOAT, light_control.last->raw);
                light_control.notify_cashing();
            }
            // font bitmap
            if (popup_font.updated()) {
                unsigned int atlas_width, atlas_height;
                void const* const bitmap = popup_font.download(atlas_width, atlas_height);
                popups.texture.image2d(GL_RED, GL_RED, static_cast<GLsizei>(atlas_width), static_cast<GLsizei>(atlas_height), 0, GL_UNSIGNED_BYTE, bitmap);
            }
        }

    private:
        unsigned int                                    screen_width;
        unsigned int                                    screen_height;
        float                                           screen_aspect;
        float                                           scale;
        unsigned int                                    scale_level;
        gl_uniform                                      camera;
        gl_program                                      sprite_program;
        gl_program                                      postprocess_program;
        gl_program                                      popup_program;
        gl_program                                      light_program;
        gl_texture                                      light_current;
        gl_texture                                      light_last;
        gl_texture                                      popup_texture;

        offscreen                                       diffuse;
        offscreen                                       light;
        pass                                            postprocess;
        pass                                            light_pass;

        message_control                                 msg_ctrl;
        draw_batch                                      popups;
        std::vector<glyph_vertex>                       glyphs;
        ft_font                                         popup_font;

        std::vector<draw_batch>                         sprites;
        std::vector<std::vector<sprite_vertex>> const*  sprite_data;
        lightmap_control                                light_control;
    };
}