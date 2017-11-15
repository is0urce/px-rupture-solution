// name: terrain_surface.hpp

#pragma once

#include "terrain_patch.hpp"

#include <px/common/matrix.hpp>
#include <px/common/point.hpp>
#include <px/common/stream.hpp>

namespace px {

	template <size_t L, size_t H>
	class terrain_surface {
	private:
		static const size_t range = H * 2 + 1;
	//public:
	//	void grid(unsigned int grid_size) {
	//		grid_length = grid_size;
	//	}
	//	void resize(unsigned int range) {
	//		cell_range = range;
	//		terrain.resize(range * 2 + 1, range * 2 + 1);
	//	}

	//public:
	//	terrain_surface() = default;

	private:
		matrix2<stream<terrain_patch<L, L>>, range, range> terrain;
	};
}