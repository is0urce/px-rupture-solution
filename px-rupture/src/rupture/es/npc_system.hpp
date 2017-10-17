// name: npc_system.hpp

#pragma once

#include <px/es/system.hpp>
#include <px/es/delta.hpp>

#include <px/memory/uq_ptr.hpp>

namespace px {

	class npc_component;
	class npc_works;

	class npc_system
		: public system<delta>
	{
	public:
		uq_ptr<npc_component> make();

	public:
		virtual ~npc_system();
		npc_system();

	protected:
		virtual void turn_update_system(delta_type const& delta_time) override;

	private:
		uq_ptr<npc_works> works;
	};
}