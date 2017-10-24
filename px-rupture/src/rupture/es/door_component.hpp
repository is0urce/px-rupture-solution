// name: door_component.hpp

#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>
#include <px/es/useable.hpp>

namespace px {

	class animator_component;
	class body_component;
	class environment;

	class door_component
		: public component
		, public link<animator_component>
		, public link<body_component>
		, public link_dispatcher<door_component>
		, public useable<body_component *, environment *>
	{
	public:
		void open()
		{
			//if (!opened)
		}

	public:
		virtual ~door_component() = default;
		door_component()
			: opened(false)
		{
		}
		door_component(door_component const&) = delete;
		door_component & operator=(door_component const&) = delete;

	protected:
		virtual bool can_use_useable(body_component *, environment *) const override
		{
			return !opened;
		}
		virtual void use_useable(body_component *, environment *) override
		{
			opened = true;
		}

	private:
		bool opened;
	};
}