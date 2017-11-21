// name: craft_recipe.hpp
// type: c++ enum
// auth: is0urce
// desc: crafting recipe data

#pragma once

#include <px/rl/craft_activity.hpp>
#include <string>

namespace px::rl {

	struct craft_recipe {
		std::string			name;
		std::string			description;
		rl::craft_activity	activity;
		unsigned int		reagent_count;
		double				power_raw;
		double				power_enhancement;
	};
}