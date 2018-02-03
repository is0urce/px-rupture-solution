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
		useable				= 41,

		armor				= 51,
		resistance			= 52,
		dodge				= 53,

		hp_bonus			= 60,
		hp_regen			= 61,
		mp_bonus			= 62,
		mp_regen			= 63,

		essence				= 70,
		ingredient_power	= 81
	};
}