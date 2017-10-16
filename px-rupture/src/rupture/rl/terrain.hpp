// name: terrain.hpp

#pragma once

#include <px/common/point.hpp>
#include <px/memory/uq_ptr.hpp>

#include <px/rl/traverse_options.hpp>
#include <px/rl/traverse.hpp>

#include <cstdint> // std::unint32_t
#include <tuple>

namespace px {

	class terrain_internal;
	class sprite_system;
	class transform_component;

	class terrain final
	{
	public:
		void assign_sprites(sprite_system * system);
		bool is_transparent(point2 const& location) const;
		bool is_traversable(point2 const& location, rl::traverse_options<rl::traverse> const& opts) const;
		void pset(std::uint32_t block_id, point2 const& location);
		//void focus(point2 const& location);

	public:
		~terrain();
		terrain();

	private:
		uq_ptr<terrain_internal> impl;
	};
}