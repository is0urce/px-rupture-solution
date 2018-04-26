// name: sound_component
// type: c++
// auth: is0urce
// desc: class declaration

#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>

namespace px {

    class sound_component final
        : public component
        , public link_dispatcher<sound_component>
    {
    public:
        virtual ~sound_component() override = default;

        sound_component() = default;

        sound_component(sound_component const&) = delete;
        sound_component & operator=(sound_component const&) = delete;
    };
}