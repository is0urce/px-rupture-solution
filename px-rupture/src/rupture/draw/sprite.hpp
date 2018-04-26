// name: sprite.hpp

#include "image.hpp"

#pragma once

namespace px {

    struct sprite
        : public image
    {
        float           x_transpose;
        float           y_transpose;
        float           x_multiple;
        float           y_multiple;

        unsigned int    texture_index;
        const char*     name;
    };
}