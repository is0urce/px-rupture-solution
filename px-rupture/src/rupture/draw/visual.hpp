// name: visual.hpp
// type: c++
// auth: is0urce
// desc: struct

#pragma once

#include <px/common/point.hpp>
#include <px/memory/uq_ptr.hpp>

namespace px {

    class transform_component;
    class animator_component;
    class sprite_component;

    struct visual {
        point2                      start;
        point2                      finish;

        uq_ptr<transform_component> pawn;
        uq_ptr<sprite_component>    sprite;
        uq_ptr<animator_component>  animator;
        transform_component const*  track;
    };
}