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

namespace px {

	class transform_component;

	class environment;

	class body_component
		: public component
		, public link_dispatcher<body_component>
		, public rl::entity
		, public rl::constitution
		, public rl::standing<unsigned int>
	{
	public:
		typedef useable<body_component*, environment*> useable_type;
		typedef rl::mass<rl::traverse> mass_type;
		typedef rl::traverse_options<rl::traverse> movement_type;

	public:
		void										assign_useable(useable_type * useable) noexcept;
		bool										is_useable() const noexcept;
		bool										can_use(body_component * user, environment * env) const;
		void										use(body_component * user, environment * env);
		bool										try_use(body_component * user, environment * env);
		mass_type const&							blocking() const noexcept;
		mass_type &									blocking() noexcept;
		movement_type const&						movement() const noexcept;
		movement_type &								movement() noexcept;

	public:
		virtual ~body_component();
		body_component() noexcept;
		body_component(body_component const&) noexcept = delete;
		body_component & operator=(body_component const&) noexcept = delete;

	private:
		mass_type									mass;
		movement_type								traverse_opts;
		useable_type *								button;
	};
}