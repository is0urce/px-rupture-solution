#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>

namespace px {

	class animator_component
		: public component
		, public link_dispatcher<animator_component>
	{
	public:
		virtual ~animator_component() = default;
		animator_component() noexcept = default;
		animator_component(animator_component const&) = delete;
		animator_component & operator=(animator_component const&) = delete;
	};
}