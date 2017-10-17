// name: door_component.hpp

#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>
#include <px/es/useable.hpp>

namespace px {

	class body_component;
	class environment;

	class door_component
		: public component
		, public link_dispatcher<door_component>
		, public useable<body_component *, environment *>
	{
	public:
		virtual ~door_component() = default;
		door_component() = default;
		door_component(door_component const&) = delete;
		door_component & operator=(door_component const&) = delete;
	};
}