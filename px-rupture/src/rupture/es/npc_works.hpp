// name: npc_works.hpp

#pragma once

#include "npc_component.hpp"
#include "transform_component.hpp"
#include "body_component.hpp"

#include "rupture/rl/scene.hpp"

#include <px/common/pool_chain.hpp>
#include <px/common/qtree.hpp>
#include <px/algorithm/fov.hpp>

namespace px {

	class npc_works	{
	public:
		void assign_scene(scene * world) noexcept {
			stage = world;
		}
		uq_ptr<npc_component> make() {
			return pool.make_uq();
		}

	public:
		void turn() {
			fov observed;
			pool.enumerate([&](npc_component & npc) {
				if (!npc.is_active()) return;

				// ensure valid composition
				transform_component * transform = npc.linked<transform_component>();
				if (!transform) return;
				body_component * body = transform->linked<body_component>();
				if (!body) return;

				// ensure npc can act
				if (!body->is_alive()) return;

				// health drop check
				if (npc.get_state() == rl::ai_state::idle && !body->health()->full()) {
					npc.set_state(rl::ai_state::alert);
					npc.destination() = transform->position(); // init waypoint to current position
				}

				// select enemy in range
				auto range = npc.get_range();
				observed.calculate(transform->position(), range, [&](int x, int y) { return stage->is_transparent({ x, y }); });
				transform_component * target = lock_target(transform->position(), *body, observed);
				if (target) {
					npc.set_state(rl::ai_state::alert);
					npc.destination() = target->position();
				}

				if (npc.get_state() == rl::ai_state::alert) {

					//point2 dest = npc.destination();
					//if (observed.contains(dest)) {
					//	transform->place(dest);
					//}
				}
			});
		}
	private:
		transform_component * lock_target(point2 const& center, body_component::standing_type const& faction, fov const& observe)
		{
			transform_component * result = nullptr;
			stage->space()->find(center, observe.range(), [&](int /*x*/, int /*y*/, transform_component * subject) {
				if (!subject) return;
				if (!observe.contains(subject->position())) return;

				if (auto subject_body = subject->linked<body_component>()) {
					if (faction.is_hostile(*subject_body)) {
						result = subject;
					}
				}
			});
			return result;
		}

	private:
		pool_chain<npc_component, 1024> pool;
		scene *							stage;
	};
}