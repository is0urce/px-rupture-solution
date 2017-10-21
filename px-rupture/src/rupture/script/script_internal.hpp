#pragma once

#include <px/common/point.hpp>
#include "rupture/rl/skill.hpp"

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
			//try
			//{
				auto & sandbox = skills[path] = sol::environment(lua, sol::create, lua.globals());
				lua.script_file(path, sandbox);

				bool targeted = sandbox["targeted"].get_or(false);

				// functional
				auto target_action = [&sandbox](body_component * user, body_component * target) -> void {
					try
					{
						//sandbox["action"](script_unit(user), script_unit(target));
						sandbox["action"]();
					}
					catch (sol::error & lua_error)
					{
						//px_assert_nothrow(lua_error);
					}
				};
				auto target_condition = [&sandbox](body_component * user, body_component * target) -> bool {
					try
					{
						//sol::function_result script_result = sandbox["condition"](script_unit(user), script_unit(target));
						return true; //script_result.get<bool>();
					}
					catch (sol::error & lua_error)
					{
						//px_assert_nothrow(lua_error);
						return false;
					}
				};
				auto area_action = [&sandbox](body_component * user, point2 const& area) -> void {
					try
					{
						//sandbox["action"](script_unit(user), area);
					}
					catch (sol::error & lua_error)
					{
						//px_assert_nothrow(lua_error);
					}
				};
				auto area_condition = [&sandbox](body_component * user, point2 const& area) -> bool {
					try
					{
						//sol::function_result script_result = sandbox["condition"](script_unit(user), area);
						return true;// script_result.get<bool>();
					}
					catch (sol::error & lua_error)
					{
						//px_assert_nothrow(lua_error);
						return false;
					}
				};

				// create record

				return { targeted, target_action, target_condition, area_action, area_condition };
			//}
			//catch (sol::error & script_error)
			//{
			//	//px_assert_nothrow(script_error);
			//}
			//catch (...)
			//{
			//	throw std::runtime_error("px::script::load_skill(path), path=" + path);
			//}
		}

	public:
		script_internal()
		{
			lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
		}

	private:
		sol::state								lua;
		std::map<std::string, sol::environment>	skills;
	};
}