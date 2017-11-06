// name: script_unit.hpp
// type: c++

#pragma once

#include "rupture/es/body_component.hpp"
#include "rupture/es/transform_component.hpp"

namespace px {

	class script_unit
	{
	public:
		void add_effect(std::string name, int duration, int effect_id, int subtype, int i0, int i1, double v0, double v1) {
			if (body) {
				body_component::buff_type buff;
				buff.set_name(name);
				buff.set_duration(duration);
				buff.add(body_component::enhancement_type::mixed(static_cast<rl::effect>(effect_id), subtype, i0, i1, v0, v1));
				body->add(buff);
			}
		}
		void damage(int amount) {
			if (body) {
				auto & hp = body->health();
				if (hp) hp->damage(amount);
			}
		}
		bool is_alive() const {
			return body && body->is_alive();
		}
		bool is_valid() const noexcept {
			return body != nullptr;
		}
		bool is_enemy(script_unit const& subject) const {
			return body && subject.body && body->is_hostile(*subject.body);
		}
		void place(point2 target) {
			auto pawn = get_transform();
			if (pawn) {
				pawn->place(target);
			}
		}
		int mp() const {
			if (!body) return 0;
			auto const& mp = body->energy();
			if (!mp) return 0;
			return mp->current();
		}
		void deplete(int val) {
			if (!body) return;
			auto & mp = body->energy();
			if (!mp) return;
			mp->damage(val);
		}
		point2 position() {
			auto pawn = get_transform();
			return pawn ? pawn->position() : point2(0, 0);
		}

		body_component * get_body() {
			return body;
		}
		body_component const* get_body() const {
			return body;
		}
		transform_component * get_transform() {
			return body ? body->linked<transform_component>() : nullptr;
		}

	public:
		script_unit()
			: body(nullptr)
			, transform(nullptr)
		{
		}
		script_unit(body_component * user)
			: body(user)
			, transform(nullptr)
		{
		}

	private:
		body_component *		body;
		transform_component *	transform;
	};

}