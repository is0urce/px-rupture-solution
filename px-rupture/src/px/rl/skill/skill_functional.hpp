// name: skill_functional.hpp
// type: c++ header
// desc: class
// auth: is0urce

#pragma once

#include "skill_impact.hpp"

#include <functional>

namespace px::rl {

    template <typename User, typename Target, typename Area>
    class skill_functional
        : public skill_impact<User, Target, Area>
    {
    public:
        using target_fn = std::function<void(user_type, target_type)>;
        using target_predicate = std::function<bool(user_type, target_type)>;
        using area_fn = std::function<void(user_type, area_type)>;
        using area_predicate = std::function<bool(user_type, area_type)>;
        enum class v_targeted { mark };
        enum class v_area { mark };

    public:
        virtual ~skill_functional() override = default;

        skill_functional()
            : m_targeted(false) {
        }

        skill_functional(bool targeted, target_fn tf, target_predicate tfc, area_fn gf, area_predicate gfc)
            : m_targeted(targeted)
            , m_target(tf)
            , m_target_check(tfc)
            , m_ground(gf)
            , m_ground_check(gfc) {
        }

        skill_functional(v_targeted /*mark*/, target_fn tf, target_predicate tfc)
            : m_targeted(true)
            , m_target(tf)
            , m_target_check(tfc) {
        }

        skill_functional(v_area /*mark*/, area_fn gf, area_predicate gfc)
            : m_targeted(false)
            , m_ground(gf)
            , m_ground_check(gfc) {
        }

    protected:
        virtual bool is_targeted_skill() const override {
            return m_targeted;
        }

        virtual void use_skill(user_type user, target_type target) override {
            if (m_target) {
                m_target(user, target);
            }
        }

        virtual void use_skill(user_type user, area_type area) override {
            if (m_ground) {
                m_ground(user, area);
            }
        }

        virtual bool useable_skill(user_type user, target_type target) const override {
            return m_target_check && m_target_check(user, target);
        }

        virtual bool useable_skill(user_type user, area_type area) const override {
            return m_ground_check && m_ground_check(user, area);
        }

    private:
        bool                m_targeted;
        target_fn           m_target;
        target_predicate    m_target_check;
        area_fn             m_ground;
        area_predicate      m_ground_check;
    };
}