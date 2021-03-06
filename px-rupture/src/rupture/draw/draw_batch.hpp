// name: draw_batch.hpp
// type: c++
// auth: is0urce
// desc: structure

#pragma once

#include <px/rgl/rgl.hpp>
#include <px/rgl/pass.hpp>

namespace px {

    struct draw_batch {
    public:
        template <typename T>
        void draw_arrays(GLenum mode, GLenum usage, size_t size, T const* data) {
            if (size != 0) {
                vertices.load(usage, sizeof(T) * size, data);
                pass.draw_arrays(mode, static_cast<GLsizei>(size));
            }
        }

        template <typename C>
        void draw_arrays(GLenum mode, GLenum usage, C const& container) {
            if (container.size() != 0) {
                vertices.load_array(usage, container);
                pass.draw_arrays(mode, static_cast<GLsizei>(container.size()));
            }
        }

        gl_buffer   vertices;
        gl_vao      geometry;
        gl_texture  texture;
        pass        pass;
    };
}