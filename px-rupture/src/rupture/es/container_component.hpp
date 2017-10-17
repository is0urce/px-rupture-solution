#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>

namespace px {

	class container_component
		: public component
		, public link_dispatcher<container_component>
	{
	public:
		virtual ~container_component() = default;
	};
}