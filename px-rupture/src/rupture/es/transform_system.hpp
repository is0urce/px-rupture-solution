// name: transform_system.hpp
// type: c++
// auth: is0urce
// desc: class declaration

#pragma once

#include <px/es/system.hpp>
#include <px/es/delta.hpp>

#include <px/memory/uq_ptr.hpp>

namespace px {

    class transform_component;
    class transform_works;

    class transform_system
        : public system<delta>
    {
    public:
        uq_ptr<transform_component> make();
        void                        store();

    public:
        virtual                     ~transform_system() override;
        transform_system();

    protected:
        virtual void                turn_update_system(delta_type const& delta_time) override;

    private:
        uq_ptr<transform_works>     works;
    };
}