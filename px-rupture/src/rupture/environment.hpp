// name: environment.cpp

#pragma once

#include "core.hpp"

#include "es/notification_system.hpp"
#include "rl/scene.hpp"
#include "io/repository.hpp"

#include <px/common/point.hpp>
#include <px/memory/uq_ptr.hpp>

namespace px {

	class transform_component;

	class environment
		: public core
	{
	public:
		void					step(point2 const& movement);
		void					start();
		unsigned int			current_turn() const noexcept;
		void					pass_turn();
		bool					turn_passed() const noexcept; // true if it's the world processing stage
		bool					is_running() const noexcept;

	public:
		virtual ~environment();
		environment();

	private:
		void					incarnate(transform_component * target);
		void					spawn(const char* name, int x, int y);

	protected:
		notification_system		messages;
		scene					stage;

	private:
		transform_component *	player;
		bool					run;
		unsigned int			turn_number;
		bool					turn_pass;		// true if it's the world processing stage
		point2					last_step;
		repository				directory;
	};
}