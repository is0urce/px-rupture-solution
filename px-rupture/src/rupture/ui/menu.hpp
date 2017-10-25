// name: menu.hpp
// typc: c++ header

#pragma once

#include "director_works.hpp"
#include "panel.hpp"

#include "editor.hpp"
#include "inspector.hpp"
#include "inventory.hpp"
#include "performance.hpp"
#include "status.hpp"
#include "skills.hpp"

#include <px/memory/memory.hpp>

#include <vector>

namespace px {

	class menu final
	{
	public:
		void resize(unsigned int w, unsigned int h)
		{
			director.resize(w, h);
		}
		void draw(double delta_time)
		{
			director.begin(delta_time);
			combine();
			director.end();
		}
		bool click(unsigned int mouse_button, bool is_down)
		{
			return director.click(mouse_button, is_down);
		}
		bool text(unsigned int codepoint)
		{
			return director.text(codepoint);
		}
		bool hover(unsigned int x, unsigned int y)
		{
			return director.hover(x, y);
		}
		bool scroll(double horisontal, double vertical)
		{
			return director.scroll(horisontal, vertical);
		}
		bool takes_input()
		{
			return director.takes_input();
		}
		//void open_inventory(container_component * cont);

	public:
		~menu()
		{
		}
		menu(unsigned int w, unsigned int h, environment * game)
			: director(w, h)
			, inventory_open(true)
		{
			stack.emplace_back(make_uq<ui::skills>(game));
			stack.emplace_back(make_uq<ui::status>(game));
			stack.emplace_back(make_uq<ui::inspector>(game));

			stack.emplace_back(make_uq<ui::performance>());
			stack.emplace_back(make_uq<ui::editor>(game));
			stack.emplace_back(make_uq<ui::inventory>(game, &inventory_open));
			//inventory = stack.back().get();
		}

	private:
		void combine()
		{
			for (auto & panel : stack) {
				panel->combine();
			}
		}

	private:
		ui::director_works				director;
		std::vector<uq_ptr<ui::panel>>	stack;
		environment *					game;
		bool							inventory_open;
		ui::inventory *					inventory;
	};
}