// name: sprite_component
// type: c++
// auth: is0urce
// desc: class declaration

#pragma once

#include "rupture/draw/sprite.hpp"

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>

namespace px {

	class transform_component;

	class sprite_component final
		: public component
		, public link<transform_component>
		, public link_dispatcher<sprite_component>
		, public sprite
	{
	public:
		virtual ~sprite_component() = default;
		sprite_component() = default;
		sprite_component(sprite_component const&) = delete;
		sprite_component & operator=(sprite_component const&) = delete;
	};
}