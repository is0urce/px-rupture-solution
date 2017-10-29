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
				transform_component * transform = npc.linked<transform_component>();
				if (!transform) return;
				body_component * body = transform->linked<body_component>();
				if (!body) return;

				if (npc.get_state() == rl::ai_state::idle) {
					npc.set_state(rl::ai_state::alert);
				}

				if (npc.get_state() == rl::ai_state::alert) {
					//auto range = npc.get_range();
					unsigned int range = 20;
					observed.calculate(transform->position(), range, [&](int x, int y) { return stage->is_transparent({ x, y }); });

					transform_component * target = lock_target(transform, body, range);

					if (target) {
						point2 dest = target->position();
						if (observed.contains(dest)) {
							transform->place(dest);
						}
					}
				}
			});
		}
	private:
		transform_component * lock_target(transform_component * transform, body_component * body, unsigned int range)
		{
			transform_component * result = nullptr;
			stage->space()->find(transform->position(), range, [&](int /*x*/, int /*y*/, transform_component * subject) {
				if (subject) {
					if (auto subject_body = subject->linked<body_component>()) {
						if (body->is_hostile(*subject_body)) {
							result = subject;
						}
					}
				}
				result = subject;
			});
			return result;
		}

	private:
		pool_chain<npc_component, 1024> pool;
		scene *							stage;
	};
}