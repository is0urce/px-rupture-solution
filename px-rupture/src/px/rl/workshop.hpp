// name: workshop.hpp
// type: c++ enum
// auth: is0urce
// desc: crafting stations enumeration

#pragma once

namespace px::rl {
	enum class workshop : unsigned char {
		none = 0,
		blacksmith = 1,
		alchemy = 2
	};
}