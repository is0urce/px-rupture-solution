// name: door_component.cpp

#include "door_component.hpp"

#include "body_component.hpp"
#include "animator_component.hpp"
#include "../environment.hpp"

namespace px {

    door_component::~door_component() noexcept = default;

    door_component::door_component()
        : opened(false)
        , open_duration(1) {
    }

    bool door_component::open() {
        if (body_component * body = linked<body_component>()) {
            body->blocking().make_empty();
            if (animator_component * animator = linked<animator_component>()) {
                if (animator->size() != 0) {
                    animator->play(0);
                }
            }
            opened = true;
            return true;
        }
        return false;
    }

    bool door_component::close() {
        if (body_component * body = linked<body_component>()) {
            body->blocking().make_solid();
            opened = false;
            return true;
        }
        return false;
    }

    bool door_component::set_open(bool flag) {
        return opened = flag;
    }

    unsigned char door_component::use_duration() const noexcept {
        return open_duration;
    }

    bool door_component::is_opened() const noexcept {
        return opened;
    }

    bool door_component::can_use_useable(body_component *, environment *) const {
        return !opened;
    }

    void door_component::use_useable(body_component *, environment * game) {
        if (open() && open_duration != 0) {
            game->end_turn(open_duration);
        }
    }
}