// name: render_system.cpp
// type: c++
// auth: is0urce
// desc: class implementation

#include "render_system.hpp"

#include "../draw/renderer.hpp"

#include <px/memory/memory.hpp>

namespace px {

    // constructor & destructor

    render_system::~render_system() = default;

    render_system::render_system(unsigned int width, unsigned int height)
        : works(make_uq<renderer>(width, height)) {
    }

    // virtual overrides

    void render_system::update_system(delta_type const& delta_time) {
        works->run(delta_time.turn_duration);
    }

    // methods

    void render_system::resize(unsigned int width, unsigned int height) {
        render_system::works->resize(width, height);
    }

    void render_system::assign_sprite_data(std::vector<std::vector<sprite_vertex>> const* data) noexcept {
        works->assign_sprite_data(data);
    }

    void render_system::assign_lightmap_data(lightmap_data const* current, lightmap_data const* last) noexcept {
        works->assign_lightmap_data(current, last);
    }

    void render_system::assigm_message_data(message_data const* data) noexcept {
        works->assign_message_data(data);
    }

    unsigned int render_system::add_texture(unsigned int texture_width, unsigned int texture_height, void const* data) {
        return works->add_texture(texture_width, texture_height, data);
    }

    void render_system::zoom(bool is_up) {
        works->zoom(is_up);
    }

    float render_system::get_scale() const noexcept {
        return works->get_scale();
    }

    unsigned int render_system::width() const noexcept {
        return works->width();
    }

    unsigned int render_system::height() const noexcept {
        return works->height();
    }
}