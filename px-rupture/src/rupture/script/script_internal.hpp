#pragma once

#include "unit.hpp"

#include <px/dev/assert.hpp>

#include <sol.hpp>

#include <map>
#include <string>

namespace px {

	class body_component;
	class script_internal final
	{
	public:
		void run(std::string const& code)
		{
			lua.script(code);
		}
		void execute(std::string const& path)
		{
			lua.script_file(path);
		}
		rl::skill_functional<body_component *, body_component *, point2> impact(std::string const& path)
		{
			try {
				auto & sandbox = skills[path] = sol::environment(lua, sol::create, lua.globals());
				lua.script_file(path, sandbox);

				bool targeted = sandbox["targeted"].get_or(false);
				sol::function action = sandbox["action"];
				sol::function condition = sandbox["condition"];

				// functional
				auto target_action = [action](body_component * user, body_component * target) -> void {
					try {
						action(unit(user), unit(target));
					}
					catch (sol::error & error) {
						px::debug(error.what());
					}
				};

				auto area_action = [action](body_component * user, point2 const& area) -> void {
					try {
						action(unit(user), area);
					}
					catch (sol::error & error) {
						px::debug(error.what());
					}
				};


				auto target_condition = [condition](body_component * user, body_component * target) -> bool {
					try {
						return condition(unit(user), unit(target));
					}
					catch (sol::error & error) {
						px::debug(error.what());
						return false;
					}
				};
				auto area_condition = [condition](body_component * user, point2 const& area) -> bool {
					try {
						return condition(unit(user), area);
					}
					catch (sol::error & error) {
						px::debug(error.what());
						return false;
					}
				};

				// create record

				return { targeted, target_action, target_condition, area_action, area_condition };
			}
			catch (sol::error & error) {
				px::debug(error.what());
				return {};
			}
			catch (...) {
				throw std::runtime_error("px::script::load_skill(path), path=" + path);
			}
		}

	public:
		script_internal()
		{
			lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);

			lua.new_usertype<unit>("unit"
				, "damage", &unit::damage
				, "is_valid", &unit::is_valid
				, "place", &unit::place
				, "position", &unit::position
				);

			lua.new_usertype<point2>("point"
				, "x", &point2::x
				, "y", &point2::y
				);
		}

	private:
		sol::state								lua;
		std::map<std::string, sol::environment>	skills;
	};
}