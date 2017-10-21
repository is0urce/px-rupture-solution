#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>

#include "rupture/rl/skill.hpp"
#include <px/rl/skill/skill_set.hpp>

namespace px {

	class character_component
		: public component
		, public link_dispatcher<character_component>
		, public rl::skill_set<skill>
	{
	public:
		virtual ~character_component() = default;
	};
}