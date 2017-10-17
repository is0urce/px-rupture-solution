// name: light_system.hpp

#pragma once

#include <px/es/system.hpp>
#include <px/es/delta.hpp>

#include <px/common/point.hpp>
#include <px/common/color.hpp>
#include <px/memory/uq_ptr.hpp>

#include <vector>

namespace px {

	class message;
	class message_data;
	class message_works;
	class transform_component;

	class notification_system final
		: public system<delta>
	{
	public:
		void					target(transform_component const* camera) noexcept;
		void					send(message && msg, point2 const& location);
		message_data const*		data() const noexcept;

	public:
		virtual ~notification_system();
		notification_system();

	protected:
		virtual void			turn_update_system(delta_type const& delta_time) override;

	private:
		uq_ptr<message_works>	works;
	};
}