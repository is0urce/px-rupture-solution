#pragma once

#include "core.hpp"

#include "es/transform_component.hpp"

namespace px {

	class transform_component;

	class environment
		: public core
	{
	public:
		void incarnate(transform_component * target)
		{
			player = target;
			sprites.target(target);
		}
		void step(point2 const& movement)
		{
			if (player) {
				auto destination = player->position() + movement;
				player->place(destination);
				player->store();
			}
		}
		void turn()
		{
			++turn_number;
		}

	public:
		environment(unsigned int w, unsigned int h)
			: core(w, h)
			, turn_number(0)
		{
		}

	private:
		transform_component * player;
		unsigned int turn_number;
	};
}