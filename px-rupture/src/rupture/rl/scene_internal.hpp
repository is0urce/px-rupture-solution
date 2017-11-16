// name: scene_internal.hpp

#pragma once

#include "rupture/es/transform_component.hpp"
#include "rupture/es/composite_component.hpp"
#include "rupture/es/body_component.hpp"

#include "terrain_patch.hpp"
#include "terrain_surface.hpp"

#include <px/common/qtree.hpp>
#include <px/memory/uq_ptr.hpp>
#include <px/memory/abstract_release_block.hpp>

#include <px/fn/ant_generator.hpp>

#include <random>
#include <vector>

namespace px {

	class scene_internal {
	public:
		static const size_t terrain_len = 20;
		typedef terrain_surface<terrain_len, 1> surface_type;
		typedef typename surface_type::stream_type stream_type;

		// nested
		class join_block;
		class terrain_release_block final
			: public abstract_release_block
		{
		public:
			terrain_release_block(join_block * raw, scene_internal * current)
				: original(raw)
				, scene(current)
			{
			}

		protected:
			virtual void release_block() noexcept override {
				scene->leave(original->get_value()->data().cell());
				delete original;
			}

		private:
			join_block *		original;
			scene_internal *	scene;
		};

		// nested
		class join_block final {
		public:
			stream_type * get_value() noexcept {
				return &stream;
			}
			abstract_release_block * get_control() noexcept {
				return &ctrl;
			}

		public:
			join_block(scene_internal * scene)
				: ctrl(this, scene)
			{
			}
		private:
			stream_type				stream;
			terrain_release_block	ctrl;
		};

	public:
		void save_terrain() {
		}
		void assign_sprites(sprite_system * system) {
			sprites = system;
		}
		void leave(point2 const& /*cell*/) {

		}
		void enter(point2 const& grid) {

			point2 start = point2(terrain_len, terrain_len) * grid;

			auto map = fn::ant_generator::generate(std::mt19937{}, terrain_len, terrain_len, terrain_len * terrain_len * 100 / 61);
			map.enumerate([&](size_t x, size_t y, unsigned char tile) {
				point2 relative = point2(static_cast<int>(x), static_cast<int>(y));
				pset(tile == 0 ? 1 : 2, start + relative);
			});
		}

		void clear() {
			units.clear();
		}
		size_t size() const {
			return units.size();
		}
		void focus(point2 const& world) {
			surface.focus(world);
			surface.enumerate([&](point2 const& cell, auto & ptr) {
				if (!ptr) {
					join_block * join = new join_block(this);
					ptr.reset(join->get_value(), join->get_control());

					ptr->load([&](auto & chunk) {
						chunk.assign_sprites(sprites);
						chunk.assign_cell(cell);
					});

					enter(cell);
				}
			});
		}

		bool is_transparent(point2 const& location) const {
			if (!surface.is_transparent(location)) return false;

			bool transparent = true;
			space.find(location, [&](transform_component * pawn) {
				body_component * body = pawn->linked<body_component>();
				transparent &= !body || body->blocking().is_transparent();
			});

			return transparent;
		}
		bool is_traversable(point2 const& location, rl::traverse_options<rl::traverse> const& opts) const {
			if (!surface.is_traversable(location, opts)) return false;

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
			surface.pset(block_id, location);
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
		std::vector<uq_ptr<composite_component>>	units;
		surface_type								surface;
		sprite_system *								sprites;
	};
}