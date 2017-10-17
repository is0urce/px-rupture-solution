// name: core.hpp

#pragma once

#include "es/animator_system.hpp"
#include "es/body_system.hpp"
#include "es/light_system.hpp"
#include "es/npc_system.hpp"
#include "es/sprite_system.hpp"
#include "es/transform_system.hpp"

namespace px {

	class core
	{
	public:
		virtual ~core();
		core();

	public:
		sprite_system		sprites;
		transform_system	transforms;
		animator_system		animators;
		light_system		lights;
		body_system			bodies;
		npc_system			npcs;		// non player characters manager
	};
}