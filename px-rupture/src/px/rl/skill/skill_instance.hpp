// name: skill_instance.hpp
// type: c++ header
// auth: is0urce
// desc: class declaration

#pragma once

// base for instanced abilities with a state
// proxy to base skill functional

#include <px/rl/skill/skill_impact.hpp>

namespace px::rl {

    template <typename State, typename User, typename Target, typename Area>
    class skill_instance
        : public skill_impact<User, Target, Area>
    {
    public:
        using state_type = State;
        using impact_type = skill_impact<User, Target, Area>;

    public:
        void assign_impact(impact_type * base_impact) {
            m_base = base_impact;
        }

        state_type & state() noexcept {
            return m_state;
        }

        state_type const& state() const noexcept {
            return m_state;
        }

        template <typename Archive>
        void serialize(Archive & archive) {
            archive(m_state);
        }

    public:
        virtual ~skill_instance() override = default;

        skill_instance()
            : m_base(nullptr) {
        }

        skill_instance(state_type start_state, impact_type * impact)
            : m_base(impact)
            , m_state(start_state) {
        }

    protected:
        virtual bool useable_instance() const {
            return true;
        }

        virtual void use_instance() {
        }

        // override

        virtual bool is_targeted_skill() const override {
            return m_base && m_base->is_targeted();
        }

        virtual void use_skill(user_type user, target_type target) override {
            if (m_base) {
                m_base->use(user, target);
                use_instance();
            }
        }

        virtual void use_skill(user_type user, area_type area) override {
            if (m_base) {
                m_base->use(user, area);
                use_instance();
            }
        }

        virtual bool useable_skill(user_type user, target_type target) const override {
            return useable_instance() && m_base && m_base->useable(user, target);
        }

        virtual bool useable_skill(user_type user, area_type area) const override {
            return useable_instance() && m_base && m_base->useable(user, area);
        }

    private:
        state_type      m_state;
        impact_type *   m_base;
    };
}