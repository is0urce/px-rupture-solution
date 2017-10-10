// name: ui_system.cpp

#include "ui_system.hpp"

#include "rupture/menu.hpp"

#include <px/memory/memory.hpp>

namespace px {

	ui_system::~ui_system()
	{
	}
	ui_system::ui_system(unsigned int width, unsigned int height)
		: works(make_uq<menu>(width, height))
	{
	}

	void ui_system::update_system(delta_type const& /*delta_time*/)
	{
		works->draw();
	}

	void ui_system::toggle(bool visible)
	{
		works->toggle(visible);
	}

}