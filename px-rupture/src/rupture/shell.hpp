// name: shell.hpp

#pragma once

#include "key.hpp"
#include "environment.hpp"

#include <px/es/engine.hpp>
#include <px/es/delta.hpp>

#include "es/sprite_component.hpp"
#include "es/transform_component.hpp"
#include "es/composite_component.hpp"

namespace px {

	class shell final
		: public environment
	{
	public:
		void frame(double timer)
		{
			if (run) {
				time.advance(timer);
				engine.update(time);

				++time.tick_index;
				engine.tick_update(time);
				render.run();
			}
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
			: environment(start_widht, start_height)
			, run(true)
		{
			register_systems();

			auto tr = transforms.make();
			tr->place({ 0, 0 });
			tr->store();
			auto spr = sprites.make("m_imp");
			spr->connect<transform_component>(tr.get());
			unit = make_uq<composite_component>();
			unit->add(std::move(tr));
			unit->add(std::move(spr));
			unit->enable();

			start();
		}

	private:
		void register_systems()
		{
			engine.add(&sprites);
			engine.add(&transforms);
		}
		void start()
		{
			run = true;
			sprites.target(nullptr);
			time.restart();
		}

	private:
		bool				run;
		delta				time;
		engine<delta>		engine;

		uq_ptr<composite_component> unit;
	};
}