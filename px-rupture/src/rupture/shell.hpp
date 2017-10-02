// name: shell.hpp

#pragma once

#include "key.hpp"
#include "environment.hpp"
#include "draw/renderer.hpp"

#include <px/es/engine.hpp>
#include <px/es/delta.hpp>

#include "es/sprite_system.hpp"
#include "es/transform_system.hpp"

#include "es/sprite_component.hpp"
#include "es/transform_component.hpp"

#include <lodepng.h>

namespace px {

	class shell final
		: public environment
	{
	public:
		void frame(double timer)
		{
			time.advance(timer);
			engine.update(time);

			++time.tick_index;
			engine.tick_update(time);
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
			engine.add(&sprites);
			engine.add(&transforms);

			std::vector<unsigned char> texture_data;
			unsigned int texture_width;
			unsigned int texture_height;
			auto error = lodepng::decode(texture_data, texture_width, texture_height, "data/img/monsters.png");
			if (error) throw std::runtime_error("error while loading image");
			render.add_texture(texture_width, texture_height, texture_data.data());
			render.assign_batch_data(sprites.data());

			sprites.target(nullptr);

			tr.place({ 0, 0 });
			tr.store();

			sprite = sprites.make("rat");
			sprite->activate();
			sprite->connect<transform_component>(&tr);

			time.restart();
		}

	private:
		int					width;
		int					height;
		delta				time;
		engine<delta>		engine;
		sprite_system		sprites;
		transform_system	transforms;
		renderer			render;

		uq_ptr<sprite_component> sprite;
		transform_component tr;
	};
}