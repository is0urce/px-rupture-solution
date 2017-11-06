// name: menu.cpp
// typc: c++

#include "menu.hpp"

#include "director.hpp"
#include "panel.hpp"

#include "editor.hpp"
#include "inspector.hpp"
#include "inventory.hpp"
#include "performance.hpp"
#include "status.hpp"
#include "skills.hpp"

#include <px/memory/memory.hpp>

namespace px {

	// ctor

	menu::menu(unsigned int w, unsigned int h, environment * game)
		: nexus(make_uq<director>(w, h))
		, inventory_open(false)
	{

		stack.emplace_back(make_uq<ui::skills>(game));
		stack.emplace_back(make_uq<ui::status>(game));
		stack.emplace_back(make_uq<ui::inspector>(game));

		stack.emplace_back(make_uq<ui::performance>());
		stack.emplace_back(make_uq<ui::editor>(game));

		auto inv = make_uq<inventory>(game, &inventory_open);
		inventory_panel = inv.get();
		stack.emplace_back(std::move(inv));
	}

	// dtor

	menu::~menu() {
	}

	// methods

	void menu::resize(unsigned int w, unsigned int h) {
		nexus->resize(w, h);
	}
	void menu::draw(double delta_time) {
		nexus->begin(delta_time);
		combine();
		nexus->end();
	}
	bool menu::click(unsigned int mouse_button, bool is_down) {
		return nexus->click(mouse_button, is_down);
	}
	bool menu::text(unsigned int codepoint) {
		return nexus->text(codepoint);
	}
	bool menu::hover(unsigned int x, unsigned int y) {
		return nexus->hover(x, y);
	}
	bool menu::scroll(double horisontal, double vertical) {
		return nexus->scroll(horisontal, vertical);
	}
	bool menu::takes_input() {
		return nexus->takes_input();
	}
	void menu::toggle_inventory() {
		inventory_open = !inventory_open;
	}
	void menu::combine() {
		for (auto & ptr : stack) {
			ptr->combine();
		}
	}
}