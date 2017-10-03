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
#include "es/composite_component.hpp"

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
		shell(unsigned int start_widht, unsigned int start_height)
			: width(start_widht)
			, height(start_height)
			, render(start_widht, start_height)
		{
			engine.add(&sprites);
			engine.add(&transforms);

			add_texture("data/img/monsters.png");
			render.assign_batch_data(sprites.data());

			sprites.target(nullptr);

			time.restart();

			auto tr = transforms.make();
			tr->place({ 0, 0 });
			tr->store();
			auto spr = sprites.make("rat");
			spr->connect<transform_component>(tr.get());
			unit = make_uq<composite_component>();
			unit->add(std::move(tr));
			unit->add(std::move(spr));
			unit->enable();
		}
	private:
		void add_texture(std::string const& name)
		{
			std::vector<unsigned char> bits;
			unsigned int texture_width;
			unsigned int texture_height;
			auto error = lodepng::decode(bits, texture_width, texture_height, name);
			if (error) throw std::runtime_error("add_texture error while loading image, path=" + name);
			render.add_texture(texture_width, texture_height, bits.data());
		}

	private:
		unsigned int		width;
		unsigned int		height;
		delta				time;
		engine<delta>		engine;
		sprite_system		sprites;
		transform_system	transforms;
		renderer			render;

		uq_ptr<composite_component> unit;
	};
}