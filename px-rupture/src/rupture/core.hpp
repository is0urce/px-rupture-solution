// name: core.hpp

#pragma once

#include "es/sprite_system.hpp"
#include "es/transform_system.hpp"
#include "es/light_system.hpp"
#include "es/body_system.hpp"

namespace px {

	class core
	{
	public:
		virtual ~core();
		core();

	public:
		sprite_system		sprites;
		transform_system	transforms;
		light_system		lights;
		body_system			bodies;
	};
}