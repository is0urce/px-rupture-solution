// name: equipment.hpp
// type: c++
// auth: is0urce
// desc: equipment slot enum

#pragma once

namespace px::rl {

    enum class equipment : unsigned char {
        not_valid = 0,  // default is invalid

        hand,           // weapon

        head,
        chest,
        legs,

        teeth,          // intrinsic weapon
        hide,           // intrinsic armor

        max_value
    };
}