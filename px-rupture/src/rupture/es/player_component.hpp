// name: player_component.hpp
// type: c++ header
// auth: is0urce

#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>

#include <cstdint>

namespace px {

    class player_component
        : public component
        , public link_dispatcher<player_component>
    {
    public:
        std::uint32_t get_seed() const noexcept {
            return seed;
        }

        void set_seed(std::uint32_t seed_value) noexcept {
            seed = seed_value;
        }

        template <typename Archive>
        void serialize(Archive & archive) {
            archive(seed);
        }

    public:
        virtual ~player_component() override = default;

        player_component()
            : seed(0) {
        }

    private:
        std::uint32_t   seed;
    };
}