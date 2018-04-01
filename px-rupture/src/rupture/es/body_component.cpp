// name: body_component.cpp
// type: c++
// auth: is0urce
// type: class implementation

#include "body_component.hpp"
#include "container_component.hpp"
#include "../environment.hpp"

namespace px {

    // ctor & dtor

    body_component::~body_component() = default;

    body_component::body_component() noexcept
        : button(nullptr)
        , exp(0)
        , lvl(0)
    {
    }

    // methods

    void body_component::add(buff_type affect) {
        buffs.push_back(affect);
    }

    void body_component::assign_useable(useable_type * useable) noexcept {
        button = useable;
    }

    rl::mass<rl::traverse> const& body_component::blocking() const noexcept {
        return mass;
    }

    rl::mass<rl::traverse> & body_component::blocking() noexcept {
        return mass;
    }

    body_component::movement_type const& body_component::movement() const noexcept {
        return traverse_opts;
    }

    body_component::movement_type &	body_component::movement() noexcept {
        return traverse_opts;
    }

    bool body_component::is_useable() const noexcept {
        return button != nullptr;
    }

    bool body_component::can_use(body_component * user, environment * env) const {
        return button && button->can_use(user, env);
    }

    void body_component::use(body_component * user, environment * env) {
        if (button) {
            button->use(user, env);
        }
    }

    bool body_component::try_use(body_component * user, environment * env) {
        return button && button->try_use(user, env);
    }

    bool body_component::equip(size_t idx) {
        if (auto cont = linked<container_component>()) {
            if (auto selected_item = cont->get(idx)) {
                auto enh = selected_item->accumulate({ rl::effect::equipment });
                auto slot = static_cast<rl::equipment>(enh.sub);

                if (slot == rl::equipment::not_valid) return false;

                if (auto item_ptr = cont->unacquire(idx, 1)) {
                    if (auto swap_ptr = mannequin.equip(slot, std::move(item_ptr))) {
                        cont->acquire(std::move(swap_ptr));
                    }
                    return true;
                }
            }
        }
        return false;
    }

    void body_component::unequip(rl::equipment slot) {
        if (auto cont = linked<container_component>()) {
            auto item_ptr = mannequin.remove(slot);
            if (item_ptr) {
                cont->acquire(std::move(item_ptr));
            }
        }
    }

    rl::item * body_component::equipment(rl::equipment slot) const {
        return mannequin[slot];
    }

    body_component::enhancement_type body_component::accumulate(enhancement_type accumulator) const {
        for (auto const& buff : buffs) {
            accumulator = buff.accumulate(accumulator);
        }
        return mannequin.accumulate(accumulator);
    }

    std::uint32_t body_component::level() const noexcept {
        return lvl;
    }

    std::uint32_t body_component::experience() const noexcept {
        return exp;
    }

    void body_component::set_level(std::uint32_t n) noexcept {
        lvl = n;
    }

    void body_component::set_experience(std::uint32_t n) noexcept {
        exp = n;
    }

    std::uint32_t body_component::mod_experience(std::uint32_t n) noexcept {
        return exp += n;
    }

    body_component::doll_type &	body_component::get_mannequin() {
        return mannequin;
    }

    std::vector<body_component::buff_type> const& body_component::get_buffs() const {
        return buffs;
    }

    void body_component::use_potion(rl::item const& potion, environment * context) {

        // hp restore
        if (potion.has_effect(rl::effect::hp_bonus)) {
            if (auto resource = health()) {
                auto bonus = static_cast<body_component::resource_value_type>(potion.accumulate({ rl::effect::hp_bonus }).magnitude0);
                resource->restore(bonus);

                // notify
                context->popup("+ " + std::to_string(bonus), { 0.0, 1.0, 0.0 });
            }
        }

        // mp restore
        if (potion.has_effect(rl::effect::mp_bonus)) {
            if (auto resource = energy()) {
                auto bonus = static_cast<body_component::resource_value_type>(potion.accumulate({ rl::effect::mp_bonus }).magnitude0);
                resource->restore(bonus);

                // notify
                context->popup("+ " + std::to_string(bonus), { 0.0, 0.0, 1.0 });
            }
        }

        // hp regen
        if (potion.has_effect(rl::effect::hp_regen)) {
            auto effect = potion.accumulate({ rl::effect::hp_regen });
            body_component::buff_type regen;
            regen.set_name("regeneration");
            regen.set_tag("b_hp_regen");
            regen.set_duration(effect.value0);
            regen.add(body_component::buff_type::enhancement_type::real(rl::effect::hp_regen, 0, effect.magnitude0));
            add(regen);

            // notify
            context->popup("+ " + regen.name(), { 0.0, 1.0, 0.0 });
        }

        // mp regen
        if (potion.has_effect(rl::effect::mp_regen)) {
            auto effect = potion.accumulate({ rl::effect::mp_regen });
            body_component::buff_type regen;
            regen.set_name("invigoration");
            regen.set_tag("b_mp_regen");
            regen.set_duration(effect.value0);
            regen.add(body_component::buff_type::enhancement_type::real(rl::effect::mp_regen, 0, effect.magnitude0));
            add(regen);

            // notify
            context->popup("+ " + regen.name(), { 0.0, 1.0, 0.0 });
        }
    }
}