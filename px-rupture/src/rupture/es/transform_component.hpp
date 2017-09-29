// name: transform_component.hpp

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>

#include <px/common/point.hpp>

#pragma once

namespace px {

	// forward declarations
	template <typename T> class qtree;

	class transform_component
		: public component
		, public link_dispatcher<transform_component>
	{
	public:
		point2 const& position() const noexcept;
		point2 const& last() const noexcept;
		void store();

		void incarnate(qtree<transform_component*> * world);
		qtree<transform_component*> * world() const noexcept;

		void move(point2 const& direction);	// move by offset
		void place(point2 destination);		// place into specified location

		// space is not serizlized
		template <typename Archive>
		void serialize(Archive & archive)
		{
			archive(current, prev);
		}

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
		qtree<transform_component*> *	space;
		point2							current;
		point2							prev;
	};
}