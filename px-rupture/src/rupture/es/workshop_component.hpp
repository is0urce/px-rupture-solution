// name: workshop_component.hpp

#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>
#include <px/es/useable.hpp>

#include <px/rl/workshop.hpp>

namespace px {

	class body_component;
	class environment;

	class workshop_component final
		: public component
		, public link_dispatcher<workshop_component>
		, public useable<body_component *, environment *>
	{
	public:
		rl::workshop variant() const noexcept;
		void set_variant(rl::workshop variant);

		template <typename Archive>
		void serialize(Archive & archive) {
			archive(station);
		}

	public:
		virtual ~workshop_component();
		workshop_component();
		workshop_component(workshop_component const&) = delete;
		workshop_component & operator=(workshop_component const&) = delete;

	protected:
		virtual bool can_use_useable(body_component *, environment *) const override;
		virtual void use_useable(body_component *, environment *) override;

	private:
		rl::workshop station;
	};
}