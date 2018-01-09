// name: transform_component.cpp
// type: c++
// auth: is0urce
// desc: class methods implementation

#include "transform_component.hpp"

#include <px/common/qtree.hpp>

namespace px {

	// ctor / dtor

	transform_component::~transform_component() {
		if (is_active() && !background) {
			retract();
		}
	}

	transform_component::transform_component() noexcept
		: space(nullptr)
		, background(false)
	{
	}
	transform_component::transform_component(point2 position) noexcept
		: transform(position)
		, space(nullptr)
		, background(false)
	{
	}

	// virtual overloads

	void transform_component::activate_component() {
		if (!background) {
			insert();
		}
	}
	void transform_component::deactivate_component() {
		if (!background) {
			retract();
		}
	}

	// static methods

	bool transform_component::is_static() const noexcept {
		return background;
	}
	void transform_component::make_static() {
		if (!background) {
			if (is_active()) {
				retract();
			}
			background = true;
		}
	}
	void transform_component::make_dynamic() {
		if (background) {
			background = false;
			if (is_active()) {
				insert();
			}
		}
	}
	void transform_component::set_static(bool static_flag) {
		if (static_flag) {
			make_static();
		}
		else {
			make_dynamic();
		}
	}

	void transform_component::move(point2 const& direction) {
		place(current + direction);
	}

	void transform_component::place(point2 destination) {
		if (is_active() && !background) {
			project(destination);
		}
		current = destination;
	}

	void transform_component::insert() {
		if (space) {
			space->add(current, this);
		}
	}
	void transform_component::retract() {
		if (space) {
			space->remove(current, this);
		}
	}
	void transform_component::project(point2 destination) {
		if (space) {
			space->move(current, this, destination);
		}
	}

	void transform_component::incarnate(qtree<transform_component*> * world) {
		if (is_active() && !background) {
			if (space) space->remove(current, this);
			if (world) world->add(current, this);
		}
		space = world;
	}

	qtree<transform_component*> * transform_component::world() const noexcept {
		return space;
	}

	void transform_component::swap(transform_component & that) {
		std::swap(current, that.current);
		std::swap(prev, that.prev);
		std::swap(space, that.space);
		std::swap(background, that.background);
	}
}