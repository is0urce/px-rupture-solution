// name: terrain.hpp

#pragma once

#include <px/common/point.hpp>
#include <px/memory/uq_ptr.hpp>

#include <px/rl/traverse_options.hpp>
#include <px/rl/traverse.hpp>

namespace px {

	class terrain_internal;

	class terrain
	{
	public:
		bool is_transparent(point2 const& location) const;
		bool is_traversable(point2 const& location, rl::traverse_options<rl::traverse> const& opts) const;

	public:
		~terrain();
		terrain();

	private:
		uq_ptr<terrain_internal> impl;
	};
}