// name: shell.hpp

#pragma once

#include "key.hpp"
#include "environment.hpp"

#include <px/es/engine.hpp>
#include <px/es/delta.hpp>

#include "es/sprite_component.hpp"
#include "es/transform_component.hpp"
#include "es/composite_component.hpp"

#include <px/fn/ant_generator.hpp>
#include <random>

#include <vector>

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
		void scroll(double horisontal, double vertical)
		{
			double total = horisontal + vertical;
			render.zoom(total > 0);
		}
		void press(key action_index)
		{
			switch (action_index) {
			case key::move_east: step({ 1, 0 }); break;
			case key::move_west: step({ -1, 0 }); break;
			case key::move_north: step({ 0, 1 }); break;
			case key::move_south: step({ 0, -1 }); break;
			}
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
			auto tr = transforms.make();
			tr->place({ 4, 5 });
			tr->store();
			auto spr = sprites.make("m_imp");
			spr->connect<transform_component>(tr.get());
			auto unit = make_uq<composite_component>();
			unit->add(std::move(tr));
			unit->add(std::move(spr));
			unit->enable();
			units.emplace_back(std::move(unit));

			tr = transforms.make();
			incarnate(tr.get());
			tr->place({ 5, 5 });
			tr->store();
			spr = sprites.make("m_darkness");
			spr->connect<transform_component>(tr.get());
			unit = make_uq<composite_component>();
			unit->add(std::move(tr));
			unit->add(std::move(spr));
			unit->enable();
			units.emplace_back(std::move(unit));

			tr = transforms.make();
			tr->place({ 6, 5 });
			tr->store();
			spr = sprites.make("m_succubus");
			spr->connect<transform_component>(tr.get());
			unit = make_uq<composite_component>();
			unit->add(std::move(tr));
			unit->add(std::move(spr));
			unit->enable();
			units.emplace_back(std::move(unit));

			auto map = fn::ant_generator::generate(std::mt19937{}, 10, 10, 10 * 10 * 100 / 61);
			map.enumerate([this] (size_t x, size_t y, unsigned char tile) {
				this->terrain.pset(tile == 0 ? 1 : 3, point2(static_cast<int>(x), static_cast<int>(y)));
			});

			run = true;
			time.restart();
		}

	private:
		bool				run;
		delta				time;
		engine<delta>		engine;

		std::vector<uq_ptr<composite_component>> units;
	};
}