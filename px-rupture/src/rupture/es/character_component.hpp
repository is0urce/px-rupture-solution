#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>

namespace px {

	class character_component
		: public component
		, public link_dispatcher<character_component>
	{
	public:
		virtual ~character_component() = default;
	};
}