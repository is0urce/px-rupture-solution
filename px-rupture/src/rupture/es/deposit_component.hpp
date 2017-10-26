// name: deposite_component.hpp

#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>
#include <px/es/useable.hpp>

namespace px {

	class body_component;
	class container_component;
	class composite_component;
	class environment;

	class deposite_component
		: public component
		, public link<container_component>
		, public link<composite_component>
		, public link_dispatcher<deposite_component>
		, public useable<body_component *, environment *>
	{
	public:
		void set_dissolve(bool should_destroy) noexcept;
		bool dissolving() const noexcept;

		template <typename Archive>
		void serialize(Archive & archive) {
			archive(dissolve);
		}

	public:
		virtual ~deposite_component();
		deposite_component();
		deposite_component(deposite_component const&) = delete;
		deposite_component & operator=(deposite_component const&) = delete;

	protected:
		virtual bool can_use_useable(body_component * user, environment * environment) const override;
		virtual void use_useable(body_component * user, environment * environment) override;

	private:
		bool dissolve;
	};
}