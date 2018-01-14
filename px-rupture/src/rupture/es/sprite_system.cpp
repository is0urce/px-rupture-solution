// name: sprite_system.hpp

#include "sprite_system.hpp"
#include "sprite_works.hpp"

#include <px/memory/memory.hpp>

namespace px {

	sprite_system::~sprite_system() = default;

	sprite_system::sprite_system()
		: works(make_uq<sprite_works>())
	{
	}

	// override

	void sprite_system::update_system(delta_type const& delta_time)	{
		works->batch(delta_time.turn_duration);
	}

	// methods

	uq_ptr<sprite_component> sprite_system::make(std::string const& name) {
		return works->make(name);
	}
	sprite const* sprite_system::frame(std::string const& name) const {
		return works->frame(name);
	}

	std::vector<std::vector<sprite_vertex>> const* sprite_system::data() const noexcept {
		return works->data();
	}

	void sprite_system::target(transform_component const* camera) noexcept {
		works->target(camera);
	}

	void sprite_system::add_sprite(std::string const& name, float sx, float sy, float dx, float dy, unsigned int texture_id) {
		works->add_sprite(name, sx, sy, dx, dy, texture_id);
	}
}