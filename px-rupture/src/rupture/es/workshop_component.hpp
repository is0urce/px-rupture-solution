#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>
#include <px/es/useable.hpp>

namespace px {

	class body_component;
	class environment;

	class workshop_component
		: public component
		, public link_dispatcher<workshop_component>
		, public useable<body_component *, environment *>
	{
	public:
		virtual ~workshop_component() = default;
		workshop_component() = default;
		workshop_component(workshop_component const&) = delete;
		workshop_component & operator=(workshop_component const&) = delete;
	};
}