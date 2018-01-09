// name: transform_component.hpp
// type: c++ header
// auth: is0urce
// desc: class declaration

#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>

#include <px/es/transform.hpp>

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
		void							make_static();
		void							make_dynamic();
		void							set_static(bool is_static);
		bool							is_static() const noexcept;

	public:
		virtual ~transform_component();
		transform_component() noexcept;
		transform_component(point2 position) noexcept;
		transform_component(transform_component const&) noexcept = delete;
		transform_component & operator=(transform_component const&) noexcept = delete;

		template <typename Archive>
		void serialize(Archive & archive) {
			archive(current, prev, background);
		}

	protected:
		virtual void					activate_component() override;
		virtual void					deactivate_component() override;

	private:
		void							insert();
		void							retract();
		void							project(point2 destination);
		void							swap(transform_component & that);

	private:
		qtree<transform_component*> *	space;											// space partitioning it's in
		bool							background;										// is static
	};
}