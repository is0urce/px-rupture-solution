// name: renderer.hpp
// type: c++
// auth: is0urce
// desc: rendering procedures

#pragma once

#define GLEW_STATIC
#include <gl/glew.h>

#define GLM_FORCE_RADIANS
#pragma warning(push)
#pragma warning(disable:4201) // warning C4201: nonstandard extension used: nameless struct/union
#include <glm/glm.hpp>
#pragma warning(pop)

#include <px/rgl/rgl.hpp>
#include <px/rgl/compilation.hpp>
#include <px/rgl/offscreen.hpp>

#include "sprite_vertex.hpp"
#include "sprite_batch.hpp"
#include "camera_uniform.hpp"
#include "lightmap_control.hpp"

#include <vector>
#include <stdexcept>

namespace px {

	class renderer
	{
	public:
		struct camera_data;

	public:
		void run(double /*time*/)
		{
			glDisable(GL_SCISSOR_TEST);

			camera.load<camera_uniform>(GL_STREAM_DRAW, {
				{ scale, scale * screen_aspect },
				//{ 0.0f, 0.0f },
				{ 0.5f, 0.5f },
				{ static_cast<float>(screen_width), static_cast<float>(screen_height) },

				{ static_cast<float>(light_control.width()), static_cast<float>(light_control.height()) },
				{ static_cast<float>(light_control.width() / 2), static_cast<float>(light_control.height() / 2) },
				{ static_cast<float>(light_control.width() / 2), static_cast<float>(light_control.height() / 2) }
			});

			if (light_control.is_dirty()) {
				light_current.image2d(GL_RGBA, GL_RGBA, static_cast<GLsizei>(light_control.width()), static_cast<GLsizei>(light_control.height()), 0, GL_FLOAT, light_control.current->raw);
				//light_last.image2d(GL_RGBA, GL_RGBA, static_cast<GLsizei>(20), static_cast<GLsizei>(20));
				light_control.cashed();
			}

			glBindFramebuffer(GL_FRAMEBUFFER, diffuse.framebuffer);
			glClear(GL_COLOR_BUFFER_BIT);
			glUseProgram(sprite_program);
			if (sprite_data) {
				size_t size = sprite_data->size();
				for (size_t i = 0; i != size; ++i) {
					std::vector<sprite_vertex> const& vertices = (*sprite_data)[i];
					if (vertices.size() > 0) {
						sprites[i].draw_arrays(GL_QUADS, GL_STREAM_DRAW, vertices.size(), vertices.data());
					}
				}
			}

			glBindFramebuffer(GL_FRAMEBUFFER, light.framebuffer);
			glClear(GL_COLOR_BUFFER_BIT);
			glUseProgram(light_program);
			light_pass.draw_arrays(GL_QUADS, 4);

			glUseProgram(postprocess_program);
			postprocess.draw_arrays(GL_QUADS, 4);

			gl_assert();
		}
		void resize(int width, int height)
		{
			assign_size(width, height);
			reset_framebuffers();
		}
		void assign_sprite_data(std::vector<std::vector<sprite_vertex>> const* data) noexcept
		{
			sprite_data = data;
		}
		void assign_lightmap_data(lightmap_data const* data) noexcept
		{
			light_control.current = data;
		}
		void add_texture(unsigned int texture_width, unsigned int texture_height, void const* data)
		{
			if (!data) throw std::runtime_error("px::renderer::load_texture(...) - data is null");

			sprites.emplace_back();
			auto & batch = sprites.back();

			batch.texture.image2d(GL_RGBA, GL_RGBA, static_cast<GLsizei>(texture_width), static_cast<GLsizei>(texture_height), 0, GL_UNSIGNED_BYTE, data);
			batch.texture.filters(GL_NEAREST, GL_NEAREST); // required

			batch.vertices = GL_ARRAY_BUFFER;
			batch.geometry.swizzle(batch.vertices, sizeof(sprite_vertex), { GL_FLOAT, GL_FLOAT }, { 2, 2 }, { offsetof(sprite_vertex, position), offsetof(sprite_vertex, texture) });

			setup_batch(batch);
		}
		void zoom(bool up)
		{
			scale *= up ? 1.1f : 0.9f;
		}
		float get_scale()
		{
			return scale;
		}
		void set_scale(float pan)
		{
			scale = pan;
		}

	public:
		renderer(unsigned int width, unsigned int height)
			: sprite_data(nullptr)
			, scale(1.0f)
		{
			assign_size(width, height);

			// setup rendering

			create_resources();
			reset_framebuffers();
		}

	private:
		void create_resources()
		{
			// set states

			glClearColor(0, 0, 0, 1);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// compile shaders

			sprite_program = compile_program("data/shaders/sprite", { "Camera" }, { "img" });
			postprocess_program = compile_program("data/shaders/process", {}, { "diffuse", "lightmap" });
			//light_program = compile_program("data/shaders/light", { "Camera" }, { "current", "last" });
			light_program = compile_program("data/shaders/light", { "Camera" }, { "current" });

			// setup textures

			light_current.image2d(GL_RGBA, GL_RGBA, static_cast<GLsizei>(20), static_cast<GLsizei>(20));
			light_current.filters(GL_LINEAR, GL_NEAREST); // required
			light_last.image2d(GL_RGBA, GL_RGBA, static_cast<GLsizei>(20), static_cast<GLsizei>(20));
			light_last.filters(GL_LINEAR, GL_NEAREST); // required
		}
		void reset_framebuffers()
		{
			diffuse.setup(GL_RGBA32F, GL_RGBA, screen_width, screen_height);
			light.setup(GL_RGBA32F, GL_RGBA, screen_width, screen_height);

			postprocess.output(0, screen_width, screen_height);
			postprocess.bind_textures({ diffuse.texture, light.texture });

			light_pass.output(light.framebuffer, screen_width, screen_height);
			light_pass.bind_textures({ light_current /*, light_last*/ });
			light_pass.bind_uniform(camera);

			for (auto & batch : sprites) {
				setup_batch(batch);
			}
		}
		void assign_size(unsigned int new_width, unsigned int new_height)
		{
			screen_width = new_width;
			screen_height = new_height;
			screen_aspect = static_cast<float>(screen_width) / static_cast<float>(screen_height);
		}
		void setup_batch(sprite_batch & batch) const
		{
			batch.pass = { diffuse.framebuffer, batch.geometry, static_cast<GLsizei>(screen_width), static_cast<GLsizei>(screen_height) };
			batch.pass.bind_texture(batch.texture);
			batch.pass.bind_uniform(camera);
		}

	private:
		unsigned int					screen_width;
		unsigned int					screen_height;
		float							screen_aspect;
		gl_uniform						camera;
		gl_program						sprite_program;
		gl_program						postprocess_program;
		gl_program						light_program;

		gl_texture						light_current;
		gl_texture						light_last;

		offscreen						diffuse;
		offscreen						light;

		pass							postprocess;
		pass							light_pass;

		std::vector<sprite_batch>		sprites;
		float							scale;

		std::vector<std::vector<sprite_vertex>> const* sprite_data;
		lightmap_control				light_control;
	};
}