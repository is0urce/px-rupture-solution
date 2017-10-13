#include "scene.hpp"

#include "scene_internal.hpp"

#include <px/memory/memory.hpp>

namespace px {

	scene::~scene()
	{
	}
	scene::scene()
		: works(make_uq<scene_internal>())
	{
	}

	bool scene::is_transparent(point2 const& location) const
	{
		return works->is_transparent(location);
	}
	bool scene::is_traversable(point2 const& location, rl::traverse_options<rl::traverse> const& opts) const
	{
		return works->is_traversable(location, opts);
	}

	void scene::add(uq_ptr<composite_component> && ptr)
	{
		works->add(std::forward<uq_ptr<composite_component>>(ptr));
	}

	void scene::assign_sprites(sprite_system * system)
	{
		works->assign_sprites(system);
	}

	void scene::pset(uint32_t block_id, point2 const& location)
	{
		works->pset(block_id, location);
	}
}