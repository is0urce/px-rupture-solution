// name: terrain_internal.hpp

#pragma once

#include "tile.hpp"
#include "tile_library.hpp"

#include <px/common/matrix.hpp>
#include <px/common/point.hpp>

#include <fstream>

namespace px {
	
	template <size_t W, size_t H>
	class terrain_patch {
	public:
		void assign_sprites(sprite_system * system) {
			library.assign_sprites(system);
		}
		void assign_cell(point2 cell) {
			grid_cell = cell;
			point2 start = grid_cell * point2(W, H);
			tiles.enumerate([&](size_t x, size_t y, tile & block) {
				block.block_id = 0;
				block.transform.place(start + point2(static_cast<int>(x), static_cast<int>(y)));
				block.transform.store();
			});
		}
		point2 grid() const {
			return grid_cell;
		}
		bool is_transparent(point2 const& location) const {
			return tiles.get_or(border, location).mass.is_transparent();
		}
		bool is_traversable(point2 const& location, rl::traverse_options<rl::traverse> const& opt) const {
			return tiles.get_or(border, location).mass.is_traversable(opt);
		}
		void pset(std::uint32_t block_id, point2 const& location) {
			library.setup(block_id, tiles.at(location));
		}
		bool write(std::string const& path) const {
			std::ofstream out_stream(path, std::ios_base::binary);
			if (!out_stream.is_open()) return false;

			tiles.enumerate([&](size_t /*x*/, size_t /*y*/, tile const& block) {
				out_stream.write(reinterpret_cast<char const*>(&block.block_id), sizeof block.block_id);
			});

			return true;
		}
		bool read(std::string const& path) {
			std::ifstream in_stream(path, std::ios_base::binary);
			if (!in_stream.is_open()) return false;

			tiles.enumerate([&](size_t /*x*/, size_t /*y*/, tile & block) {
				in_stream.read(reinterpret_cast<char*>(&block.block_id), sizeof block.block_id);
				library(block);
			});

			return true;
		}
		template <typename Operator>
		void enumerate(Operator && function) {
			tiles.enumerate(std::forward<Operator>(function));
		}
		template <typename Operator>
		void enumerate(Operator && function) const {
			tiles.enumerate(std::forward<Operator>(function));
		}
		void invalidate() {
			tiles.enumerate([&](size_t /*x*/, size_t /*y*/, tile & block) {
				library(block);
			});
		}

	public:
		terrain_patch()
		{
		}

	private:
		matrix2<tile, W, H>					tiles;
		tile								border;
		tile_library						library;
		point2								grid_cell;
	};
}