// name: npc_system.cpp
// type: c++
// auth: is0urce
// desc: class implementation

#include "npc_system.hpp"

#include "npc_works.hpp"

#include <px/memory/memory.hpp>

namespace px {

    // constructor & destructor

    npc_system::~npc_system() = default;

    npc_system::npc_system()
        : works(make_uq<npc_works>())
    {
    }

    // virtual overrides

    void npc_system::turn_update_system(delta_type const& /*delta_time*/) {
        works->turn();
    }

    // methods

    void npc_system::assign_scene(scene * world) noexcept {
        works->assign_scene(world);
    }

    uq_ptr<npc_component> npc_system::make() {
        return works->make();
    }
}