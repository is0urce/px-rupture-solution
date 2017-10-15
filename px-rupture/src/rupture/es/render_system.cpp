// name: render_system.cpp

#include "render_system.hpp"

#include "rupture/draw/renderer.hpp"

#include <px/memory/memory.hpp>

namespace px {

	render_system::~render_system()
	{
	}
	render_system::render_system(unsigned int width, unsigned int height)
		: works(make_uq<renderer>(width, height))
	{
	}

	void render_system::update_system(delta_type const& delta_time)
	{
		works->run(delta_time.turn_duration);
	}

	void render_system::resize(int width, int height)
	{
		render_system::works->resize(width, height);
	}
	void render_system::assign_sprite_data(std::vector<std::vector<sprite_vertex>> const* data) noexcept
	{
		works->assign_sprite_data(data);
	}
	
	void render_system::assign_lightmap_data(lightmap_data const* current, lightmap_data const* last) noexcept
	{
		works->assign_lightmap_data(current, last);
	}
	void render_system::assigm_message_data(message_data const* data) noexcept
	{
		works->assign_message_data(data);
	}
	void render_system::add_texture(unsigned int texture_width, unsigned int texture_height, void const* data)
	{
		works->add_texture(texture_width, texture_height, data);
	}
	void render_system::zoom(bool is_up)
	{
		works->zoom(is_up);
	}
}