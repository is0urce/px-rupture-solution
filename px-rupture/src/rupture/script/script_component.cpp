// name: script_component.cpp

#include "script_component.hpp"

#include "../environment.hpp"
#include "../es/body_component.hpp"

#include "script_system.hpp"

namespace px {

    script_component::~script_component() = default;

    // ctors

    script_component::script_component(script_system * script_machine, std::string const& name, std::uint32_t distance)
        : machine(script_machine)
        , script_name(name)
        , activation_distance(distance) {
    }

    script_component::script_component(script_system * script_machine, std::string const& name)
        : script_component(script_machine, name, 1) {
    }

    script_component::script_component(script_system * script_machine)
        : script_component(script_machine, "", 1) {
    }

    script_component::script_component()
        : script_component(nullptr, "", 1) {
    }

    // overloads

    bool script_component::can_use_useable(body_component * /*user*/, environment * /*environment*/) const {
        return true;
    }

    void script_component::use_useable(body_component * /*user*/, environment * /*environment*/) {
        if (machine) {
            machine->execute(script_name);
        }
    }

    // static functions

    std::string const& script_component::name() const noexcept {
        return script_name;
    }

    std::uint32_t script_component::distance() const noexcept {
        return activation_distance;
    }

    script_system const* script_component::get_machine() const noexcept {
        return machine;
    }

    void script_component::set_script_name(std::string const& new_script_name) {
        script_name = new_script_name;
    }
}