// name: environment.hpp
// type: c++
// auth: is0urce
// desc: class declaration

#pragma once

#include "core.hpp"

#include "es/notification_system.hpp"
#include "rl/scene.hpp"

#include <px/common/point.hpp>
#include <px/memory/uq_ptr.hpp>
#include <px/rl/damage_type.hpp>
#include <px/rl/hit_result.hpp>
#include <px/rl/craft_activity.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace px {

	class body_component;
	class transform_component;
	class composite_component;
	class repository;

	struct visual;

	class environment
		: public core
	{
	public:
		point2								area() const noexcept;		// hovered point
		transform_component *				target() noexcept;					// hover point in world
		void								target(point2 offset);				// hovered unit
		bool								is_running() const noexcept;
		void								step(point2 const& movement);		// move player
		void								advance();							// move player to target
		void								action(unsigned int action_idx);	// use skill
		void								use(unsigned int mods);				// use object
		void								function_edit(std::uint32_t idx);
		void								start();
		void								end();
		void								clear();
		transform_component	*				possessed() noexcept;
		void								start_turn();						// clear from previous turn
		void								end_turn(unsigned int turns);		// end player turn and sart world processing
		bool								turn_passed() const noexcept;		// true if it's the world processing stage
		unsigned int						current_turn() const noexcept;		// returns turn number
		void								return_turn();						// pass turn back to player
		void								popup(std::string, color tint, point2 location);
		void								popup(std::string, color tint);
		int									distance(point2 const& a, point2 const& b) const;
		rl::hit_result						hit(body_component const& source, body_component const& target) const;
		std::tuple<int, rl::damage_type>	dps(body_component const& source) const;
		void								damage(body_component & target, int damage, rl::damage_type dmg_type);
		void								give_experience(int experience, int level);
		bool								has_access(rl::craft_activity station) const noexcept;
		void								open_workshop(rl::craft_activity station);
		void								close_workshop();
		bool								save(std::string const& name);
		bool								load(std::string const& name);
		uq_ptr<composite_component>	&		spawn(uq_ptr<composite_component> unit);
		uq_ptr<composite_component> &		spawn(std::string const& blueprint, point2 const& position);
		void								emit_visual(std::string const& name, point2 start, point2 finish, transform_component * track);
		void								update_blueprints(std::string const& out_path, std::string const& blueprint);

	public:
		virtual								~environment();
		environment();

	private:
		void								incarnate(transform_component * target);
		void								lock_target();
		void								save_main();
		void								load_main();
		void								save_scene(point2 const& cell);
		void								load_scene(point2 const& cell);
		void								prewarm();							// start routine
		void								create_player();					// start routine

	protected:
		notification_system					messages;
		scene								stage;

	private:
		transform_component *				player;
		uq_ptr<repository>					current;
		uq_ptr<repository>					parent;
		bool								run;
		unsigned int						turn_number;
		bool								turn_pass;			// true if it's the world processing stage
		point2								target_hover;		// offset of target area
		point2								target_area;		// target area in world coordinates
		transform_component *				target_unit;		// targeted unit
		rl::craft_activity					opened_workshop;
		std::vector<visual>					vfx;
	};
}