// name: tile_prototype.hpp

#pragma once

#include <px/rl/mass.hpp>
#include <px/rl/traverse.hpp>

#include <cstdint>
#include <string>

namespace px {

	struct tile_prototype
	{
		std::uint32_t block_id;
		std::string name;
		rl::mass<rl::traverse> mass;
	};
}