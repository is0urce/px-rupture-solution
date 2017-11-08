// name: environment.cpp

#pragma once

#include "core.hpp"

#include "es/notification_system.hpp"
#include "rl/scene.hpp"
#include "io/repository.hpp"

#include <px/common/point.hpp>
#include <px/memory/uq_ptr.hpp>
#include <px/rl/damage_type.hpp>
#include <px/rl/hit_result.hpp>
#include <px/rl/workshop.hpp>

#include <string>

namespace px {

	class body_component;
	class transform_component;
	class composite_component;

	class environment
		: public core
	{
	public:
		bool								is_running() const noexcept;
		uq_ptr<composite_component>	&		spawn(uq_ptr<composite_component> unit, transform_component * hint);
		uq_ptr<composite_component> &		spawn(std::string const& blueprint, point2 const& position);
		void								step(point2 const& movement);		// move player
		void								advance();							// move player to target
		void								action(unsigned int action_idx);	// use skill
		void								use(unsigned int mods);				// use object
		void								start();
		transform_component	*				possessed() noexcept;
		transform_component *				target() noexcept;
		point2								area() const noexcept;
		void								focus(point2 offset);
		void								start_turn();						// clear from previous turn
		void								end_turn(unsigned int turns);		// end player turn and sart world processing
		bool								turn_passed() const noexcept;		// true if it's the world processing stage
		unsigned int						current_turn() const noexcept;		// returns turn number
		void								return_turn();						// pass turn back to player
		void								popup(std::string, color tint, point2 location);
		int									distance(point2 const& a, point2 const& b) const;
		rl::hit_result						hit(body_component const& source, body_component const& target) const;
		std::tuple<int, rl::damage_type>	dps(body_component const& source) const;
		void								damage(body_component & target, int damage, rl::damage_type dmg_type);
		bool								has_access(rl::workshop station) const noexcept;
		void								access_workshop(rl::workshop station);

	public:
		virtual								~environment();
		environment();

	private:
		void								incarnate(transform_component * target);
		void								lock_target();

	protected:
		notification_system					messages;
		scene								stage;

	private:
		transform_component *				player;
		repository							directory;
		bool								run;
		unsigned int						turn_number;
		bool								turn_pass;			// true if it's the world processing stage
		point2								target_hover;		// offset of target area
		point2								target_area;		// target area in world coordinates
		transform_component *				target_unit;		// targeted unit
		rl::workshop						opened_workshop;
	};
}