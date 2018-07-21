// name: script.cpp
// type: c++ code
// auth: is0urce
// desc: class methods implementation

#include "script.hpp"

#include "script_internal.hpp"

#include <px/memory/memory.hpp>

namespace px {

    namespace {
        static const float gc_wait = 2.5f;
        static const float gc_repeat = 10.0f;
    }

    // constructor & destructor

    script::~script() = default;

    script::script()
        : works(make_uq<script_internal>())
        , garbage_timer(gc_wait) {
    }

    // methods

    void script::assign_environment(environment * game) {
        if (works) {
            works->assign_environment(game);
        }
    }

    void script::execute(std::string const& name) {
        if (works) {
            works->execute(name);
        }
    }

    void script::run(std::string const& code) {
        if (works) {
            works->run(code);
        }
    }

    rl::skill_functional<body_component *, body_component *, point2> script::impact(std::string const& name) {
        return works->impact(name);
    }

    void script::turn_update_system(delta_type const& /* delta_time */) {
        garbage_timer = gc_wait;
    }

    void script::update_system(delta_type const& delta_time) {
        if (garbage_timer < delta_time.turn_duration) {
            garbage_timer = delta_time.turn_duration + gc_repeat;
            if (works) {
                works->collect_garbage();
            }
        }
    }
}