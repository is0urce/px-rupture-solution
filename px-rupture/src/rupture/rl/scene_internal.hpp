// name: scene_internal.hpp

#pragma once

#include "rupture/es/transform_component.hpp"
#include "rupture/es/composite_component.hpp"
#include "rupture/es/body_component.hpp"

#include "terrain_patch.hpp"
#include "terrain_surface.hpp"

#include <px/common/qtree.hpp>
#include <px/memory/uq_ptr.hpp>

#include <vector>

namespace px {

	class scene_internal {
	private:
		static const size_t terrain_len = 50;

	public:
		void assign_sprites(sprite_system * system) {
			terrain.assign_sprites(system);
		}

		void clear() {
			units.clear();
		}
		size_t size() const {
			return units.size();
		}

		bool is_transparent(point2 const& location) const {
			if (!terrain.is_transparent(location)) return false;

			bool transparent = true;
			space.find(location, [&](transform_component * pawn) {
				body_component * body = pawn->linked<body_component>();
				transparent &= !body || body->blocking().is_transparent();
			});

			return transparent;
		}
		bool is_traversable(point2 const& location, rl::traverse_options<rl::traverse> const& opts) const {
			if (!terrain.is_traversable(location, opts)) return false;

			bool traversable = true;
			space.find(location, [&](transform_component * pawn) {
				body_component * body = pawn->linked<body_component>();
				traversable &= !body || body->blocking().is_traversable(opts);
			});

			return traversable;
		}
		transform_component * any(point2 const& location) const {
			transform_component * subject = nullptr;
			space.find(location, [&](transform_component * pawn) {
				subject = pawn;
			});
			return subject;
		}
		body_component * anybody(point2 const& location) const {
			body_component * found = nullptr;
			space.find(location, [&](transform_component * pawn) {
				body_component * body = pawn->linked<body_component>();
				if (body) found = body;
			});
			return found;
		}
		uq_ptr<composite_component> & spawn(uq_ptr<composite_component> && ptr) {
			transform_component * pawn = ptr->linked<transform_component>();
			if (pawn) {
				pawn->incarnate(&space);
			}
			return insert(std::forward<uq_ptr<composite_component>>(ptr));
		}

		qtree<transform_component*> * get_space() {
			return &space;
		}
		void pset(std::uint32_t block_id, point2 const& location) {
			terrain.pset(block_id, location);
		}
		template <typename Operator>
		void discard(Operator && message_function) {
			size_t i = 0;
			size_t size = units.size();
			while (i != size) {
				if (units[i]->decayed()) {
					message_function(*units[i]);
					units[i] = std::move(units.back());
					units.pop_back();
					--size;
				}
				else {
					++i;
				}
			}
		}
		template <typename Operator>
		void enumerate(Operator && function) const {
			for (uq_ptr<composite_component> const& unit : units) {
				function(*unit);
			}
		}
		template <typename Operator>
		void enumerate(Operator && function) {
			for (uq_ptr<composite_component> & unit : units) {
				function(*unit);
			}
		}

	public:
		scene_internal()
			: space(64)
		{
		}
		uq_ptr<composite_component> & insert(uq_ptr<composite_component> && ptr) {
			ptr->enable();
			units.push_back(std::move(ptr));
			return units.back();
		}

	private:
		qtree<transform_component*>					space;
		terrain_patch<terrain_len, terrain_len>		terrain;
		std::vector<uq_ptr<composite_component>>	units;
	};
}