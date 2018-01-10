// name: body_component.hpp
// type: c++ header
// auth: is0urce
// desc: class declaration

#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>

#include <px/es/useable.hpp>

#include <px/rl/buff.hpp>
#include <px/rl/constitution.hpp>
#include <px/rl/doll.hpp>
#include <px/rl/entity.hpp>
#include <px/rl/equipment.hpp>
#include <px/rl/standing.hpp>
#include <px/rl/mass.hpp>
#include <px/rl/traverse.hpp>
#include "rupture/rl/item.hpp"

#include <cstdint>

namespace px {

	class transform_component;
	class character_component;
	class container_component;
	class composite_component;

	class environment;

	class body_works;

	class body_component
		: public component
		, public link<transform_component>
		, public link<composite_component>
		, public link<character_component>
		, public link<container_component>
		, public link_dispatcher<body_component>
		, public rl::entity
		, public rl::constitution<std::int32_t>
		, public rl::standing<std::int32_t>
	{
		friend class												body_works;
	public:
		using useable_type = useable<body_component*, environment*>;
		using mass_type = rl::mass<rl::traverse>;
		using movement_type = rl::traverse_options<rl::traverse>;
		using standing_type = rl::standing<std::int32_t>;
		using buff_type = rl::buff<rl::effect, std::int32_t, double>;
		using enhancement_type = rl::enhancement<rl::effect, std::int32_t, double>;

	public:
		void														add(buff_type affect);
		enhancement_type											accumulate(enhancement_type start) const;
		void														assign_useable(useable_type * useable) noexcept;
		mass_type const&											blocking() const noexcept;
		mass_type &													blocking() noexcept;
		bool														can_use(body_component * user, environment * env) const;
		void														equip(size_t idx);
		rl::item *													equipment(rl::equipment slot) const;
		std::uint32_t												experience() const;
		bool														is_useable() const noexcept;
		std::uint32_t												level() const;
		movement_type const&										movement() const noexcept;
		movement_type &												movement() noexcept;
		void														set_level(std::uint32_t n);
		void														set_experience(std::uint32_t n);
		bool														try_use(body_component * user, environment * env);

		void														unequip(rl::equipment slot);
		void														use(body_component * user, environment * env);

		template <typename Archive>
		void														serialize(Archive & archive) {
			archive(mass, traverse_opts);
			archive(static_cast<rl::entity &>(*this));
			archive(static_cast<rl::constitution<int32_t> &>(*this));
			archive(static_cast<rl::standing<int32_t> &>(*this));
			archive(mannequin, buffs);
			archive(lvl, exp);
		}

	public:
		virtual														~body_component();
		body_component() noexcept;
		body_component(body_component const&) noexcept = delete;
		body_component & operator=(body_component const&) noexcept = delete;

	private:
		mass_type													mass;
		movement_type												traverse_opts;
		useable_type *												button;
		rl::doll<rl::equipment, rl::item>							mannequin;
		std::vector<buff_type>										buffs;
		std::uint32_t												lvl;
		std::uint32_t												exp;
	};
}