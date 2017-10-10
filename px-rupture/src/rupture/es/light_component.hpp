// name: light_component.hpp

#include <px/es/component.hpp>
#include <px/es/light.hpp>

namespace px {

	class light_component
		: public component
		, public light
	{
	public:
		virtual ~light_component()
		{
		}
		light_component()
		{
		}
		light_component(light_component const&) = delete;
		light_component & operator=(light_component const&) = delete;
	};
}