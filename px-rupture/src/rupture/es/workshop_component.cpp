// name: workshop_component.cpp
// type: c++
// auth: is0urce
// desc: class implementation

#include "workshop_component.hpp"

#include "../environment.hpp"

namespace px {

    // constructor & desctuctor

    workshop_component::~workshop_component() = default;

    workshop_component::workshop_component()
        : activity_variant(rl::craft_activity::none) {
    }

    // overrides

    bool workshop_component::can_use_useable(body_component * /*user*/, environment * /*context*/) const {
        return true;
    }

    void workshop_component::use_useable(body_component * /*user*/, environment * context) {
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