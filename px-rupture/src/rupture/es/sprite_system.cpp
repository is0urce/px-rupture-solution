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
		load();
	}

	// override

	void sprite_system::update_system(delta_type const& delta_time)
	{
		works->batch(delta_time.real_delta);
	}

	// methods

	uq_ptr<sprite_component> sprite_system::make(std::string const& name)
	{
		return works->make(name);
	}

	std::vector<sprite_vertex> const* sprite_system::data() const noexcept
	{
		return works->data();
	}

	void sprite_system::target(transform_component const* camera) noexcept
	{
		works->target(camera);
	}

	void sprite_system::load()
	{
		works->add_image("rat", 0, 0, 1, 1, 0);
	}
}