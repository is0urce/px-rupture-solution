#pragma once

#include <px/es/system.hpp>
#include <px/es/delta.hpp>

#include <px/memory/uq_ptr.hpp>

#include <string>

namespace px {

	class animator_component;
	class animator_works;

	class animator_system
		: public system<delta>
	{
	public:
		uq_ptr<animator_component> make(std::string const& name);

	public:
		virtual ~animator_system();
		animator_system();

	protected:
		virtual void update_system(delta_type const& delta_time) override;
		virtual void turn_update_system(delta_type const& delta_time) override;

	private:
		uq_ptr<animator_works> works;
	};
}