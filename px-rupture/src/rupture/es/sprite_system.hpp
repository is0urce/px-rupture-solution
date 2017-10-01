// name: sprite_system.hpp

#pragma once

#include <px/es/system.hpp>
#include <px/es/delta.hpp>

#include <px/memory/uq_ptr.hpp>

namespace px {
	
	class sprite_component;
	class sprite_works;

	class sprite_system final
		: public system<delta>
	{
	public:
		virtual ~sprite_system();
		sprite_system();

	protected:
		virtual void update_system(delta_type const& delta_time);
		
	private:
		uq_ptr<sprite_works> works;
	};
}