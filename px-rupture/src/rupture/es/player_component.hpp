// name: player_component.hpp

#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>

namespace px {

	class player_component
		: public component
		, public link_dispatcher<player_component>
	{
	public:
		virtual ~player_component() = default;
	};
}