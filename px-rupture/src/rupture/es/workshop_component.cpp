// name: workshop_component.cpp

#include "workshop_component.hpp"

#include "rupture/environment.hpp"

namespace px {

	workshop_component::~workshop_component() = default;
	workshop_component::workshop_component()
		: station(rl::workshop::none) {
	}

	// overrides

	bool workshop_component::can_use_useable(body_component * /*body*/, environment * /*context*/) const {
		return true;
	}
	void workshop_component::use_useable(body_component * /*body*/, environment * context) {
		if (context) {
			context->access_workshop(station);
		}
	}

	// methods

	rl::workshop workshop_component::variant() const noexcept {
		return station;
	}
	void workshop_component::set_variant(rl::workshop variant) {
		station = variant;
	}
}