// name: npc_component.hpp

#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>

namespace px {

	class npc_component
		: public component
		, public link_dispatcher<npc_component>
	{
	public:
		virtual ~npc_component() = default;
	};
}