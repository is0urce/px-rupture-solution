// name: render_system.hpp
// type: c++
// auth: is0urce
// desc: class declaration

#pragma once

#include <px/es/system.hpp>
#include <px/es/delta.hpp>

#include <px/memory/uq_ptr.hpp>

#include <vector>

namespace px {

    class renderer;
    class lightmap_data;
    class message_data;

    struct sprite_vertex;

    class render_system final
        : public system<delta>
    {
    public:
        void                resize(unsigned int width, unsigned int height);
        void                assign_sprite_data(std::vector<std::vector<sprite_vertex>> const* data) noexcept;
        void                assign_lightmap_data(lightmap_data const* current, lightmap_data const* last) noexcept;
        void                assigm_message_data(message_data const* data) noexcept;
        void                add_texture(unsigned int texture_width, unsigned int texture_height, void const* data);
        void                zoom(bool up);
        float               get_scale() const noexcept;
        unsigned int        width() const noexcept;
        unsigned int        height() const noexcept;

    public:
        virtual             ~render_system() override;
        render_system(unsigned int width, unsigned int height);

    protected:
        virtual void        update_system(delta_type const& delta_time) override;

    private:
        uq_ptr<renderer>    works;
    };
}