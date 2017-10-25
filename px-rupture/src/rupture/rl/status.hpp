#pragma once

namespace px::rl {

	class status {
	public:
		double damage;
		double dot;
		double hp_bonus;
		double accuracy;
		double crit;
		
	public:
		status()
			: damage(0)
			, dot(0)
			, hp_bonus(0)
			, accuracy(0)
			, crit(0)
		{
		}

	public:
		status & operator+=(status & lh) {
			damage += lh.damage;
			dot += lh.dot;
			hp_bonus += lh.hp_bonus;
			crit += lh.crit;
		}
	};
}