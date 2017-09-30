// name: shell.hpp

#pragma once

#include "key.hpp"
#include "environment.hpp"
#include "draw/renderer.hpp"

#include <px/es/engine.hpp>
#include <px/es/delta.hpp>

#include "es/sprite_system.hpp"
#include "es/transform_system.hpp"

namespace px {

	class shell final
		: public environment
	{
	public:
		void frame(double timer)
		{
			delta_time.advance(timer);
			engine.update(delta_time);

			++delta_time.tick_index;
			engine.tick_update(delta_time);
			render.run();
		}
		void resize(int screen_width, int screen_height)
		{
			width = screen_width;
			height = screen_height;
			render.resize(width, height);
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
			, render(start_widht, start_height)
		{
			delta_time.restart();
			engine.add(&sprites);
			engine.add(&transforms);
		}

	private:
		int					width;
		int					height;
		engine<delta>		engine;
		delta				delta_time;
		renderer			render;
		sprite_system		sprites;
		transform_system	transforms;
	};
}