#pragma once

#include <px/common/point.hpp>
#include <px/memory/uq_ptr.hpp>

namespace px {

	class terrain_internal;

	class terrain
	{
	public:
		bool is_transparent(point2 const& location);
		bool is_traversable(point2 const& location);

	private:
		uq_ptr<terrain_internal> impl;
	};
}