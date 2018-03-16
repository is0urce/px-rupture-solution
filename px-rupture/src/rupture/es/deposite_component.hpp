// name: deposite_component.hpp
// type: c++ header
// auth: is0urce
// desc: class declaration

#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>
#include <px/es/useable.hpp>

#include <cstdint>

namespace px {

    class body_component;
    class container_component;
    class composite_component;
    class environment;

    class deposite_component final
        : public component
        , public link<container_component>
        , public link<composite_component>
        , public link_dispatcher<deposite_component>
        , public useable<body_component *, environment *>
    {
    public:
        using time_type = std::uint32_t;

    public:
        void            set_dissolve(bool should_destroy) noexcept;
        void            set_use_duration(time_type timer) noexcept;
        bool            is_dissolving() const noexcept;
        time_type       get_use_duration() const noexcept;

    public:
        virtual ~deposite_component();
        deposite_component();
        deposite_component(deposite_component const&) = delete;
        deposite_component & operator=(deposite_component const&) = delete;

    public:
        template <typename Archive>
        void            serialize(Archive & archive) {
            archive(dissolve, use_duration);
        }

    protected:
        virtual bool    can_use_useable(body_component * user, environment * environment) const override;
        virtual void    use_useable(body_component * user, environment * environment) override;

    private:
        time_type       use_duration;   // time requirement for using this deposite
        bool            dissolve;       // true if object should be destroyea after use
    };
}