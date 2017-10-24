// name: unit.hpp

#pragma once

#include "rupture/es/body_component.hpp"
#include "rupture/es/transform_component.hpp"

namespace px {

	class transform_component;
	class unit
	{
	public:
		void damage(int amount) {
			if (body) {
				auto & hp = body->health();
				if (hp) hp->damage(amount);
			}
		}
		bool is_valid() const noexcept {
			return body != nullptr;
		}
		void place(point2 target) {
			auto pawn = get_transform();
			if (pawn) {
				pawn->place(target);
			}
		}
		point2 position() {
			auto pawn = get_transform();
			return pawn ? pawn->position() : point2(0, 0);
		}

	public:
		unit()
			: body(nullptr)
			, transform(nullptr)
		{
		}
		unit(body_component * user)
			: body(user)
			, transform(nullptr)
		{
		}

	private:
		transform_component * get_transform() {
			if (transform) return transform;
			if (body) transform = body->linked<transform_component>();
			return transform;
		}

	private:
		body_component *		body;
		transform_component *	transform;
	};

}