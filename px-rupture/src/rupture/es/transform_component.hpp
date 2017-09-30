// name: transform_component.hpp

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>

#include <px/es/transform.hpp>

#pragma once

namespace px {

	// forward declarations
	template <typename T> class qtree;

	class transform_component
		: public component
		, public link_dispatcher<transform_component>
		, public transform
	{
	public:
		void incarnate(qtree<transform_component*> * world);	// set space
		qtree<transform_component*> * world() const noexcept;	// get current space
		void move(point2 const& direction);						// move by offset
		void place(point2 destination);							// place into specified location

	public:
		virtual ~transform_component();
		transform_component();

	protected:
		virtual void activate_component() override;
		virtual void deactivate_component() override;

	private:
		void insert();
		void retract();

	private:
		qtree<transform_component*> * space;
	};
}