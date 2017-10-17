// name: animator_system.hpp

#include "animator_system.hpp"
#include "animator_works.hpp"

#include <px/memory/memory.hpp>

namespace px {


	animator_system::~animator_system()
	{

	}
	animator_system::animator_system()
		: works(make_uq<animator_works>())
	{

	}

	uq_ptr<animator_component> animator_system::make(std::string const& name)
	{
		return works->make(name);
	}

	void animator_system::update_system(delta_type const& delta_time)
	{
		works->update(delta_time.turn_duration);
	}
	void animator_system::turn_update_system(delta_type const& /*delta_time*/)
	{
		works->finish_animations();
	}
}