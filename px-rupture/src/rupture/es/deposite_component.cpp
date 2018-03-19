// name: deposite_component.cpp
// type: c++
// auth: is0urce
// type: class implementation

#include "deposite_component.hpp"

#include "../environment.hpp"
#include "body_component.hpp"
#include "container_component.hpp"
#include "composite_component.hpp"
#include "transform_component.hpp"

namespace px {

    // ctor & dtor

    deposite_component::~deposite_component() = default;

    deposite_component::deposite_component()
        : dissolve(true)
        , use_duration(1)
    {
    }

    // virtual

    bool deposite_component::can_use_useable(body_component * user, environment * environment) const {
        if (!environment || !user) return false;
        if (!user->linked<container_component>() || !this->linked<container_component>()) return false;
        auto [unit, location] = this->unwind<composite_component, transform_component>();
        auto pawn = user->linked<transform_component>();
        return unit && pawn && environment->distance(pawn->position(), location->position()) <= 1;
    }

    void deposite_component::use_useable(body_component * user, environment * environment) {
        if (user && environment) {
            auto dest = user->linked<container_component>();
            auto source = linked<container_component>();
            if (source && dest) {

                // transfer items with popups
                auto pawn = user->linked<transform_component>();
                dest->give(*source, [&](auto const& item) {
                    if (pawn) {
                        environment->popup("+ " + item.name(), 0xffffff, pawn->position());
                    }
                });

                // destroy deposit
                if (dissolve) {
                    if (auto unit = linked<composite_component>()) {
                        unit->destroy(0);
                    }
                }

                if (use_duration > 0) {
                    environment->end_turn(use_duration);
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