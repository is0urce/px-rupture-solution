// name: core.hpp

#pragma once

#include "es/animator_system.hpp"
#include "es/body_system.hpp"		// effects
#include "es/character_system.hpp"	// cooldowns
#include "es/light_system.hpp"		// lightmap
#include "es/npc_system.hpp"		// actions
#include "es/sprite_system.hpp"
#include "es/transform_system.hpp"	// store position

namespace px {

	class core
	{
	public:
		virtual ~core();
		core();

	public:
		animator_system		animators;
		body_system			bodies;
		character_system	characters;
		light_system		lights;
		npc_system			npcs;		// non player characters manager
		sprite_system		sprites;
		transform_system	transforms;
	};
}