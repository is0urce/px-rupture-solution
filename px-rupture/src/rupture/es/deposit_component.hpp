// name: deposit_component.hpp
// type: c++ header
// auth: is0urce
// desc: class declaration

#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>
#include <px/es/useable.hpp>

#include <cstdint>

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
		void			set_dissolve(bool should_destroy) noexcept;
		bool			dissolving() const noexcept;

		template <typename Archive>
		void			serialize(Archive & archive) {
			archive(dissolve, use_duration);
		}

	public:
		virtual ~deposite_component();
		deposite_component();
		deposite_component(deposite_component const&) = delete;
		deposite_component & operator=(deposite_component const&) = delete;

	protected:
		virtual bool	can_use_useable(body_component * user, environment * environment) const override;
		virtual void	use_useable(body_component * user, environment * environment) override;

	private:
		std::uint32_t	use_duration;
		bool			dissolve;
	};
}