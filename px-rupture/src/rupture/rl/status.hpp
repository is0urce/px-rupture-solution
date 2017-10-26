#pragma once

namespace px::rl {

	class status {
	public:
		double damage;
		double dot;
		double hp_bonus;
		double accuracy;
		double critical;
		
	public:
		status()
			: damage(0)
			, dot(0)
			, hp_bonus(0)
			, accuracy(0)
			, critical(0)
		{
		}

		template <typename Archive>
		void serialize(Archive & archive) {
			archive(damage, dot, hp_bonus, accuracy, critical);
		}

	public:
		status & operator+=(status & lh) noexcept {
			damage += lh.damage;
			dot += lh.dot;
			hp_bonus += lh.hp_bonus;
			critical += lh.critical;
		}
	};
}