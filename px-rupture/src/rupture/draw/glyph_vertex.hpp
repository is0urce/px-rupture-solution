// name: glyph_vertex.hpp
// type: c++ struct

#pragma once

#include "glm_options.hpp"

namespace px {

    struct glyph_vertex {
        glm::vec2 position;
        glm::vec2 texture;
        glm::vec4 tint;
    };
}