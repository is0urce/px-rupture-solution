// name: terrain_internal.hpp

#pragma once

#include "tile.hpp"

#include "rupture/es/sprite_system.hpp"

#include <px/common/matrix.hpp>
#include <px/common/point.hpp>

#include <px/rl/mass.hpp>
#include <px/rl/traverse.hpp>

namespace px {
	
	class terrain_internal final
	{
	public:
		void assign_sprites(sprite_system * system)
		{
			sprites = system;
		}
		bool is_transparent(point2 const& location) const
		{
			return tiles[location].mass.is_transparent();
		}
		bool is_traversable(point2 const& location, rl::traverse_options<rl::traverse> const& opt) const
		{
			return tiles[location].mass.is_traversable(opt);
		}

	public:
		terrain_internal()
			: sprites(nullptr)
		{
			//tiles.generate([&] { tile t; return std::move(t); }, 10, 10);
			//tiles.resize_default(10, 10);
			tiles.resize(10, 10);
		}

	private:
		matrix2<tile> tiles;
		sprite_system * sprites;
	};
}