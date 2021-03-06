// name: npc_system.hpp
// type: c++
// auth: is0urce
// desc: class declaration

#pragma once

#include <px/es/system.hpp>
#include <px/es/delta.hpp>

#include <px/memory/uq_ptr.hpp>

namespace px {

    class npc_component;
    class npc_works;
    class scene;

    class npc_system
        : public system<delta>
    {
    public:
        void                    assign_scene(scene * world) noexcept;
        uq_ptr<npc_component>   make();

    public:
        virtual                 ~npc_system() override;
        npc_system();

    protected:
        virtual void            turn_update_system(delta_type const& delta_time) override;

    private:
        uq_ptr<npc_works>       works;
    };
}