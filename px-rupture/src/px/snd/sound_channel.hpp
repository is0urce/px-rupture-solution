// name: ai_state.hpp
// type: c++
// auth: is0urce
// type: enum

#pragma once

namespace px {

    enum class sound_channel : unsigned int {
        unspecified = 0,
        master = 1,
        music = 2,
        sfx = 3
    };
}