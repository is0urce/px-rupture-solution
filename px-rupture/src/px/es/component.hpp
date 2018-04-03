// name: component.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#pragma once

// base class for components in entity / component system

namespace px {

    class component abstract {
    public:
        bool is_active() const noexcept {
            return active;
        }

        void activate() {
            if (!active) {
                activate_component();
                active = true;
            }
        }

        void deactivate() {
            if (active) {
                deactivate_component();
                active = false;
            }
        }

    public:
        virtual ~component() = default;

        component() noexcept
            : active(false)
        {
        }

    protected:
        virtual void activate_component() {
        }

        virtual void deactivate_component() {
        }

    private:
        bool active; // present on scene
    };
}