#pragma once

#include "gl_framebuffer.hpp"
#include "gl_texture.hpp"

namespace px {

	class offscreen {
	public:
		gl_framebuffer	framebuffer;
		gl_texture		texture;

	public:
		void setup(GLenum internal_format, GLenum format, GLsizei width, GLsizei height) {
			texture.image2d(internal_format, format, width, height);
			framebuffer.texture(texture);
		}
	};
}