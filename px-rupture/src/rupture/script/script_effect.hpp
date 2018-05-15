// name: script_effect.hpp
// type: c++
// auth: is0urce
// desc: class for script binding

#pragma once

#include "../es/body_component.hpp"

namespace px {

    class script_unit;
    class script_environment;

    class script_effect final {
        friend class script_unit;
        friend class script_environment;

    public:
        void set_name(std::string const& text) {
            buff.set_name(text);
        }

        void set_duration(unsigned int time) {
            buff.set_duration(time);
        }

        void set_hidden(bool flag) {
            buff.set_hidden(flag);
        }

        // add effect by id
        void add(unsigned int efect_id, float value) {
            add_effect(static_cast<rl::effect>(efect_id), value);
        }

        void add_dot(float dmg) {
            add_effect(rl::effect::dot, dmg);
        }

        void add_resist(float value) {
            add_effect(rl::effect::resistance, value);
        }

    protected:
        script_effect() noexcept {
        }

    private:
        void add_effect(rl::effect fx, float value) {
            buff.add(body_component::buff_type::enhancement_type::real(fx, 0, value));
        }

    private:
        body_component::buff_type buff;
    };

}