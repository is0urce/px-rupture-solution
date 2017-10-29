// name: body_component.hpp

#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>

#include <px/es/useable.hpp>

#include <px/rl/constitution.hpp>
#include <px/rl/entity.hpp>
#include <px/rl/standing.hpp>
#include <px/rl/mass.hpp>
#include <px/rl/traverse.hpp>

#include <cstdint>

namespace px {

	class transform_component;
	class character_component;
	class container_component;
	class composite_component;

	class environment;

	class body_component
		: public component
		, public link<transform_component>
		, public link<composite_component>
		, public link<character_component>
		, public link<container_component>
		, public link_dispatcher<body_component>
		, public rl::entity
		, public rl::constitution<int32_t>
		, public rl::standing<int32_t>
	{
	public:
		typedef useable<body_component*, environment*>	useable_type;
		typedef rl::mass<rl::traverse>					mass_type;
		typedef rl::traverse_options<rl::traverse>		movement_type;
		typedef rl::standing<int32_t>					standing_type;

	public:
		void											assign_useable(useable_type * useable) noexcept;
		bool											is_useable() const noexcept;
		bool											can_use(body_component * user, environment * env) const;
		void											use(body_component * user, environment * env);
		bool											try_use(body_component * user, environment * env);
		mass_type const&								blocking() const noexcept;
		mass_type &										blocking() noexcept;
		movement_type const&							movement() const noexcept;
		movement_type &									movement() noexcept;

		template <typename Archive>
		void serialize(Archive & archive) {
			archive(mass, traverse_opts);
			archive(static_cast<rl::entity &>(*this));
			archive(static_cast<rl::constitution<int32_t> &>(*this));
			archive(static_cast<rl::standing<int32_t> &>(*this));
		}

	public:
		virtual ~body_component();
		body_component() noexcept;
		body_component(body_component const&) noexcept = delete;
		body_component & operator=(body_component const&) noexcept = delete;

	private:
		mass_type										mass;
		movement_type									traverse_opts;
		useable_type *									button;
	};
}