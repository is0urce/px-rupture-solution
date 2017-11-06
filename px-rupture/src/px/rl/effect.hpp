// name: effect.hpp

#pragma once

namespace px::rl {
	enum class effect : unsigned char {
		invalid = 0,

		damage = 1,
		dot = 2,
		accuracy = 3,
		critical = 4,
		speed = 5,

		power,
		strength,
		dexterity,
		intelligence,
		vitality,

		equipment,
		armor,
		resistance,
		dodge,

		hp_bonus,
		hp_regen,
		mp_bonus,
		mp_regen,

		ingredient_power,
		essence
	};
}