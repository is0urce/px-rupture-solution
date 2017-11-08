// name: menu.cpp
// typc: c++

#include "menu.hpp"

#include "director.hpp"
#include "panel.hpp"

#include "craft.hpp"
#include "editor.hpp"
#include "inspector.hpp"
#include "inventory.hpp"
#include "performance.hpp"
#include "status.hpp"
#include "skills.hpp"

#include <px/memory/memory.hpp>

namespace px {

	// aux

	namespace {
		template <typename M, typename ... Args>
		M * make_panel(std::vector<uq_ptr<panel>> & c, Args... args) {
			auto p = make_uq<M>(std::forward<Args>(args)...);
			M * ptr = p.get();
			c.emplace_back(std::move(p));
			return ptr;
		}
	}

	// ctor

	menu::menu(unsigned int w, unsigned int h, environment * game)
		: nexus(make_uq<director>(w, h))
		, inventory_open(false)
		, inventory_panel(nullptr)
		, craft_panel(nullptr)
	{
		make_panel<ui::skills>(stack, game);
		make_panel<ui::status>(stack, game);
		make_panel<ui::inspector>(stack, game);

		make_panel<ui::performance>(stack);
		make_panel<ui::editor>(stack, game);

		inventory_panel = make_panel<inventory>(stack, game, &inventory_open);
		craft_panel = make_panel<craft>(stack, game);
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
	void menu::rollback() {
		inventory_open = false;
		craft_panel->rollback();
	}
}