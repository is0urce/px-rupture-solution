#pragma once

#include "key.hpp"

#include <px/es/engine.hpp>
#include <px/es/delta.hpp>

namespace px {
	class shell
	{
	public:
		void frame(double timer)
		{
			delta_time.advance(timer);
			engine.update(delta_time);

			++delta_time.tick_index;
			engine.tick_update(delta_time);
		}
		void resize(int screen_width, int screen_height)
		{
			width = screen_width;
			height = screen_height;
		}
		void text(unsigned int /*codepoint*/)
		{

		}
		void click(int /*mouse_button*/)
		{

		}
		void hover(int /*x*/, int /*y*/)
		{

		}
		void scroll(double /*horisontal*/, double /*vertical*/)
		{

		}
		void press(key /*action_index*/)
		{

		}
	public:
		~shell()
		{

		}
		shell(int start_widht, int start_height)
			: width(start_widht)
			, height(start_height)
		{
			delta_time.restart();
		}

	private:
		int				width;
		int				height;
		engine<delta>	engine;
		delta			delta_time;
	};
}