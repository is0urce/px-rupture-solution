// name: sprite_component
// type: c++
// auth: is0urce
// desc: class declaration

#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>

namespace px {

	class transform_component;

	class sprite_component
		: component
		, link<transform_component>
		, link_dispatcher<sprite_component>
	{
	};
}