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

#include "sprite_vertex.hpp"
#include "sprite_batch.hpp"
#include "camera_uniform.hpp"

#include <vector>
#include <stdexcept>

namespace px {

	class renderer
	{
	public:
		struct camera_data;

	public:
		void run()
		{
			camera.load<camera_uniform>(GL_STREAM_DRAW, { { scale, scale * screen_aspect },{ 0.0, 0.0 } });

			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(sprite_program);
			if (sprite_data) {
				size_t size = sprite_data->size();
				for (size_t i = 0; i != size; ++i) {
					std::vector<sprite_vertex> const& vertices = sprite_data->at(i);
					if (vertices.size() > 0) {
						sprites[i].draw_arrays(GL_QUADS, GL_STREAM_DRAW, vertices.size(), vertices.data());
					}
				}
			}

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
		void add_texture(unsigned int texture_width, unsigned int texture_height, void const* data)
		{
			if (!data) throw std::runtime_error("px::renderer::load_texture(...) - data is null");

			sprites.emplace_back();
			auto & batch = sprites.back();

			batch.texture.image2d(GL_RGBA, GL_RGBA, static_cast<GLsizei>(texture_width), static_cast<GLsizei>(texture_height), 0, GL_UNSIGNED_BYTE, data);
			batch.texture.filters(GL_NEAREST, GL_NEAREST); // required

			batch.vertices = GL_ARRAY_BUFFER;
			batch.geometry.swizzle(batch.vertices, sizeof(sprite_vertex), { GL_FLOAT, GL_FLOAT }, { 2, 2 }, { offsetof(sprite_vertex, position), offsetof(sprite_vertex, texture) });

			batch.pass = { 0, batch.geometry, static_cast<GLsizei>(screen_width), static_cast<GLsizei>(screen_height) };
			batch.pass.bind_texture(batch.texture);
			batch.pass.bind_uniform(camera);
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

			create_pipeline();
			reset_framebuffers();
		}

	private:
		void create_pipeline()
		{
			// set states

			glClearColor(1, 1, 0, 1);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);

			// compile shaders

			sprite_program = compile_program("data/shaders/sprite", { "Camera" }, { "img" });
		}
		void reset_framebuffers()
		{
			for (auto & batch : sprites) {
				batch.pass = { 0, batch.geometry, static_cast<GLsizei>(screen_width), static_cast<GLsizei>(screen_height) };
				batch.pass.bind_texture(batch.texture);
				batch.pass.bind_uniform(camera);
			}
		}
		void assign_size(unsigned int new_width, unsigned int new_height)
		{
			screen_width = new_width;
			screen_height = new_height;
			screen_aspect = static_cast<float>(screen_width) / static_cast<float>(screen_height);
		}

	private:
		unsigned int					screen_width;
		unsigned int					screen_height;
		float							screen_aspect;
		gl_uniform						camera;
		gl_program						sprite_program;
		std::vector<sprite_batch>		sprites;
		float							scale;

		std::vector<std::vector<sprite_vertex>> const* sprite_data;
	};
}