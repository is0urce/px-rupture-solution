// name: script_environment.hpp
// type: c++
// auth: is0urce
// desc: script bindings for game object unit

#pragma once

#include "script_unit.hpp"
#include "script_effect.hpp"

#include "../app/settings.hpp"
#include "../environment.hpp"

#include "../es/composite_component.hpp"

#include <string>
#include <tuple>
#include <vector>

namespace px {

    class script_environment {
    public:

        // distance in game units
        int distance(point2 const& a, point2 const& b) {
            return game->distance(a, b);
        }

        // emit notification text
        void popup(std::string text, int rgba, point2 location) {
            game->popup(text, color(rgba), location);
        }

        // emit visual effect
        void vfx(std::string const& name, point2 start, point2 finish, script_unit & track) {
            game->emit_visual(name, start, finish, track.get_transform());
        }

        // emit animated clip
        void clip(std::string const& name, unsigned int clip_id, point2 start, point2 finish, script_unit & track) {
            game->emit_animation(name, clip_id, start, finish, track.get_transform());
        }

        // emit light effect
        void light(point2 location, float r, float g, float b) {
            game->emit_light(location, color(r, g, b));
        }

        // emit visual effect
        void sfx(std::string const& name, float volume, point2 start, point2 /* finish */, script_unit & /* track */) {
            game->play_sound(settings::sound_path + name, volume, start);
        }

        // simulate melee hit event ang get it results
        // return: damage, type, connects, critical
        std::tuple<float, int, bool, bool> hit(script_unit const& attacker, script_unit const& target) {
            body_component const* user_body = attacker.get_body();
            body_component const* target_body = target.get_body();

            bool connects = false;
            bool critical = false;

            float damage = 0;
            rl::damage_type damage_type = rl::damage_type::pure;

            if (user_body && target_body) {
                rl::hit_result hit = game->hit(*user_body, *target_body);
                std::tie(damage, damage_type) = game->dps(*user_body);
                switch (hit) {
                case rl::hit_result::connects:
                case rl::hit_result::critical:
                    connects = true;
                    break;
                case rl::hit_result::failure:
                case rl::hit_result::miss:
                default:
                    break;
                }
            }

            return { damage, static_cast<int>(damage_type), connects, critical };
        }

        script_unit spawn(std::string const& blueprint_tag, point2 const& location) {
            auto & composite = game->spawn(blueprint_tag, location);
            return script_unit(composite ? composite->qlink<body_component, transform_component>() : nullptr);
        }

        float damage(script_unit & target, float damage, int damage_type) {
            float damage_done = 0;
            if (auto body = target.get_body()) {
                damage_done = static_cast<float>(game->damage(*body, static_cast<int>(damage), static_cast<rl::damage_type>(damage_type)));
            }
            return damage_done;
        }

        bool in_sight(script_unit & user, point2 const& location) {
            if (auto body = user.get_body()) {
                return game->in_sight(*body, location);
            }
            return false;
        }

        bool in_line(script_unit & user, point2 const& location) {
            if (auto body = user.get_body()) {
                return game->in_line(*body, location);
            }
            return false;
        }

        script_effect create_effect(std::string const& name, unsigned int duration) {
            script_effect result;
            result.set_name(name);
            result.set_duration(duration);
            return result;
        }

        bool is_traversable(point2 const& location, script_unit const& unit) {
            bool result = false;
            if (auto body = unit.get_body()) {
                result = game->is_traversable(location, *body);
            }
            return result;
        }

        std::tuple<bool, point2> neighbour(point2 const& location, unsigned int direction) {
            return game->neighbour(location, direction);
        }

        script_unit get_first(point2 const& location, unsigned int radius, bool require_los) {
            targets.clear();
            if (game) {
                game->query_targets(location, radius, require_los, [&](point2 const& /*location*/, body_component * body) {
                    targets.push_back(body);
                });
            }
            return get_next();
        }

        script_unit get_next() {
            body_component * target = nullptr;
            if (!targets.empty()) {
                target = targets.back();
                targets.pop_back();
            }
            return script_unit(target);
        }

    public:
        script_environment() noexcept
            : game(nullptr) {
        }

        script_environment(environment * environment) noexcept
            : game(environment) {
        }

    private:
        std::vector<body_component*>    targets;
        environment *                   game;
    };
}