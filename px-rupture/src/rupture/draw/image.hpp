// name: image.hpp
// type: c++ struce
// auth: is0urce
// desc: pod struct 

#pragma once

// texture cooordinates of an image

namespace px {

    struct image {
        float sx_texture;
        float sy_texture;
        float dx_texture;
        float dy_texture;
    };
}