// name: door_component.hpp

#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>
#include <px/es/useable.hpp>

#include <cstdint>

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
		bool set_open(bool flag);
		bool open();
		bool close();
		bool is_opened() const noexcept;
		unsigned char use_duration() const noexcept;

		template <typename Archive>
		void serialize(Archive & archive) {
			archive(opened, open_duration);
		}

	public:
		virtual ~door_component();
		door_component();
		door_component(door_component const&) = delete;
		door_component & operator=(door_component const&) = delete;

	protected:
		virtual bool can_use_useable(body_component *, environment *) const override;
		virtual void use_useable(body_component *, environment *) override;

	private:
		unsigned char	open_duration;
		bool			opened;
	};
}