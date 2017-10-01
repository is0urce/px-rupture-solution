// name: sprite_system.hpp

#include "sprite_system.hpp"
#include "sprite_works.hpp"

#include <px/memory/memory.hpp>

namespace px {

	sprite_system::~sprite_system()
	{
	}

	sprite_system::sprite_system()
		: works(make_uq<sprite_works>())
	{
	}

	// override

	void sprite_system::update_system(delta_type const& delta_time)
	{
		works->batch(delta_time.real_delta);
	}

	// methods

	uq_ptr<sprite_component> sprite_system::make()
	{
		return works->make();
	}

	std::vector<vertex> const* sprite_system::data() const noexcept
	{
		return works->data();
	}
}