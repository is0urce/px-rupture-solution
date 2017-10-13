// name: scene.hpp

#pragma once

#include <px/common/point.hpp>
#include <px/memory/uq_ptr.hpp>

#include <px/rl/traverse_options.hpp>
#include <px/rl/traverse.hpp>

#include <cstdint> // uint32_t block_id

namespace px {

	class scene_internal;

	class composite_component;
	class sprite_system;

	class scene final
	{
	public:
		bool is_transparent(point2 const& location) const;
		bool is_traversable(point2 const& location, rl::traverse_options<rl::traverse> const& opts) const;

		void add(uq_ptr<composite_component> && ptr);

		void assign_sprites(sprite_system * system);
		void pset(uint32_t block_id, point2 const& location);

	public:
		~scene();
		scene();

	private:
		uq_ptr<scene_internal> works;
	};
}