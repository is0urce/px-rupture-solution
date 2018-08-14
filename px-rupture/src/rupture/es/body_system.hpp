// name: body_system.hpp
// type: c++ header
// auth: is0urce
// desc: class declaration

#pragma once

// body system handles effect ticks, dying, experience splashes

#include <px/es/system.hpp>
#include <px/es/delta.hpp>

#include <px/memory/uq_ptr.hpp>

namespace px {

    class body_component;
    class body_works;
    class environment;

    class body_system
        : public system<delta>
    {
    public:
        void                    assign_environment(environment * env) noexcept; // bind system to an environment
        uq_ptr<body_component>  make();

    public:
        virtual                 ~body_system() override;
        body_system();

    protected:
        virtual void            turn_update_system(delta_type const& delta_time) override;

    private:
        uq_ptr<body_works>      works;
    };
}