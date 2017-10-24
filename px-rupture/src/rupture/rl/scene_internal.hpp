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
		transform_component * any(point2 const& location) const
		{
			transform_component * subject = nullptr;
			space.find(location, [&](transform_component * pawn) {
				subject = pawn;
			});
			return subject;
		}
		body_component * anybody(point2 const& location) const
		{
			body_component * found = nullptr;
			space.find(location, [&](transform_component * pawn) {
				body_component * body = pawn->linked<body_component>();
				if (body) found = body;
			});
			return found;
		}
		uq_ptr<composite_component> & spawn(uq_ptr<composite_component> && ptr, transform_component * transform, point2 const& location)
		{
			if (!transform) transform = ptr->query<transform_component>();
			if (transform) {
				transform->place(location);
				transform->store();
				transform->incarnate(&space);
			}
			return insert(std::forward<uq_ptr<composite_component>>(ptr));
		}
		uq_ptr<composite_component> & spawn(uq_ptr<composite_component> && ptr, transform_component * transform)
		{
			if (!transform) transform = ptr->query<transform_component>();
			if (transform) {
				transform->incarnate(&space);
			}
			return insert(std::forward<uq_ptr<composite_component>>(ptr));
		}

		qtree<transform_component*> * get_space()
		{
			return &space;
		}
		void assign_sprites(sprite_system * system)
		{
			terrain.assign_sprites(system);
		}
		void pset(std::uint32_t block_id, point2 const& location)
		{
			terrain.pset(block_id, location);
		}
		template <typename Operator>
		void discard(Operator && predicate) {
			size_t i = 0;
			size_t size = units.size();
			while (i != size) {
				if (predicate(*units[i])) {
					units[i] = std::move(units.back());
					units.pop_back();
					--size;
				}
				else {
					++i;
				}
			}
		}

	public:
		scene_internal()
			: space(64)
		{
		}
		uq_ptr<composite_component> & insert(uq_ptr<composite_component> && ptr)
		{
			ptr->enable();
			units.push_back(std::move(ptr));
			return units.back();
		}

	private:
		qtree<transform_component*>					space;
		terrain_internal							terrain;
		std::vector<uq_ptr<composite_component>>	units;
	};
}