// name: environment.cpp

#pragma once

#include "core.hpp"

#include "rl/terrain.hpp"

#include <px/memory/uq_ptr.hpp>

#include <vector>

namespace px {

	class transform_component;
	class composite_component;

	class environment
		: public core
	{
	public:
		void incarnate(transform_component * target);
		void step(point2 const& movement);
		void turn();
		void start();

	public:
		virtual ~environment();
		environment();

	protected:
		transform_component *	player;
		bool					run;
		unsigned int			turn_number;
		terrain					terrain;

		std::vector<uq_ptr<composite_component>> units;
	};
}