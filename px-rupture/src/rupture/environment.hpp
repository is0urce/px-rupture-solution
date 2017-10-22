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
		void					pass_turn(point2 const& step);
		bool					turn_passed() const noexcept; // true if it's the world processing stage
		void					spawn(uq_ptr<composite_component> unit, transform_component * hint);
		void					spawn(std::string const& blueprint, point2 const& position);
		void					step(point2 const& movement);
		void					advance();
		void					start();
		transform_component	*	possessed() noexcept;
		transform_component *	target() noexcept;
		point2					area() const noexcept;
		void					focus(point2 offset);
		void					action(unsigned int action_idx);

	public:
		virtual ~environment();
		environment();

	private:
		void					incarnate(transform_component * target);
		void					lock_target();

	protected:
		notification_system		messages;
		scene					stage;
		script					mashine;

	private:
		transform_component *	player;
		repository				directory;
		bool					run;
		unsigned int			turn_number;
		bool					turn_pass;		// true if it's the world processing stage
		point2					last_step;
		point2					target_hover;	// offset of target area
		point2					target_area;	// target area in world coordinates
		transform_component *	target_unit;	// targeted unit
	};
}