// name: core.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "es/animator_system.hpp"
#include "es/body_system.hpp"       // effects
#include "es/character_system.hpp"  // cooldowns
#include "es/light_system.hpp"      // lightmap
#include "es/npc_system.hpp"        // actions
#include "es/sprite_system.hpp"
#include "es/sound_system.hpp"
#include "es/transform_system.hpp"  // store position

#include "script/script_system.hpp"

#include <px/es/factory_dispatcher.hpp>

namespace px {

    class builder;

    class facility
        : public factory_dispatcher<facility>
        , public animator_system
        , public body_system
        , public character_system
        , public light_system
        , public npc_system
        , public script_system
        , public sprite_system
        , public sound_system
        , public transform_system
    {
    public:
        virtual ~facility();
        facility();
    };
}