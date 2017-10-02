// name: sprite_batch.hpp
// type: c++
// auth: is0urce
// desc: structure

#pragma once

#include <px/rgl/rgl.hpp>

namespace px {

	struct sprite_batch
	{
		gl_buffer	vertices;
		gl_vao		geometry;
		gl_texture	texture;
		gl_pass		pass;

	public:
		template <typename T>
		void draw_arrays(GLenum mode, GLenum usage, size_t size, T const* data)
		{
			vertices.load(usage, sizeof(T) * size, data);
			pass.draw_arrays(mode, static_cast<GLsizei>(size));
		}
	};

}