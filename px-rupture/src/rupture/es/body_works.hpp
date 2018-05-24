// name: body_works.hpp
// type: c++
// auth: is0urce
// desc: class internal implementation

#pragma once

#include "animator_component.hpp"
#include "body_component.hpp"
#include "composite_component.hpp"
#include "character_component.hpp"
#include "transform_component.hpp"

#include "../environment.hpp"

#include <px/common/pool_chain.hpp>

#include <algorithm>

namespace px {

    class body_works final {
    public:

        void assign_environment(environment * env) noexcept {
            game = env;
        }

        uq_ptr<body_component> make() {
            return pool.make_uq();
        }

        void tick() {
            pool.enumerate([&](body_component & body) {
                process_effects(body);
                process_death(body);
            });
        }

    public:
        body_works()
            : game(nullptr) {
        }

    private:
        void process_effects(body_component & body) {
            // damage-over-time
            auto dot = body.accumulate({ rl::effect::damage_periodic });
            if (dot.magnitude0 > 0) {
                auto damage = static_cast<body_component::resource_value_type>(dot.magnitude0);
                game->damage(body, damage, static_cast<rl::damage_type>(dot.sub));
            }

            // resource regeneration
            auto mp_regen = body.accumulate({ rl::effect::mp_regen });
            if (mp_regen.magnitude0 > 0) {
                if (body.energy()) {
                    auto mod = static_cast<body_component::resource_value_type>(mp_regen.magnitude0);
                    body.energy()->restore(mod);
                }
            }
            auto hp_regen = body.accumulate({ rl::effect::hp_regen });
            if (hp_regen.magnitude0 > 0) {
                if (body.health()) {
                    auto mod = static_cast<body_component::resource_value_type>(hp_regen.magnitude0);
                    body.health()->restore(mod);
                }
            }

            // degrade buffs
            for (auto & buff : body.buffs) {
                buff.reduce_duration(1);
            }

            // erase expired 
            body.buffs.erase(std::remove_if(body.buffs.begin(), body.buffs.end(), [](auto & buff) { return buff.is_expired(); }), body.buffs.end());
        }

        // death of unit
        void process_death(body_component & body) {
            if (composite_component * unit = body.linked<composite_component>()) {
                switch (unit->lifetime()) {
                case persistency::permanent:    // skip for permanent entities (player and controllers)
                    if (body.is_dead()) {
                        if (auto animations = unit->query<animator_component>()) {
                            animations->play("death");
                        }
                        auto person = unit->query<character_component>();
                        auto pawn = body.linked<transform_component>();
                        if (pawn && person && person->has_trait("t_red_blood")) {
                            game->spawn("d_blood_red", pawn->position());
                        }
                    }
                    break;
                case persistency::destroying:
                    unit->decay(1);
                    break;
                default:
                    if (body.is_dead()) {
                        // give experience
                        game->give_experience(body.experience(), body.level());
                        body.set_experience(0);

                        // set destroying persistency status
                        unit->destroy(0);
                    }
                    break;
                }
            }
        }

    private:
        pool_chain<body_component, 1024>    pool;
        environment *                       game;
    };
}