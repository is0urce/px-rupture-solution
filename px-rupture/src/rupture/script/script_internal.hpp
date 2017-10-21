#pragma once

#include <px/common/point.hpp>
#include "rupture/rl/skill.hpp"

#include <sol.hpp>

#include <string>

namespace px {

	class body_component;
	class script_internal
	{
	public:
		void run(std::string const& code)
		{
			lua.script(code);
		}
		void execute(std::string const& /*path*/)
		{
		}
		skill & impact(std::string const& /*name*/)
		{
			skill a;
			return a;
		}

	public:
		script_internal()
		{
			lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
		}

	private:
		sol::state lua;
	};
}