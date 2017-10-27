#pragma once

#include <cstdint>

namespace px::rl {

	class status {
	public:
		double			damage;
		double			dot;
		double			hp_bonus;
		double			accuracy;
		double			critical;
		double			ore_power;
		std::uint64_t	essence;
		
	public:
		status()
			: damage(0)
			, dot(0)
			, hp_bonus(0)
			, accuracy(0)
			, critical(0)
			, ore_power(0)
			, essence(1)
		{
		}

		template <typename Archive>
		void serialize(Archive & archive) {
			archive(damage, dot, hp_bonus, accuracy, critical, ore_power, essence);
		}

	public:
		status & operator+=(status & lh) noexcept {
			damage += lh.damage;
			dot += lh.dot;
			hp_bonus += lh.hp_bonus;
			accuracy += lh.accuracy;
			critical += lh.critical;
			ore_power += lh.ore_power;
		}
	};
}