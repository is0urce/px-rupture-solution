// name: npc_system.cpp

#include "npc_system.hpp"

#include "npc_works.hpp"

#include <px/memory/memory.hpp>

namespace px {

	// ctor & dtor

	npc_system::~npc_system()
	{

	}
	npc_system::npc_system()
		: works(make_uq<npc_works>())
	{

	}

	// virtual overrides

	void npc_system::turn_update_system(delta_type const& /*delta_time*/)
	{

	}
}