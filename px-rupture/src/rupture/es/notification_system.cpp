// name: notification_sytem.cpp

#include "notification_system.hpp"

#include "rupture/draw/message_works.hpp"

#include "transform_component.hpp"

#include <px/memory/memory.hpp>

namespace px {

	notification_system::~notification_system()
	{
	}
	notification_system::notification_system()
		: works(make_uq<message_works>())
	{
	}

	void notification_system::turn_update_system(delta_type const& /*delta_time*/)
	{
		works->flatten();
	}

	void notification_system::send(message && msg, point2 const& location)
	{
		works->add_message(std::move(msg), location);
	}

	void notification_system::target(transform_component const* camera) noexcept
	{
		works->target(camera);
	}
	message_data const* notification_system::data() const noexcept
	{
		return works->data();
	}
}