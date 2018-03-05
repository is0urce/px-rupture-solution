// name: script_environment.hpp
// type: c++
// auth: is0urce
// desc: script bindings for game object unit

#pragma once

#include "script_unit.hpp"

#include "rupture/environment.hpp"

#include "rupture/es/composite_component.hpp"

#include <string>
#include <tuple>

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

        // simulate melee hit event ang get it results
        std::tuple<int, int, bool, bool> hit(script_unit const& attacker, script_unit const& target) {
            body_component const* user_body = attacker.get_body();
            body_component const* target_body = target.get_body();

            int damage = 0;
            bool critical = false;
            bool connects = false;
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

        void damage(script_unit & target, int damage, int damage_type) {
            if (auto body = target.get_body()) {
                game->damage(*body, damage, static_cast<rl::damage_type>(damage_type));
            }
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

    public:
        script_environment() noexcept
            : game(nullptr)
        {
        }
        script_environment(environment * environment) noexcept
            : game(environment)
        {
        }

    private:
        environment * game;
    };
}