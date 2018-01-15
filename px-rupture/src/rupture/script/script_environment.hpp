// name: script_environment.hpp
// type: c++
// auth: is0urce
// desc: script bindings for game object unit

#pragma once

#include "script_unit.hpp"

#include "rupture/environment.hpp"

#include "rupture/es/composite_component.hpp"

#include <string>
#include <tuple>

namespace px {

	class script_environment {
	public:
		int distance(point2 const& a, point2 const& b) {
			return game->distance(a, b);
		}

		void popup(std::string text, int rgba, point2 location) {
			game->popup(text, color(rgba), location);
		}
		//void emit_vfx(point2 location, std::string const& tag)
		//{
		//	m_environment->visual(tag, location, location, nullptr);
		//}
		//void emit_projectile(point2 start, point2 end, std::string const& tag)
		//{
		//	m_environment->visual(tag, start, end, nullptr);
		//}
		//void emit_missile(point2 start, script_unit & target, std::string const& tag)
		//{
		//	m_environment->visual(tag, start, target.position(), target.transform());
		//}
		//void emit_decal(script_unit & target, std::string const& tag)
		//{
		//	m_environment->visual(tag, target.last_position(), target.position(), target.transform());
		//}

		void vfx(std::string img, point2 start, point2 finish, script_unit & track) {
			game->emit_visual(img, start, finish, track.get_transform());
		}

		// simulate melee hit event ang get it results
		std::tuple<int, int, bool, bool> hit(script_unit const& attacker, script_unit const& target) {
			body_component const* user_body = attacker.get_body();
			body_component const* target_body = target.get_body();

			int damage = 0;
			bool critical = false;
			bool connects = false;
			rl::damage_type damage_type = rl::damage_type::pure;

			if (user_body && target_body) {
				rl::hit_result hit = game->hit(*user_body, *target_body);
				std::tie(damage, damage_type) = game->dps(*user_body);
				switch (hit) {
				case rl::hit_result::connects:
				case rl::hit_result::critical:
					connects = true;
					break;
				case rl::hit_result::failure:
				case rl::hit_result::miss:
				default:
					break;
				}
			}

			return { damage, static_cast<int>(damage_type), connects, critical };
		}

		script_unit spawn(std::string const& blueprint_tag, point2 const& location) {
			auto & composite = game->spawn(blueprint_tag, location);
			return script_unit(composite ? composite->qlink<body_component, transform_component>() : nullptr);
		}

		void damage(script_unit & target, int damage, int damage_type) {
			if (auto * body = target.get_body()) {
				game->damage(*body, damage, static_cast<rl::damage_type>(damage_type));
			}
		}

	public:
		script_environment() noexcept
			: game(nullptr)
		{
		}
		script_environment(environment * environment) noexcept
			: game(environment)
		{
		}

	private:
		environment * game;
	};
}