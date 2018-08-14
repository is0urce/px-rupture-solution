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

#include <px/es/factory_line.hpp>

namespace px {

    class builder;

    class core
        : public factory_dispatcher<core>
        , public factory_line<animator_component, animator_system>
        , public factory_line<body_component, body_system>
        , public factory_line<character_component, character_system>
        , public factory_line<light_component, light_system>
        , public factory_line<npc_component, npc_system>
        , public factory_line<script_component, script_system>
        , public factory_line<sprite_component, sprite_system>
        , public factory_line<sound_component, sound_system>
        , public factory_line<transform_component, transform_system>
    {
    public:
        virtual ~core();
        core();
    };
}