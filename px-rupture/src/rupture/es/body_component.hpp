// name: body_component.hpp

#include <px/es/component.hpp>

#include <px/es/link_dispatcher.hpp>

#include <px/rl/constitution.hpp>
#include <px/rl/standing.hpp>
#include <px/rl/mass.hpp>
#include <px/rl/traverse.hpp>

namespace px {

	class transform_component;

	class body_component
		: public component
		, public link_dispatcher<body_component>
		, public rl::constitution
		, public rl::standing
	{
	public:
		rl::mass<rl::traverse> const& blocking() const
		{
			return mass;
		}
		rl::mass<rl::traverse> & blocking()
		{
			return mass;
		}

	public:
		virtual ~body_component() noexcept = default;
		body_component() noexcept = default;
		body_component(body_component const&) noexcept = delete;
		body_component & operator=(body_component const&) noexcept = delete;

	private:
		rl::mass<rl::traverse> mass;
	};
}