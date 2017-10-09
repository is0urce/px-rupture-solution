// name: terrain_internal.hpp

#pragma once

#include "tile.hpp"
#include "tile_prototype.hpp"

#include "rupture/app/settings.hpp"
#include "rupture/app/document.hpp"
#include "rupture/es/sprite_system.hpp"

#include "rupture/es/sprite_component.hpp"

#include <px/common/matrix.hpp>
#include <px/common/point.hpp>

#include <px/rl/mass.hpp>
#include <px/rl/traverse.hpp>

#include <map>

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
		void pset(uint32_t block_id, point2 const& location)
		{
			tile & target = tiles.at(location);
			tile_prototype const& prototype = lib[block_id];

			target.block_id = block_id;
			target.mass = prototype.mass;
			target.sprite = sprites->make(prototype.name);
			target.sprite->connect(&target.transform);
			target.sprite->activate();
		}

	public:
		terrain_internal()
			: sprites(nullptr)
		{
			tiles.resize(10, 10);
			tiles.enumerate([&](size_t x, size_t y, tile & t) {
				t.block_id = 0;
				t.transform.place({ static_cast<int>(x), static_cast<int>(y) });
			});

			auto doc = document::load_document(settings::tiles_path);
			auto tile_list = doc["tiles"];
			for (auto const& tile_node : tile_list) {
				unsigned int block_id = tile_node["block_id"];
				std::string name = tile_node["name"];
				bool transparent = tile_node["transparent"];
				unsigned int traverse_mask = tile_node["traversable"];

				rl::mass<rl::traverse>::bitset_type blocking_bits{ traverse_mask };

				add_prototype(block_id, name, { transparent, ~blocking_bits });
			}
		}

	private:
		void add_prototype(uint32_t block_id, std::string const& name, rl::mass<rl::traverse> const& mass)
		{
			lib[block_id] = { block_id, name, mass };
		}

	private:
		matrix2<tile>						tiles;
		sprite_system *						sprites;
		std::map<uint32_t, tile_prototype>	lib;
	};
}