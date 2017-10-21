// name: environment.cpp

#pragma once

#include "core.hpp"

#include "es/notification_system.hpp"
#include "rl/scene.hpp"
#include "io/repository.hpp"
#include "script/script.hpp"

#include <px/common/point.hpp>
#include <px/memory/uq_ptr.hpp>

#include <string>

namespace px {

	class transform_component;
	class composite_component;

	class environment
		: public core
	{
	public:
		unsigned int			current_turn() const noexcept;
		bool					is_running() const noexcept;
		void					pass_turn();
		bool					turn_passed() const noexcept; // true if it's the world processing stage
		void					spawn(uq_ptr<composite_component> unit, transform_component * hint);
		void					spawn(std::string const& blueprint, point2 const& position);
		void					step(point2 const& movement);
		void					start();
		transform_component	*	possessed() noexcept;

	public:
		virtual ~environment();
		environment();

	private:
		void					incarnate(transform_component * target);

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
		script					code;
	};
}