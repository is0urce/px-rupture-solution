// name: body_component.hpp

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
		, public rl::constitution<int32_t>
		, public rl::standing<int32_t>
	{
		friend class												body_works;
	public:
		typedef useable<body_component*, environment*>				useable_type;
		typedef rl::mass<rl::traverse>								mass_type;
		typedef rl::traverse_options<rl::traverse>					movement_type;
		typedef rl::standing<int32_t>								standing_type;
		typedef rl::buff<rl::effect, std::int32_t, double>			buff_type;
		typedef rl::enhancement<rl::effect, std::int32_t, double>	enhancement_type;

	public:
		void														assign_useable(useable_type * useable) noexcept;
		bool														is_useable() const noexcept;
		bool														can_use(body_component * user, environment * env) const;
		void														use(body_component * user, environment * env);
		bool														try_use(body_component * user, environment * env);
		mass_type const&											blocking() const noexcept;
		mass_type &													blocking() noexcept;
		movement_type const&										movement() const noexcept;
		movement_type &												movement() noexcept;
		void														equip(size_t idx);
		void														unequip(rl::equipment slot);
		rl::item *													equipment(rl::equipment slot) const;
		enhancement_type											accumulate(enhancement_type start) const;
		std::uint32_t												level() const;
		std::uint32_t												experience() const;
		void														set_level(std::uint32_t n);
		void														set_experience(std::uint32_t n);
		

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