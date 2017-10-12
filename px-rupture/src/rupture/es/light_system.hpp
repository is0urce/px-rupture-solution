// name: light_system.hpp

#pragma once

#include <px/es/system.hpp>
#include <px/es/delta.hpp>

#include <px/memory/uq_ptr.hpp>

namespace px {

	class light_works;
	class light_component;
	class transform_component;
	class lightmap_data;

	class light_system final
		: public system<delta>
	{
	public:
		uq_ptr<light_component> make();
		void target(transform_component const* camera) noexcept;
		lightmap_data const* current_data() noexcept;
		lightmap_data const* last_data() noexcept;

	public:
		virtual ~light_system();
		light_system();

	protected:
		virtual void update_system(delta_type const& delta_time);

	private:
		uq_ptr<light_works> works;
	};
}