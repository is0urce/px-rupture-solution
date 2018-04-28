// name: sound.hpp
// type: c++ header
// desc: class
// auth: is0urce

#pragma once

namespace px {

    class sound {
    public:
        char const*     name;
        double          volume;
        double          pan;
        bool            is_on;

    public:
        template <typename Archive>
        void serialize(Archive & archive) {
            archive(name, is_on, volume, pan);
        }

    public:
        sound() noexcept
            : name(nullptr)
            , volume(1)
            , pan(0)
            , is_on(true) {
        }
    };
}