// name: tile.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include "../es/transform_component.hpp"

#include <px/memory/uq_ptr.hpp>
#include <px/rl/mass.hpp>
#include <px/rl/traverse.hpp>

#include <cstdint>

namespace px {

    class sprite_component;

    class tile {
    public:
        std::uint32_t               block_id;
        transform_component         transform;
        uq_ptr<sprite_component>    sprite;
        rl::mass<rl::traverse>      mass;

    public:
        tile() = default;
        tile(tile && that) noexcept
            : sprite(std::move(that.sprite))
            , transform(that.transform.position())
            , mass(std::move(that.mass))
            , block_id(std::move(that.block_id))
        {
        }
        tile(tile const&) noexcept = delete;
        tile & operator=(tile const&) = delete;
    };
}