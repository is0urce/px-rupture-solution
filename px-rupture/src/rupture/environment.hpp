// name: environment.cpp

#pragma once

#include "core.hpp"

#include "es/notification_system.hpp"
#include "rl/scene.hpp"

#include <px/common/point.hpp>
#include <px/memory/uq_ptr.hpp>

namespace px {

	class transform_component;

	class environment
		: public core
	{
	public:
		void step(point2 const& movement);
		void start();
		unsigned int current_turn() const noexcept;
		void pass_turn();
		bool turn_passed() const noexcept; // true if it's world processing stage

	public:
		virtual ~environment();
		environment();

	private:
		void incarnate(transform_component * target);
		void spawn(const char* name, int x, int y);

	protected:
		transform_component *	player;
		bool					run;
		scene					stage;
		unsigned int			turn_number;
		bool					turn_pass;
		point2					last_step;
		notification_system		messages;
	};
}