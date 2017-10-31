// name: scene.cpp

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

	uq_ptr<composite_component> & scene::spawn(uq_ptr<composite_component> && ptr, transform_component * transform, point2 const& location)
	{
		return works->spawn(std::forward<uq_ptr<composite_component>>(ptr), transform, location);
	}
	uq_ptr<composite_component> & scene::spawn(uq_ptr<composite_component> && ptr, transform_component * transform)
	{
		return works->spawn(std::forward<uq_ptr<composite_component>>(ptr), transform);
	}

	void scene::assign_sprites(sprite_system * system)
	{
		works->assign_sprites(system);
	}

	void scene::pset(std::uint32_t block_id, point2 const& location)
	{
		works->pset(block_id, location);
	}

	qtree<transform_component*> * scene::space() noexcept
	{
		return works->get_space();
	}

	transform_component * scene::any(point2 const& location) const
	{
		return works->any(location);
	}
	body_component * scene::anybody(point2 const& location) const
	{
		return works->anybody(location);
	}
	void scene::discard(std::function<void(uq_ptr<composite_component> &)> predicate)
	{
		works->discard(predicate);
	}
}