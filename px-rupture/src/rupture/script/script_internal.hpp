#pragma once

#include <sol.hpp>

#include <string>

namespace px {
	class script_internal
	{
	public:
		void run(std::string const& code)
		{
			lua.script(code);
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