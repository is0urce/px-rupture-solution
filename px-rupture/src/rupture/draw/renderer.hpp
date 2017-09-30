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

namespace px {

	class renderer
	{
	public:
		void run()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glClearColor(1, 1, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			gl_assert();
		}
		void resize(int size_width, int size_height)
		{
			width = size_width;
			height = size_height;

			reset_framebuffers();
		}

	public:
		renderer(int width, int height)
			: width(width)
			, height(height)
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
		}
		void reset_framebuffers()
		{
		}

	private:
		int	width;
		int	height;
	};
}