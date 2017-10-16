#pragma once

#include <px/common/qtree.hpp>
#include <px/memory/uq_ptr.hpp>

#include "rupture/es/transform_component.hpp"
#include "rupture/es/composite_component.hpp"
#include "rupture/es/body_component.hpp"

#include "terrain_internal.hpp"

#include <vector>

namespace px {

	class scene_internal
	{
	public:
		bool is_transparent(point2 const& location) const
		{
			if (!terrain.is_transparent(location)) return false;

			bool transparent = true;
			space.find(location, [&](transform_component * pawn) {
				body_component * body = pawn->linked<body_component>();
				transparent &= !body || body->blocking().is_transparent();
			});

			return transparent;
		}
		bool is_traversable(point2 const& location, rl::traverse_options<rl::traverse> const& opts) const
		{
			if (!terrain.is_traversable(location, opts)) return false;

			bool traversable = true;
			space.find(location, [&](transform_component * pawn) {
				body_component * body = pawn->linked<body_component>();
				traversable &= !body || body->blocking().is_traversable(opts);
			});

			return traversable;
		}
		void spawn(uq_ptr<composite_component> && ptr, transform_component * transform, point2 const& location)
		{
			if (!transform) transform = ptr->query<transform_component>();
			if (transform) {
				transform->place(location);
				transform->store();
				transform->incarnate(&space);
			}
			insert(std::forward<uq_ptr<composite_component>>(ptr));
		}
		void spawn(uq_ptr<composite_component> && ptr, transform_component * transform)
		{
			if (!transform) transform = ptr->query<transform_component>();
			if (transform) {
				transform->incarnate(&space);
			}
			insert(std::forward<uq_ptr<composite_component>>(ptr));
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
		void insert(uq_ptr<composite_component> && ptr)
		{
			ptr->enable();
			units.push_back(std::move(ptr));
		}

	private:
		qtree<transform_component*>					space;
		terrain_internal							terrain;
		std::vector<uq_ptr<composite_component>>	units;
	};
}