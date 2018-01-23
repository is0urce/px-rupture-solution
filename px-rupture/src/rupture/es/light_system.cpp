// name: light_system.cpp

#include "light_system.hpp"
#include "light_works.hpp"

#include <px/memory/memory.hpp>

namespace px {

	// ctors

	light_system::~light_system() = default;
	light_system::light_system()
		: works(make_uq<light_works>(20))
	{
	}

	// virtual overload

	void light_system::turn_update_system(delta_type const& /*delta_time*/) {
		works->calculate_lights();
	}

	// methrods

	void light_system::assign_scene(scene const* stage) noexcept {
		works->assign_scene(stage);
	}

	void light_system::recalculate() {
		works->clear_lightmap();
		works->calculate_lights();
	}

	uq_ptr<light_component> light_system::make() {
		return works->make();
	}

	void light_system::clear_lightmap() {
		works->clear_lightmap();
	}

	void light_system::target(transform_component const* camera) noexcept {
		works->target(camera);
	}

	lightmap_data const* light_system::current_data() noexcept {
		return works->current();
	}

	lightmap_data const* light_system::last_data() noexcept	{
		return works->last();
	}

	void light_system::set_fov_cropping(bool do_cropping) noexcept {
		works->set_fov_cropping(do_cropping);
	}
}