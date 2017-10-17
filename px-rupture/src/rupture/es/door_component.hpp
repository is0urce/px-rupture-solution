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
	};

	class storage_component
		: public component
		, public link_dispatcher<storage_component>
		, public useable<body_component *, environment *>
	{
	public:
		virtual ~storage_component() = default;
	};

	class deposite_component
		: public component
		, public link_dispatcher<deposite_component>
		, public useable<body_component *, environment *>
	{
	public:
		virtual ~deposite_component() = default;
	};

	class workshop_component
		: public component
		, public link_dispatcher<workshop_component>
		, public useable<body_component *, environment *>
	{
	public:
		virtual ~workshop_component() = default;
	};
}