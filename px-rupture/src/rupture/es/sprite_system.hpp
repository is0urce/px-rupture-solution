// name: sprite_system.hpp

#pragma once

#include <px/es/system.hpp>
#include <px/es/delta.hpp>

#include <px/memory/uq_ptr.hpp>

#include <string>
#include <vector>

namespace px {
	
	class sprite_component;
	class sprite_works;
	class transform_component;
	struct sprite_vertex;

	class sprite_system final
		: public system<delta>
	{
	public:
		uq_ptr<sprite_component> make(std::string const& name);
		std::vector<sprite_vertex> const* data() const noexcept;
		void target(transform_component const*) noexcept;

	public:
		virtual ~sprite_system();
		sprite_system();

	protected:
		virtual void update_system(delta_type const& delta_time);

	private:
		void load();
		
	private:
		uq_ptr<sprite_works> works;
	};
}