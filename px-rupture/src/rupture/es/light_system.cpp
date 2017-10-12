// name: light_system.cpp

#include "light_system.hpp"
#include "light_works.hpp"

#include <px/memory/memory.hpp>

namespace px {

	light_system::~light_system()
	{

	}
	light_system::light_system()
		: works(make_uq<light_works>())
	{
	}


	void light_system::update_system(delta_type const& /*delta_time*/)
	{
		works->calculate_lights();
	}

	uq_ptr<light_component> light_system::make()
	{
		return works->make();
	}
	void light_system::target(transform_component const* camera) noexcept
	{
		works->target(camera);
	}
}