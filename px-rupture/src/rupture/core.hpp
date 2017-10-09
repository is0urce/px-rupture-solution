// name: core.hpp

#pragma once

#include "es/sprite_system.hpp"
#include "es/transform_system.hpp"

namespace px {

	class core
	{
	public:
		virtual ~core();
		core();

	protected:
		sprite_system		sprites;
		transform_system	transforms;
	};
}