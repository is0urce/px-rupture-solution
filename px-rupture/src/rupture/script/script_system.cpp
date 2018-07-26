// name: script_system.cpp
// type: c++ code
// auth: is0urce
// desc: class methods implementation

#include "script_system.hpp"

#include "script_component.hpp"

#include "script_internal.hpp"

#include <px/memory/memory.hpp>

namespace px {

    namespace {
        static const float gc_wait = 2.5f;
        static const float gc_repeat = 10.0f;
    }

    // constructor & destructor

    script_system::~script_system() = default;

    script_system::script_system()
        : works(make_uq<script_internal>())
        , garbage_timer(gc_wait) {
    }

    // methods

    void script_system::assign_environment(environment * game) {
        if (works) {
            works->assign_environment(game);
        }
    }

    uq_ptr<script_component> script_system::make(std::string const& script_name) {
        return make_uq<script_component>(this, script_name);
    }

    void script_system::execute(std::string const& script_name) {
        if (works) {
            works->execute(script_name);
        }
    }

    void script_system::run(std::string const& code) {
        if (works) {
            works->run(code);
        }
    }

    rl::skill_functional<body_component *, body_component *, point2> script_system::impact(std::string const& name) {
        return works->impact(name);
    }

    void script_system::turn_update_system(delta_type const& /* delta_time */) {
        garbage_timer = gc_wait;
    }

    void script_system::update_system(delta_type const& delta_time) {
        if (garbage_timer < delta_time.turn_duration && works) {
            garbage_timer = delta_time.turn_duration + gc_repeat;
            works->collect_garbage();
        }
    }
}