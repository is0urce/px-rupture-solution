// name: effect.hpp

#pragma once

#include <cstdint>

namespace px::rl {

	enum class effect : std::uint32_t {
		invalid				= 0,

		damage				= 10,
		dot					= 11,

		accuracy			= 20,
		critical			= 21,
		speed				= 22,

		power				= 30,
		strength			= 31,
		dexterity			= 32,
		intelligence		= 33,
		vitality			= 34,

		equipment			= 40,
		armor				= 41,
		resistance			= 42,
		dodge				= 43,

		hp_bonus			= 50,
		hp_regen			= 51,
		mp_bonus			= 52,
		mp_regen			= 53,

		essence				= 60,
		ingredient_power	= 61
	};
}