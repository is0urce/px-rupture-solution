#pragma once

#include <cstdint>

namespace px::rl {

	class status {
	public:
		double			power;
		double			strength;
		double			dexterity;
		double			intelligence;
		double			vitality;

		std::uint32_t	equipment;
		double			damage;
		double			armor;
		double			resistance;

		double			hp_bonus;
		double			hp_regen;
		double			mp_bonus;
		double			mp_regen;

		double			dot;
		double			accuracy;
		double			critical;
		double			speed;

		double			ingredient_power;

		std::uint64_t	essence;
		
	public:
		status()
			: power(0)
			, strength(0)
			, dexterity(0)
			, intelligence(0)
			, vitality(0)
			, equipment(0)
			, damage(0)
			, armor(0)
			, resistance(0)
			, dot(0)
			, hp_bonus(0)
			, hp_regen(0)
			, mp_bonus(0)
			, mp_regen(0)
			, accuracy(0)
			, critical(0)
			, speed(0)
			, ingredient_power(0)
			, essence(0)
		{
		}

		template <typename Archive>
		void serialize(Archive & archive) {
			archive(power, strength, dexterity, intelligence);
			archive(equipment, damage, armor, resistance);
			archive(hp_bonus, hp_regen, mp_bonus, mp_regen);
			archive(dot);
			archive(accuracy, critical, speed);
			archive(ingredient_power, essence);
		}

	public:
		status & operator+=(status & lh) noexcept {
			power += lh.power;
			damage += lh.damage;
			armor += lh.armor;
			dot += lh.dot;
			hp_bonus += lh.hp_bonus;
			accuracy += lh.accuracy;
			critical += lh.critical;
			ingredient_power += lh.ingredient_power;
		}
	};
}