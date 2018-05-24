// name: value_component
// type: c++
// auth: is0urce
// desc: class declaration

#pragma once

#include <px/es/values.hpp>
#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>

#include <cstdint>

namespace px {

    class value_component final
        : public values<std::int32_t, double>
        , public component
        , public link_dispatcher<value_component>
    {
    public:
        virtual ~value_component() override = default;

        value_component() = default;

        value_component(value_component const&) = delete;
        value_component & operator=(value_component const&) = delete;
    };
}