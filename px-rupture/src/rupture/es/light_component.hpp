// name: light_component.hpp

#include <px/es/component.hpp>
#include <px/es/light.hpp>

namespace px {

	class transform_component;

	class light_component
		: public component
		, public link<transform_component>
		, public link_dispatcher<light_component>
		, public light
	{
	public:
		virtual ~light_component() noexcept = default;
		light_component() noexcept = default;
		light_component(light_component const&) noexcept = delete;
		light_component & operator=(light_component const&) noexcept = delete;
	};
}