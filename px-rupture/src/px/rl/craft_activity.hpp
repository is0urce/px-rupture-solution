// name: craft_activity.hpp
// type: c++ enum
// auth: is0urce
// desc: crafting activity enumeration

#pragma once

namespace px::rl {

	enum class craft_activity : unsigned char {
		none = 0,
		blacksmith = 1,
		alchemy = 2
	};
}