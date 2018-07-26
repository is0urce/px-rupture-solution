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

namespace px {

    class builder;

    class core {
        friend class builder;
    public:
        virtual             ~core();
        core();

    protected:
        script_system       mashine;    // script system

        animator_system     animators;
        body_system         bodies;
        character_system    characters;
        light_system        lights;
        npc_system          npcs;       // non player characters manager
        sound_system        sounds;
        sprite_system       sprites;
        transform_system    transforms;
    };
}