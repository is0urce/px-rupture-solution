// name: sprite_system.hpp

#pragma once

#include <px/es/system.hpp>
#include <px/es/delta.hpp>

#include "sprite_system.hpp"

namespace px {
	
	class sprite_sytem
		: public system<delta>
	{

	protected:
		virtual void update_system(delta_type const& /*delta_time*/)
		{
		}
	};
}