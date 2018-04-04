// name: skill_base.hpp
// type: c++ header
// auth: is0urce
// desc: class declaration

#pragma once

#include "skill_state.hpp"

#include <px/rl/skill/skill_instance.hpp>
#include <px/common/point.hpp>

#include <cstdint>

namespace px {

    class body_component;

    class skill
        : public rl::skill_instance<skill_state<std::int32_t>, body_component *, body_component *, point2>
    {
    public:
        virtual ~skill() override = default;
        skill() = default;

    protected:
        virtual bool useable_instance() const override {
            return !state().on_cooldown();
        }

        virtual void use_instance() override {
            state().start_cooldown();
        }
    };
}