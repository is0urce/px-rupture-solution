#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>

#include "rupture/rl/inventory.hpp"

namespace px {

	class container_component
		: public component
		, public link_dispatcher<container_component>
		, public rl::inventory
	{
	public:
		template <typename Archive>
		void serialize(Archive & /*archive*/) {
			//archive(static_cast<rl::inventory &>(*this));
		}

	public:
		virtual ~container_component() = default;
		container_component() = default;
		container_component(container_component const&) = delete;
		container_component & operator=(container_component const&) = delete;
	};
}