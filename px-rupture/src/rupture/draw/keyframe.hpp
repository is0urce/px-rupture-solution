// name: keyframe.hpp
// type: c++ struct
// auth: is0urce
// desc: pod struct for animation keyframes

#pragma once

namespace px {

    struct sprite;

    struct keyframe {
        sprite const*   frame;  // what
        double          time;   // when
    };
}