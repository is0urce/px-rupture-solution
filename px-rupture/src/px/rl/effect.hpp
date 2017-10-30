#pragma once

namespace px::rl {
	enum class effect : unsigned char {
		power,
		strength,
		dexterity,
		intelligence,
		vitality,

		equipment,
		damage,
		armor,
		resistance,

		hp_bonus,
		hp_regen,
		mp_bonus,
		mp_regen,

		dot,
		accuracy,
		critical,
		speed,

		ingredient_power,
		essence
	};
}