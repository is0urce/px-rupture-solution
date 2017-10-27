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
		template <typename Archive>
		void serialize(Archive & /*archive*/) {
		}

	public:
		virtual ~player_component() = default;
	};
}