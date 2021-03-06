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
        friend class                                                body_works;

    public:
        using useable_type = useable<body_component*, environment*>;
        using mass_type = rl::mass<rl::traverse>;
        using movement_type = rl::traverse_options<rl::traverse>;
        using standing_type = rl::standing<std::int32_t>;
        using buff_type = rl::buff<rl::effect, std::int32_t, double>;
        using enhancement_type = rl::enhancement<rl::effect, std::int32_t, double>;
        using resource_value_type = resource_type::value_type::value_type;
        using doll_type = rl::doll<rl::equipment, rl::item>;

    public:
        void                                                        add(buff_type affect);
        enhancement_type                                            accumulate(enhancement_type start) const;
        void                                                        assign_useable(useable_type * useable) noexcept;
        mass_type const&                                            blocking() const noexcept;
        mass_type &                                                 blocking() noexcept;
        bool                                                        can_use(body_component * user, environment * context) const;
        bool                                                        equip(size_t idx);
        rl::item *                                                  equipment(rl::equipment slot) const;
        std::uint32_t                                               experience() const noexcept;
        bool                                                        is_useable() const noexcept;
        std::uint32_t                                               level() const noexcept;
        movement_type const&                                        movement() const noexcept;
        movement_type &                                             movement() noexcept;
        void                                                        set_level(std::uint32_t lvl) noexcept;
        void                                                        set_experience(std::uint32_t exp) noexcept;
        std::uint32_t                                               mod_experience(std::uint32_t exp) noexcept;
        std::uint32_t                                               mod_level(std::uint32_t lvl) noexcept;
        bool                                                        try_use(body_component * user, environment * context);
        doll_type &                                                 get_mannequin() noexcept;
        std::vector<buff_type> const&                               get_buffs() const noexcept;
        void                                                        unequip(rl::equipment slot);
        void                                                        use(body_component * user, environment * env);
        void                                                        use_potion(rl::item const& potion, environment * context);

        template <typename Archive>
        void                                                        serialize(Archive & archive) {
            archive(mass, traverse_opts);
            archive(static_cast<rl::entity &>(*this));
            archive(static_cast<rl::constitution<int32_t> &>(*this));
            archive(static_cast<rl::standing<int32_t> &>(*this));
            archive(mannequin, buffs);
            archive(lvl, exp);
        }

    public:
        virtual                                                     ~body_component() override;
        body_component() noexcept;
        body_component(body_component const&) noexcept = delete;
        body_component & operator=(body_component const&) noexcept = delete;

    private:
        mass_type                                                   mass;
        movement_type                                               traverse_opts;
        useable_type *                                              button;
        doll_type                                                   mannequin;
        std::vector<buff_type>                                      buffs;
        std::uint32_t                                               lvl;
        std::uint32_t                                               exp;
    };
}