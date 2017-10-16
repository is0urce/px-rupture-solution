// name: ui_system.cpp

#include "ui_system.hpp"

#include "rupture/ui/menu.hpp"

#include <px/memory/memory.hpp>

namespace px {

	ui_system::~ui_system()
	{
	}
	ui_system::ui_system(unsigned int width, unsigned int height)
		: works(make_uq<menu>(width, height))
	{
	}

	void ui_system::update_system(delta_type const& delta_time)
	{
		works->draw(delta_time.real_delta);
	}

	void ui_system::resize(unsigned int w, unsigned int h)
	{
		works->resize(w, h);
	}

	bool ui_system::click(unsigned int mouse_button, bool is_down)
	{
		return works->click(mouse_button, is_down);
	}
	bool ui_system::text(unsigned int codepoint)
	{
		return works->text(codepoint);
	}
	bool ui_system::hover(unsigned int x, unsigned int y)
	{
		return works->hover(x, y);
	}
	bool ui_system::scroll(double horisontal, double vertical)
	{
		return works->scroll(horisontal, vertical);
	}
	bool ui_system::takes_input()
	{
		return works->takes_input();
	}
}