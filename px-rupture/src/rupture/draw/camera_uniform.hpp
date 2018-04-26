// name: canera_uniform.hpp
// type: c++ struct
// auth: is0urce
// desc: shader uniform data layout

#pragma once

#include "glm_options.hpp"

namespace px {

    struct camera_uniform {
        glm::vec2   scale;
        glm::vec2   translate;
        glm::vec2   screen;
        glm::vec2   lightmap_size;
        glm::vec2   lightmap_range;
        glm::vec2   lightmap_last_offset;
        glm::vec2   interpolate;
        glm::vec2   timer;
    };
}