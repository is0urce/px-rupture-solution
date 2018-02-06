// name: menu.cpp
// typc: c++

#include "menu.hpp"

#include "director.hpp"
#include "panel.hpp"

#include "craft_smith.hpp"
#include "craft_alchemy.hpp"
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

	// ctor & dtor

	menu::~menu() = default;
	menu::menu(unsigned int w, unsigned int h, environment * game)
		: context(game)
		, nexus(make_uq<director>(w, h))
		, inventory_open(false)
		, inventory_panel(nullptr)
		, smith_panel(nullptr)
		, alchemy_panel(nullptr)
	{
		make_panel<ui::skills>(stack, game);
		make_panel<ui::status>(stack, game);
		make_panel<ui::inspector>(stack, game);

		make_panel<ui::performance>(stack);
		make_panel<ui::editor>(stack, game);

		inventory_panel = make_panel<inventory>(stack, game, &inventory_open);
		smith_panel = make_panel<craft_smith>(stack, game);
		alchemy_panel = make_panel<craft_alchemy>(stack, game);
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
		smith_panel->cancel_task();
		alchemy_panel->cancel_task();
		inventory_open = !inventory_open;
	}
	void menu::combine() {
		for (auto & ptr : stack) {
			ptr->combine();
		}
	}
	void menu::rollback() {
		smith_panel->cancel_task();
		alchemy_panel->cancel_task();
		inventory_open = false;
	}
}