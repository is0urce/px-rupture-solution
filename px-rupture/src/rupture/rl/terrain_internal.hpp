#pragma once

#include <px/common/timer.hpp>

namespace px {
	
	class terrain_internal final
	{
	public:
		bool is_transparent(point2 const& location);
		bool is_traversable(point2 const& location);
	};
}