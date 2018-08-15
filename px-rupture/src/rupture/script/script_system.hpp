// name: script_system.hpp
// type: c++ header
// auth: is0urce
// desc: class declaration

#pragma once

#include <px/memory/uq_ptr.hpp>

#include <px/common/point.hpp>
#include <px/rl/skill/skill_functional.hpp>

#include <px/es/system.hpp>
#include <px/es/delta.hpp>

namespace px {

    class body_component;
    class environment;
    class script_component;
    class script_internal;

    class script_system
        : public system<delta>
    {
    public:
        void                                                                assign_environment(environment * game);
        void                                                                execute(std::string const& script_name);
        void                                                                run(std::string const& code);
        rl::skill_functional<body_component *, body_component *, point2>    impact(std::string const& skill_name);
        uq_ptr<script_component>                                            make(std::string const& script_name);
        void                                                                close();

    public:
        virtual ~script_system();
        script_system();
        script_system(script_system const&) = delete;
        script_system & operator=(script_system const&) = delete;

    public:
        virtual void                                                        turn_update_system(delta_type const& delta_time) override;
        virtual void                                                        update_system(delta_type const& delta_time) override;

    private:
        uq_ptr<script_internal>                                             works;
        double                                                              garbage_timer;
    };
}