// name: visual.hpp
// type: c++
// auth: is0urce
// desc: struct

#pragma once

#include <px/common/point.hpp>
#include <px/memory/uq_ptr.hpp>

namespace px {

    class animator_component;
    class sprite_component;
    class light_component;
    class transform_component;

    struct visual {
        point2                      start;
        point2                      finish;

        uq_ptr<transform_component> pawn;
        uq_ptr<sprite_component>    sprite;
        uq_ptr<animator_component>  animator;
        uq_ptr<light_component>     light;
        transform_component const*  track;
    };
}