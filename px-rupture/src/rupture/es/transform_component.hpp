// name: transform_component.hpp

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>

#include <px/es/transform.hpp>

#pragma once

namespace px {

	// forward declarations
	template <typename T> class qtree;
	class body_component;

	class transform_component final
		: public component
		, public link<body_component>
		, public link_dispatcher<transform_component>
		, public transform
	{
	public:
		void							incarnate(qtree<transform_component*> * world);	// set space
		void							move(point2 const& direction);					// move by offset
		void							place(point2 destination);						// place into specified location
		qtree<transform_component*> *	world() const noexcept;							// get current space
		void							swap(transform_component & that);

	public:
		virtual ~transform_component();
		transform_component() noexcept;
		transform_component(point2 position) noexcept;
		transform_component(transform_component const&) noexcept = delete;
		transform_component & operator=(transform_component const&) noexcept = delete;
		transform_component (transform_component && that) noexcept;
		transform_component & operator=(transform_component && that) noexcept;

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