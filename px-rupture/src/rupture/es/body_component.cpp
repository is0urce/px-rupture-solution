// name: body_component.cpp

#include "body_component.hpp"

namespace px {

	body_component::body_component() noexcept
		: button(nullptr)
	{

	}
	body_component::~body_component()
	{
	}

	// methods

	void body_component::assign_useable(useable_type * useable) noexcept
	{
		button = useable;
	}
	rl::mass<rl::traverse> const& body_component::blocking() const noexcept
	{
		return mass;
	}
	rl::mass<rl::traverse> & body_component::blocking() noexcept
	{
		return mass;
	}

	body_component::movement_type const& body_component::movement() const noexcept
	{
		return traverse_opts;
	}
	body_component::movement_type &	body_component::movement() noexcept
	{
		return traverse_opts;
	}

	bool body_component::is_useable() const noexcept
	{
		return button != nullptr;
	}
	bool body_component::can_use(body_component * user, environment * env) const
	{
		return button && button->can_use(user, env);
	}
	void body_component::use(body_component * user, environment * env)
	{
		if (button) {
			button->use(user, env);
		}
	}
	bool body_component::try_use(body_component * user, environment * env)
	{
		return button && button->try_use(user, env);
	}
}