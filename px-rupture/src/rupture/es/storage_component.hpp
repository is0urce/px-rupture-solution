#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>
#include <px/es/useable.hpp>

namespace px {

	class body_component;
	class environment;

	class storage_component
		: public component
		, public link_dispatcher<storage_component>
		, public useable<body_component *, environment *>
	{
	public:
		virtual ~storage_component() = default;
		storage_component() = default;
		storage_component(storage_component const&) = delete;
		storage_component & operator=(storage_component const&) = delete;
	};
}