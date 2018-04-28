// name: script_unit.hpp
// type: c++

#pragma once

#include "../es/body_component.hpp"
#include "../es/transform_component.hpp"

#include "script_effect.hpp"

namespace px {

    class script_unit final {
    public:
        void apply_effect(script_effect & effect) {
            if (body) {
                body->add(effect.buff);
            }
        }

        bool is_alive() const {
            return body && body->is_alive();
        }

        bool is_valid() const noexcept {
            return body != nullptr;
        }

        bool is_enemy(script_unit const& subject) const {
            return body && subject.body && body->is_hostile(*subject.body);
        }

        void place(point2 target) {
            if (auto && pawn = get_transform()) {
                pawn->place(target);
            }
        }

        float mp() const {
            float result = 0;
            if (body) {
                if (auto && mp = body->energy()) {
                    result = static_cast<float>(mp->current());
                }
            }
            return result;
        }

        float damage(float amount) {
            float damage_done = 0;
            if (body) {
                if (auto && hp = body->health()) {
                    damage_done = static_cast<float>(hp->damage(static_cast<body_component::resource_value_type>(amount)));
                }
            }
            return damage_done;
        }

        float deplete(float amount) {
            float drain = 0;
            if (body) {
                if (auto && mp = body->energy()) {
                    drain = static_cast<float>(mp->damage(static_cast<body_component::resource_value_type>(amount)));
                }
            }
            return drain;
        }

        float heal(float magnitude) {
            float healing_done = 0;
            if (body) {
                if (auto && hp = body->health()) {
                    healing_done = static_cast<float>(hp->restore(static_cast<body_component::resource_value_type>(magnitude)));
                }
            }
            return healing_done;
        }

        point2 position() {
            auto pawn = get_transform();
            return pawn ? pawn->position() : point2(0, 0);
        }

        body_component * get_body() {
            return body;
        }

        body_component const* get_body() const {
            return body;
        }

        transform_component * get_transform() {
            return body ? body->linked<transform_component>() : nullptr;
        }

    public:
        script_unit()
            : body(nullptr)
            , transform(nullptr) {
        }

        script_unit(body_component * user)
            : body(user)
            , transform(nullptr) {
        }

    private:
        body_component *        body;
        transform_component *   transform;
    };

}