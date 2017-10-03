// name: transform_component.cpp

#include "transform_component.hpp"

#include <px/common/qtree.hpp>

namespace px {

	// ctor / dtor

	transform_component::~transform_component()
	{
	}
	transform_component::transform_component()
		: space(nullptr)
	{
	}
	transform_component::transform_component(point2 position)
		: transform(position)
		, space(nullptr)
	{
	}

	// virtual overloads

	void transform_component::activate_component()
	{
		insert();
	}
	void transform_component::deactivate_component()
	{
		retract();
	}

	// static methods

	void transform_component::move(point2 const& direction)
	{
		place(current + direction);
	}

	void transform_component::place(point2 destination)
	{
		if (is_active() && space) {
			space->move(current, this, destination);
		}
		current = destination;
	}

	void transform_component::insert()
	{
		if (!is_active() && space) {
			space->add(current, this);
		}
	}
	void transform_component::retract()
	{
		if (is_active() && space) {
			space->remove(current, this);
		}
	}

	void transform_component::incarnate(qtree<transform_component*> * world)
	{
		if (is_active()) {
			if (space) space->remove(current, this);
			if (world) world->add(current, this);
		}
		space = world;
	}

	qtree<transform_component*> * transform_component::world() const noexcept
	{
		return space;
	}
}