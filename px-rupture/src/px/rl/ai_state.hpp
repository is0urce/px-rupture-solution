// name: ai_state.hpp
// type: c++
// auth: is0urce
// type: enum

#pragma once

namespace px::rl {
    enum class ai_state : unsigned char {
        idle = 0,
        alert = 1
    };
}