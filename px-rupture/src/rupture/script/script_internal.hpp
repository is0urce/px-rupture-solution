// name: script internal
// type: c++ class
// auth: is0urce
// desc: internal implementation of script system

#pragma once

#include "script_unit.hpp"
#include "script_environment.hpp"

#include <px/dev/assert.hpp>

#include <sol.hpp>

#include <map>
#include <string>

namespace px {

    class script_internal final {
    public:

        void assign_environment(environment * game) {
            lua["game"] = script_environment(game);
        }

        void run(std::string const& code) {
            lua.script(code);
        }

        void execute(std::string const& path) {
            lua.script_file(path);
        }

        rl::skill_functional<body_component *, body_component *, point2> impact(std::string const& path) {
            try {
                auto & sandbox = skills[path] = sol::environment(lua, sol::create, lua.globals());
                lua.script_file(path, sandbox);

                bool targeted = sandbox["targeted"].get_or(false);
                sol::function action = sandbox["action"];
                sol::function condition = sandbox["condition"];

                // functional
                auto target_action = [action](body_component * user, body_component * target) -> void {
                    try {
                        action(script_unit(user), script_unit(target));
                    }
                    catch (sol::error const& error) {
                        px::debug(error.what());
                    }
                };

                auto area_action = [action](body_component * user, point2 const& area) -> void {
                    try {
                        action(script_unit(user), area);
                    }
                    catch (sol::error const& error) {
                        px::debug(error.what());
                    }
                };

                auto target_condition = [condition](body_component * user, body_component * target) -> bool {
                    try {
                        return condition(script_unit(user), script_unit(target));
                    }
                    catch (sol::error const& error) {
                        px::debug(error.what());
                        return false;
                    }
                };
                auto area_condition = [condition](body_component * user, point2 const& area) -> bool {
                    try {
                        return condition(script_unit(user), area);
                    }
                    catch (sol::error const& error) {
                        px::debug(error.what());
                        return false;
                    }
                };

                // create record
                return { targeted, target_action, target_condition, area_action, area_condition };
            }
            catch (sol::error const& error) {
                px::debug(error.what());
                return {};
            }
            catch (...) {
                throw std::runtime_error("px::script::load_skill(path), path=" + path);
            }
        }

    public:
        script_internal() {
            lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
            lua.open_libraries(sol::lib::math);

            add_usertypes();
            add_functions();
        }

    private:
        void add_usertypes() {

            lua.new_usertype<point2>("point"
                , "x", &point2::x
                , "y", &point2::y
                );

            lua.new_usertype<script_effect>("effect"
                , "set_name", &script_effect::set_name
                , "set_duration", &script_effect::set_duration
                , "set_hidden", &script_effect::set_hidden
                , "add", &script_effect::add
                );

            lua.new_usertype<script_unit>("unit"
                , "mp", &script_unit::mp
                , "damage", &script_unit::damage
                , "deplete", &script_unit::deplete
                , "is_alive", &script_unit::is_alive
                , "is_enemy", &script_unit::is_enemy
                , "is_valid", &script_unit::is_valid
                , "place", &script_unit::place
                , "position", &script_unit::position
                , "apply_effect", &script_unit::apply_effect
                );

            lua.new_usertype<script_environment>("environment"
                , "distance", &script_environment::distance
                , "hit", &script_environment::hit
                , "damage", &script_environment::damage
                , "distance", &script_environment::distance
                , "popup", &script_environment::popup
                , "vfx", &script_environment::vfx
                , "clip", &script_environment::clip
                , "light", &script_environment::light
                , "spawn", &script_environment::spawn
                , "in_sight", &script_environment::in_sight
                , "in_line", &script_environment::in_line
                , "create_effect", &script_environment::create_effect
                , "is_traversable", &script_environment::is_traversable
                , "neighbour", &script_environment::neighbour
                );
        }

        void add_functions() {
            lua.script_file("data/scripts/lib_rupture.lua");
        }

    private:
        sol::state                              lua;
        std::map<std::string, sol::environment> skills;
    };
}