// name: transform_system.hpp

#include "transform_system.hpp"
#include "transform_works.hpp"

#include <px/memory/memory.hpp>

namespace px {

	transform_system::~transform_system()
	{
	}
	transform_system::transform_system()
		: works(make_uq<transform_works>())
	{
	}

	// virtual

	void transform_system::turn_update_system(delta const& /*timers*/)
	{
		works->store();
	}

	// methods

	uq_ptr<transform_component> transform_system::make()
	{
		return works->make();
	}
}