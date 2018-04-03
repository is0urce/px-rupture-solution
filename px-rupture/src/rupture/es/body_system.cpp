// name: body_system.cpp
// type: c++
// auth: is0urce
// desc: class implementation

#include "body_system.hpp"
#include "body_works.hpp"

#include <px/memory/memory.hpp>

namespace px {

    // constructor & destructor

    body_system::~body_system() = default;

    body_system::body_system()
        : works(make_uq<body_works>())
    {
    }

    // virtual overrides

    void body_system::turn_update_system(delta_type const& /*delta_time*/) {
        works->tick();
    }

    // methods

    void body_system::assign_environment(environment * game) {
        works->assign_environment(game);
    }

    uq_ptr<body_component> body_system::make() {
        return works->make();
    }
}