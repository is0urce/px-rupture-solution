#include "character_system.hpp"
#include "character_works.hpp"

#include <px/memory/memory.hpp>

namespace px {

	character_system::~character_system()
	{
	}
	character_system::character_system()
		: works(make_uq<character_works>())
	{
	}

	uq_ptr<character_component> character_system::make()
	{
		return works->make();
	}

	void character_system::turn_update_system(delta_type const& /*delta_time*/)
	{
		works->turn();
	}
}