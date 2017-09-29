// name: sprite_system.hpp

#pragma once

#include <px/es/system.hpp>
#include <px/es/delta.hpp>

#include "sprite_system.hpp"

namespace px {
	
	class sprite_system final
		: public system<delta>
	{
	public:
		virtual ~sprite_system()
		{
		}

	protected:
		virtual void update_system(delta_type const& /*delta_time*/)
		{
		}
	};
}