// name: ui_system.hpp

#pragma once

#include <px/es/system.hpp>
#include <px/es/delta.hpp>

#include <px/memory/uq_ptr.hpp>

namespace px {

	class menu;

	class ui_system
		: public system<delta>
	{
	public:
		void toggle(bool up);

	public:
		virtual ~ui_system();
		ui_system(unsigned int width, unsigned int height);

	protected:
		virtual void update_system(delta_type const& delta_time);

	private:
		uq_ptr<menu> works;
	};
}