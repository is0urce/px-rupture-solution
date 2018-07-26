// name: script_component.hpp
// type: c++ header
// auth: is0urce
// desc: class declaration

#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>
#include <px/es/useable.hpp>

#include <string>
#include <cstdint>

namespace px {

    class environment;
    class body_component;
    class script_system;

    class script_component final
        : public component
        , public link_dispatcher<script_component>
        , public useable<body_component *, environment *>
    {
    public:
        std::string const& name() const noexcept;
        std::uint32_t distance() const noexcept;
        script_system const* get_machine() const noexcept;
        void set_script_name(std::string const& new_script_name);

    public:
        virtual ~script_component() override;

        script_component();
        script_component(script_system * machne, std::string const& name, std::uint32_t distance);
        script_component(script_system * machne, std::string const& name);
        script_component(script_system * machne);

        script_component(script_component const&) = delete;
        script_component & operator=(script_component const&) = delete;

    public:
        template <typename Archive>
        void            serialize(Archive & archive) {
            archive(script_name, activation_distance);
        }

    protected:
        virtual bool    can_use_useable(body_component * user, environment * environment) const override;
        virtual void    use_useable(body_component * user, environment * environment) override;

    private:
        std::string     script_name;            // script name
        std::uint32_t   activation_distance;    // min distance to use
        script_system * machine;
    };
}