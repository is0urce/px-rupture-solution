// name: mechanic.hpp

#pragma once

#include <px/rl/hit_result.hpp>
#include <px/rl/damage_type.hpp>
#include <px/rl/effect.hpp>

#include <tuple>

namespace px {

    class mechanic {
    public:
        template <typename T, typename Rng>
        static rl::hit_result hit(T const& source, T const& target, bool diminish, Rng & rng) {
            rl::hit_result result = rl::hit_result::miss;

            double accuracy = source.accumulate({ rl::effect::accuracy }).magnitude0;
            double dodge = target.accumulate({ rl::effect::dodge }).magnitude0;

            auto score = accuracy - dodge;

            // level difference attentuation
            if (diminish) {
                auto spread = target.level() - source.level();
                score -= spread > 0 ? spread * 0.05 : 0;
            }

            if (std::uniform_real_distribution<double>{}(rng) < score) {
                result = rl::hit_result::connects;
                const auto critical = source.accumulate({ rl::effect::critical }).magnitude0;
                if (std::uniform_real_distribution<double>{}(rng) < critical) {
                    result = rl::hit_result::critical;
                }
            }
            else {
                if (std::uniform_real_distribution<double>{}(rng) < 0.05) {
                    result = rl::hit_result::failure;
                }
            }

            return result;
        }

        template <typename T>
        static std::tuple<float, rl::damage_type> dps(T const& source) {
            const auto efx = source.accumulate({ rl::effect::damage });
            return { static_cast<float>(efx.magnitude0), static_cast<rl::damage_type>(efx.sub) };
        }

        template <typename T>
        static int damage(T & target, int dmg, rl::damage_type /*source*/) {
            int damage_done = 0;
            if (auto & hp = target.health()) {
                damage_done = hp->damage(dmg);
            }
            return damage_done;
        }

        template <typename T>
        static int heal(T & target, int dmg, rl::damage_type /*source*/) {
            int done = 0;
            if (auto & hp = target.health()) {
                done = hp->restore(dmg);
            }
            return done;
        }

        template <typename T>
        static int innervate(T & target, int dmg, rl::damage_type /*source*/) {
            int done = 0;
            if (auto & resource = target.energy()) {
                done = resource->restore(dmg);
            }
            return done;
        }
    };
}