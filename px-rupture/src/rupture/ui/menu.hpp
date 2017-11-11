// name: menu.hpp
// typc: c++ header

#pragma once

#include "panel.hpp"
#include <px/memory/uq_ptr.hpp>

#include <vector>

namespace px {

	class container_component;
	class director;
	class environment;
	class inventory;
	class craft;

	class menu {
	public:
		void resize(unsigned int w, unsigned int h);
		void draw(double delta_time);
		bool click(unsigned int mouse_button, bool is_down);
		bool text(unsigned int codepoint);
		bool hover(unsigned int x, unsigned int y);
		bool scroll(double horisontal, double vertical);
		bool takes_input();
		void toggle_inventory();
		void rollback(); // close active inventory trasfers

	public:
		~menu();
		menu(unsigned int w, unsigned int h, environment * game);

	private:
		void combine();

	private:
		environment	*					context;
		uq_ptr<director>				nexus;
		std::vector<uq_ptr<panel>>		stack;
		environment *					game;
		bool							inventory_open;
		inventory *						inventory_panel;
		craft *							craft_panel;
	};
}