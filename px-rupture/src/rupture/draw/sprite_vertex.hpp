// name: sprite_vertex.hpp
// type: c++ struct

#pragma once

#include "glm_options.hpp"

namespace px {

    struct sprite_vertex {
        glm::vec2 position;
        glm::vec2 texture;
    };
}