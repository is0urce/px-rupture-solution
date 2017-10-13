// name: tile.hpp

#pragma once

#include "rupture/es/transform_component.hpp"

#include <px/common/point.hpp>
#include <px/memory/uq_ptr.hpp>
#include <px/rl/mass.hpp>
#include <px/rl/traverse.hpp>

#include <cstdint>

namespace px {

	class sprite_component;

	class tile
	{
	public:
		uint32_t block_id;
		transform_component transform;
		uq_ptr<sprite_component> sprite;
		rl::mass<rl::traverse> mass;

	public:
		tile() noexcept = default;
		tile(tile const&) noexcept = delete;
		tile(tile && that) noexcept
			: sprite(std::move(that.sprite))
			, transform(std::move(that.transform))
			, mass(std::move(that.mass))
			, block_id(std::move(that.block_id))
		{
		}
	};
}