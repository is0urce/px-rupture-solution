// name: unit.hpp

#pragma once

#include "rupture/es/body_component.hpp"

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
		transform_component * location() {
			return transform;
		}

	private:
		body_component *		body;
		transform_component *	transform;
	};

}