// name: npc_works.hpp
// type: c++
// auth: is0urce
// desc: npc ai system internal class

#pragma once

#include "body_component.hpp"
#include "character_component.hpp"
#include "npc_component.hpp"
#include "transform_component.hpp"

#include "../rl/scene.hpp"

#include <px/common/pool_chain.hpp>
#include <px/common/qtree.hpp>
#include <px/algorithm/fov.hpp>
#include <px/algorithm/a_star.hpp>

namespace px {

    class npc_works {
    public:
        void assign_scene(scene * world) noexcept {
            stage = world;
        }
        uq_ptr<npc_component> make() {
            return pool.make_uq();
        }

    public:
        void turn() {
            fov sight;
            pool.enumerate([&](npc_component & npc) {
                if (!npc.is_active()) return;

                // ensure valid composition
                auto pawn = npc.linked<transform_component>();
                if (!pawn) return;
                auto body = pawn->linked<body_component>();
                if (!body) return;
                auto character = body->linked<character_component>();
                if (!character) return;

                // ensure npc can act
                if (!body->is_alive()) return;

                // health drop check
                if (npc.is_idle() && !body->health()->is_full()) {
                    npc.alert();
                    npc.destination() = pawn->position(); // init waypoint to current position
                }

                // select enemy in range
                unsigned int range = npc.get_range();
                sight.calculate(pawn->position(), range, [&](int x, int y) { return stage->is_transparent({ x, y }); });
                transform_component * target = lock_target(pawn->position(), *body, sight);
                if (target) {
                    npc.alert();
                    npc.destination() = target->position();
                }

                // act
                if (!npc.is_idle()) {
                    bool cast = false;

                    // use direct skills
                    if (target) {
                        auto * target_body = target->linked<body_component>();
                        for (size_t i = 0, size = character->size(); !cast && i != size; ++i) {
                            if (auto * skill = character->get(i)) {
                                cast = skill->is_targeted() && skill->try_use(body, target_body);
                            }
                        }
                    }

                    // use area skills
                    if (!cast) {
                        for (size_t i = 0, size = character->size(); !cast && i != size; ++i) {
                            if (auto * skill = character->get(i)) {
                                cast = !skill->is_targeted() && skill->try_use(body, npc.destination());
                            }
                        }
                    }

                    // or advance
                    if (!cast) {
                        const unsigned int iteration_steps = 150;
                        auto path = a_star::find(pawn->position(), npc.destination(), [opts = body->movement(), this](point2 const& location) { return stage->is_traversable(location, opts); }, iteration_steps);

                        if (path.size() != 0) {
                            point2 const& next = path.front();
                            if (stage->is_traversable(next, body->movement())) {
                                pawn->place(next);
                            }
                        }
                    }
                }
            });
        }

    private:

        // select target
        transform_component * lock_target(point2 const& center, body_component::standing_type const& faction, fov const& observe) {
            transform_component * result = nullptr;
            stage->space()->find(center, observe.range(), [&](int /*x*/, int /*y*/, transform_component * subject) {
                if (!subject) return;
                if (!observe.contains(subject->position())) return;

                if (auto subject_body = subject->linked<body_component>()) {
                    if (faction.is_hostile(*subject_body)) {
                        result = subject;
                    }
                }
            });
            return result;
        }

    private:
        pool_chain<npc_component, 1024> pool;
        scene *                         stage;
    };
}