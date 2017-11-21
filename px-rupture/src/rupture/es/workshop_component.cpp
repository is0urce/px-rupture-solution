// name: workshop_component.cpp

#include "workshop_component.hpp"

#include "rupture/environment.hpp"

namespace px {

	workshop_component::~workshop_component() = default;
	workshop_component::workshop_component()
		: activity_variant(rl::craft_activity::none) {
	}

	// overrides

	bool workshop_component::can_use_useable(body_component * /*body*/, environment * /*context*/) const {
		return true;
	}
	void workshop_component::use_useable(body_component * /*body*/, environment * context) {
		if (context) {
			context->open_workshop(activity_variant);
		}
	}

	// methods

	rl::craft_activity workshop_component::activity() const noexcept {
		return activity_variant;
	}
	void workshop_component::set_activity(rl::craft_activity variant) {
		activity_variant = variant;
	}
}