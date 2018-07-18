// name: deposite_component.cpp
// type: c++
// auth: is0urce
// type: class implementation

#include "deposite_component.hpp"

#include "../app/settings.hpp"

#include "../environment.hpp"
#include "body_component.hpp"
#include "container_component.hpp"
#include "composite_component.hpp"
#include "light_component.hpp"
#include "transform_component.hpp"

namespace px {

    // ctor & dtor

    deposite_component::~deposite_component() = default;

    deposite_component::deposite_component()
        : dissolve(true)
        , use_duration(1) {
    }

    // virtual

    bool deposite_component::can_use_useable(body_component * user, environment * environment) const {
        if (!environment || !user) return false;
        if (!user->linked<container_component>() || !this->linked<container_component>()) return false;
        auto[unit, location] = this->unwind<composite_component, transform_component>();
        auto pawn = user->linked<transform_component>();
        return unit && pawn && environment->distance(pawn->position(), location->position()) <= 1;
    }

    void deposite_component::use_useable(body_component * user, environment * context) {
        if (!context) return;

        if (user) {
            auto dest = user->linked<container_component>();
            auto source = linked<container_component>();
            if (source && dest) {
                auto obj_unit = linked<composite_component>();
                auto obj_body = linked<body_component>();

                // transfer items with popups
                if (source->is_empty()) {
                    auto obj_pawn = obj_body ? obj_body->linked<transform_component>() : nullptr;
                    if (obj_pawn) {
                        context->popup("depleted", 0xffffff, obj_pawn->position());
                    }
                }
                else {
                    auto user_pawn = user->linked<transform_component>();
                    dest->give(*source, [&](auto const& item) {
                        if (user_pawn) {
                            context->popup("+ " + item.name(), 0xffffff, user_pawn->position());
                        }
                    });
                }

                // turn light off
                auto obj_light = obj_unit ? obj_unit->query<light_component>() : nullptr;
                if (obj_light) {
                    obj_light->is_on = false;
                }

                // start composite unit destruction
                if (obj_unit && dissolve) {
                    obj_unit->destroy(0);
                }

                if (use_duration > 0) {
                    context->end_turn(use_duration);
                }
            }
        }
    }

    // methods

    void deposite_component::set_dissolve(bool should_destroy) noexcept {
        dissolve = should_destroy;
    }

    void deposite_component::set_use_duration(time_type timer) noexcept {
        use_duration = timer;
    }

    bool deposite_component::is_dissolving() const noexcept {
        return dissolve;
    }

    deposite_component::time_type deposite_component::get_use_duration() const noexcept {
        return use_duration;
    }
}