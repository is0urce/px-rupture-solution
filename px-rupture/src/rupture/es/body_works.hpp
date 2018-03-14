// name: body_works.hpp
// type: c++
// auth: is0urce
// desc: class internal implementation

#pragma once

#include "body_component.hpp"
#include "composite_component.hpp"

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

                // applicate damage-over-time
                auto dot = body.accumulate(body_component::enhancement_type::zero(rl::effect::dot));
                if (dot.magnitude0 > 0) {
                    auto damage = static_cast<int>(dot.magnitude0);
                    game->damage(body, damage, static_cast<rl::damage_type>(dot.sub));
                }

                // reduce effect durations
                for (auto & buff : body.buffs) {
                    buff.reduce_duration(1);
                }
                body.buffs.erase(std::remove_if(body.buffs.begin(), body.buffs.end(), [](auto & buff) { return buff.is_expired(); }), body.buffs.end());

                // death of units
                if (composite_component * unit = body.linked<composite_component>()) {
                    switch (unit->lifetime()) {
                    case persistency::permanent:    // skip for permanent entities
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
            });
        }

    public:
        body_works()
            : game(nullptr)
        {
        }

    private:
        pool_chain<body_component, 1024>    pool;
        environment *                       game;
    };
}