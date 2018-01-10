// name: scene_internal.hpp
// type: c++

#pragma once

#include "rupture/es/transform_component.hpp"
#include "rupture/es/composite_component.hpp"
#include "rupture/es/body_component.hpp"

#include "terrain_patch.hpp"
#include "terrain_surface.hpp"

#include <px/common/point.hpp>
#include <px/common/qtree.hpp>
#include <px/memory/uq_ptr.hpp>
#include <px/memory/abstract_release_block.hpp>

#include <px/fn/ant_generator.hpp>

#include <random>
#include <string>
#include <vector>

namespace px {

	class scene_internal {
	public:
		static const size_t terrain_len = 25;

	public:
		using surface_type = terrain_surface<terrain_len, 1>;
		using patch_type = typename surface_type::patch_type;
		using stream_type = typename surface_type::stream_type;

	private:
		class										join_release_block;
		friend class								terrain_release_block;

	public:
		void assign_sprites(sprite_system * system) {
			sprites = system;
		}

		void clear() {
			units.clear();
		}

		size_t size() const {
			return units.size();
		}

		void focus(point2 const& world) {
			surface.focus(world);
			surface.enumerate([&](point2 const& grid_cell, auto & ptr) {
				if (!ptr) {
					join_release_block * join = new join_release_block(this, grid_cell); // on_leave sewed into deleter
					ptr.reset(join->get_value(), join->get_control());

					on_enter(grid_cell, *ptr);
				}
			});
			//export_terrain();
		}

		void pset(std::uint32_t block_id, point2 const& location) {
			surface.pset(block_id, location);
		}

		void export_terrain() {
			surface.enumerate([&](point2 const& location, auto & ptr) {
				if (ptr && ptr->is_loaded()) {
					ptr->data().write(depot_name(location));
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
			ptr->enable();
			units.push_back(std::move(ptr));
			return units.back();
		}

		qtree<transform_component*> * get_space() {
			return &space;
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

	private:
		std::string depot_name(point2 const& grid) const {
			return settings::terrrain_path + std::to_string(grid.x()) + "_" + std::to_string(grid.y()) + ".dat";
		}
		void on_leave(point2 const& /*grid*/, stream_type & /*terrain*/) {
		}
		void on_enter(point2 const& grid_cell, stream_type & terrain) {
			terrain.load([&](patch_type & chunk) {
				chunk.assign_sprites(sprites);
				chunk.assign_cell(grid_cell);
				chunk.read(depot_name(grid_cell));
			});
		}

	private:
		qtree<transform_component*>					space;
		std::vector<uq_ptr<composite_component>>	units;
		surface_type								surface;
		sprite_system *								sprites;

		// nested class
		class terrain_release_block final
			: public abstract_release_block
		{
		public:
			terrain_release_block(join_release_block * raw, scene_internal * current, point2 grid_cell)
				: original(raw)
				, scene(current)
				, grid(grid_cell)
			{
			}

		protected:
			virtual void release_block() noexcept override {
				scene->on_leave(grid, *original->get_value());
				delete original;
			}

		private:
			join_release_block *	original;
			scene_internal *		scene;
			point2					grid;
		};

		// nested class
		class join_release_block final {
		public:
			stream_type * get_value() noexcept {
				return &stream;
			}
			abstract_release_block * get_control() noexcept {
				return &ctrl;
			}

		public:
			join_release_block(scene_internal * scene, point2 grid)
				: ctrl(this, scene, grid)
			{
			}
		private:
			stream_type				stream;
			terrain_release_block	ctrl;
		};
	};
}