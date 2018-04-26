// name: gl_uniform.hpp
// type: c++ header
// auth: is0urce
// desc: class

#pragma once

// uniform block

#include "gl_buffer.hpp"

namespace px {

    struct gl_uniform {
    public:
        operator GLuint() const noexcept {
            return block;
        }

        template <typename Structure>
        void load(GLenum mode, Structure && data) {
            block.load(mode, data);
        }
    public:
        gl_uniform() {
            block = { GL_UNIFORM_BUFFER };
        }

    private:
        gl_buffer block;
    };
}