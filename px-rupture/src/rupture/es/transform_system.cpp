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

	uq_ptr<transform_component> transform_system::make()
	{
		return works->make();
	}
}