// name: environment.cpp

#pragma once

#include "core.hpp"

#include "rl/scene.hpp"

#include <px/memory/uq_ptr.hpp>

#include <vector>

namespace px {

	class transform_component;

	class environment
		: public core
	{
	public:

		void step(point2 const& movement);
		void turn();
		void start();

	public:
		virtual ~environment();
		environment();

	private:
		void incarnate(transform_component * target);
		void spawn(const char* name, int x, int y);

	protected:
		transform_component *	player;
		bool					run;
		unsigned int			turn_number;
		scene					stage;
	};
}