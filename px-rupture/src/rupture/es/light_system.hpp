// name: light_system.hpp

#pragma once

#include <px/es/system.hpp>
#include <px/es/delta.hpp>

#include <px/memory/uq_ptr.hpp>

namespace px {

    class light_works;
    class light_component;
    class transform_component;
    class lightmap_data;
    class scene;

    class light_system final
        : public system<delta>
    {
    public:
        void                    assign_scene(scene const* stage) noexcept;
        void                    target(transform_component const* camera) noexcept;
        uq_ptr<light_component>make();
        lightmap_data const*    current_data() noexcept;
        lightmap_data const*    last_data() noexcept;
        void                    clear_lightmap();
        void                    recalculate();
        void                    set_fov_cropping(bool do_cropping) noexcept;

    public:
        virtual                 ~light_system() override;
        light_system();

    protected:
        virtual void            turn_update_system(delta_type const& delta_time) override;

    private:
        uq_ptr<light_works>     works;
    };
}