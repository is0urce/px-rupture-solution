// name: deposite_component.hpp

#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>
#include <px/es/useable.hpp>

namespace px {

	class body_component;
	class environment;

	class deposite_component
		: public component
		, public link_dispatcher<deposite_component>
		, public useable<body_component *, environment *>
	{
	public:
		virtual ~deposite_component() = default;
		deposite_component() = default;
		deposite_component(deposite_component const&) = delete;
		deposite_component & operator=(deposite_component const&) = delete;
	};
}