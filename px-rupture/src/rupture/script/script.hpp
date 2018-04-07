// name: script.hpp
// type: c++ header
// auth: is0urce
// desc: class declaration

#pragma once

#include <px/memory/uq_ptr.hpp>

#include <px/common/point.hpp>
#include <px/rl/skill/skill_functional.hpp>

namespace px {

    class body_component;
    class script_internal;
    class environment;

    class script final {
    public:
        void                                                                assign_environment(environment * game);
        void                                                                execute(std::string const& file);
        void                                                                run(std::string const& code);
        rl::skill_functional<body_component *, body_component *, point2>    impact(std::string const& name);

    public:
        ~script();
        script();

    private:
        uq_ptr<script_internal>                                             works;
    };
}