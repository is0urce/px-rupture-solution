// name: transform_component.hpp

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>
#include <px/es/transform.hpp>

#pragma once

namespace px {

	class transform_component
		: public transform<transform_component>
		, public component
		, public link_dispatcher<transform_component>
	{
	public:
		virtual void activate_component() override
		{
			insert();
		}
		virtual void deactivate_component() override
		{
			retract();
		}
	};
}