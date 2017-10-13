#pragma once

#include <px/common/qtree.hpp>
#include <px/memory/uq_ptr.hpp>

#include "rupture/es/transform_component.hpp"
#include "rupture/es/composite_component.hpp"

#include "terrain_internal.hpp"

#include <vector>

namespace px {

	class scene_internal
	{
	public:
		bool is_transparent(point2 const& location) const
		{
			if (!terrain.is_transparent(location)) return false;

			return true;
		}
		bool is_traversable(point2 const& location, rl::traverse_options<rl::traverse> const& opts) const
		{
			if (!terrain.is_traversable(location, opts)) return false;

			return true;
		}
		void add(uq_ptr<composite_component> && ptr)
		{
			units.push_back(std::move(ptr));
		}
		qtree<transform_component*> * get_space()
		{
			return &space;
		}
		void assign_sprites(sprite_system * system)
		{
			terrain.assign_sprites(system);
		}
		void pset(uint32_t block_id, point2 const& location)
		{
			terrain.pset(block_id, location);
		}

	public:
		scene_internal()
			: space(64)
		{
		}

	private:
		qtree<transform_component*>					space;
		terrain_internal							terrain;
		std::vector<uq_ptr<composite_component>>	units;
	};
}