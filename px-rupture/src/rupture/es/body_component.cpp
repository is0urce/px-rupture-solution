// name: body_component.cpp
// type: c++
// auth: is0urce
// type: class implementation

#include "body_component.hpp"
#include "container_component.hpp"

#include "../environment.hpp"
#include "../rl/mechanic.hpp"

namespace px {

    // ctor & dtor

    body_component::~body_component() = default;

    body_component::body_component() noexcept
        : button(nullptr)
        , exp(0)
        , lvl(0) {
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

    std::uint32_t body_component::mod_level(std::uint32_t mod) noexcept {
        return lvl += mod;
    }

    body_component::doll_type &	body_component::get_mannequin() noexcept {
        return mannequin;
    }

    std::vector<body_component::buff_type> const& body_component::get_buffs() const noexcept {
        return buffs;
    }

    void body_component::use_potion(rl::item const& potion, environment * context) {
        buff_type buff;

        // hp restore
        if (potion.has_effect(rl::effect::hp_bonus)) {
            auto efx = potion.accumulate({ rl::effect::hp_bonus });
            auto done = mechanic::heal(*this, static_cast<int>(efx.magnitude0), static_cast<rl::damage_type>(efx.sub));
            context->popup("+ " + std::to_string(done), { 0.0, 1.0, 0.0 });
        }

        // mp restore
        if (potion.has_effect(rl::effect::mp_bonus)) {
            auto efx = potion.accumulate({ rl::effect::mp_bonus });
            auto done = mechanic::innervate(*this, static_cast<int>(efx.magnitude0), static_cast<rl::damage_type>(efx.sub));
            context->popup("+ " + std::to_string(done), { 0.0, 0.0, 1.0 });
        }

        // hp regen
        if (potion.has_effect(rl::effect::hp_regen)) {
            auto efx = potion.accumulate({ rl::effect::hp_regen });
            buff.add(body_component::buff_type::enhancement_type::real(rl::effect::hp_regen, 0x00, efx.magnitude0));

            // notify
            context->popup("+ regeneration", { 0.0, 1.0, 0.0 });
        }

        // mp regen
        if (potion.has_effect(rl::effect::mp_regen)) {
            auto efx = potion.accumulate({ rl::effect::mp_regen });
            buff.add(body_component::buff_type::enhancement_type::real(rl::effect::mp_regen, 0x00, efx.magnitude0));

            // notify
            context->popup("+ invigoration", { 0.0, 1.0, 0.0 });
        }

        // accuracy
        if (potion.has_effect(rl::effect::accuracy)) {
            auto efx = potion.accumulate({ rl::effect::accuracy });
            buff.add(body_component::buff_type::enhancement_type::real(rl::effect::accuracy, 0x00, efx.magnitude0));

            // notify
            context->popup("+ accuracy", { 0.0, 1.0, 0.0 });
        }
        // armor
        if (potion.has_effect(rl::effect::armor)) {
            auto efx = potion.accumulate({ rl::effect::armor });
            buff.add(body_component::buff_type::enhancement_type::real(rl::effect::armor, 0x00, efx.magnitude0));

            // notify
            context->popup("+ armor", { 0.0, 1.0, 0.0 });
        }
        // dodge
        if (potion.has_effect(rl::effect::dodge)) {
            auto efx = potion.accumulate({ rl::effect::dodge });
            buff.add(body_component::buff_type::enhancement_type::real(rl::effect::dodge, 0x00, efx.magnitude0));

            // notify
            context->popup("+ dodge", { 0.0, 1.0, 0.0 });
        }

        if (buff.size() != 0) {
            buff.set_name(potion.name());
            buff.set_tag("b_" + potion.tag());
            buff.set_duration(5);
            add(buff);
        }
    }
}