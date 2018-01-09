// name: body_system.cpp

#include "body_system.hpp"

#include "body_works.hpp"

#include <px/memory/memory.hpp>

namespace px {

	// ctor & dtor

	body_system::~body_system() {
	}

	body_system::body_system()
		: works(make_uq<body_works>())
	{
	}

	// virtual overrides

	void body_system::turn_update_system(delta_type const& /*delta_time*/) {
		works->tick();
	}

	// methods

	void body_system::assign_environment(environment * game) {
		works->assign_environment(game);
	}

	uq_ptr<body_component> body_system::make() {
		return works->make();
	}
}