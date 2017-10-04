// name: renderer.hpp
// type: c++
// auth: is0urce
// desc: rendering procedures

#pragma once

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
			camera.load<camera_uniform>(GL_STREAM_DRAW, { { 1.0f, 1.0f * screen_aspect },{ 0.0, 0.0 } });

			glUseProgram(sprite_program);
			if (vertices && vertices->size() > 0) {
				sprites.draw_arrays(GL_QUADS, GL_STREAM_DRAW, vertices->size(), vertices->data());
			}

			gl_assert();
		}
		void resize(int width, int height)
		{
			screen_width = width;
			screen_height = height;
			screen_aspect = static_cast<float>(screen_width) / static_cast<float>(screen_height);

			reset_framebuffers();
		}
		void assign_sprite_data(std::vector<sprite_vertex> const* data) noexcept
		{
			vertices = data;
		}
		void add_texture(unsigned int texture_width, unsigned int texture_height, void const* data)
		{
			if (!data) throw std::runtime_error("px::renderer::load_texture(...) - data is null");

			auto & batch = sprites;

			batch.texture.image2d(GL_RGBA, GL_RGBA, static_cast<GLsizei>(texture_width), static_cast<GLsizei>(texture_height), 0, GL_UNSIGNED_BYTE, data);
			batch.texture.filters(GL_NEAREST, GL_NEAREST); // required

			batch.vertices = GL_ARRAY_BUFFER;
			batch.geometry.swizzle(batch.vertices, sizeof(sprite_vertex), { GL_FLOAT, GL_FLOAT }, { 2, 2 }, { offsetof(sprite_vertex, position), offsetof(sprite_vertex, texture) });

			batch.pass = { 0, batch.geometry, static_cast<GLsizei>(screen_width), static_cast<GLsizei>(screen_height) };
			batch.pass.bind_texture(batch.texture);
			batch.pass.bind_uniform(camera);
		}

	public:
		renderer(unsigned int width, unsigned int height)
			: screen_width(width)
			, screen_height(height)
		{
			// setup rendering

			create_pipeline();
			reset_framebuffers();
		}

	private:
		void create_pipeline()
		{
			// set states

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);

			// compile shaders

			sprite_program = compile_program("data/shaders/sprite", { "Camera" }, { "img" });
		}
		void reset_framebuffers()
		{
			auto & batch = sprites;

			batch.pass = { 0, batch.geometry, static_cast<GLsizei>(screen_width), static_cast<GLsizei>(screen_height) };
			batch.pass.bind_texture(batch.texture);
			batch.pass.bind_uniform(camera);
		}

	private:
		unsigned int	screen_width;
		unsigned int	screen_height;
		float			screen_aspect;
		gl_uniform		camera;
		gl_program		sprite_program;
		sprite_batch	sprites;
		std::vector<sprite_vertex> const* vertices;
	};
}