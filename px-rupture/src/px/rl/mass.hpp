// name: mass.hpp
// type: c++ header
// desc: tile class
// auth: is0urce

// base of shared tile and unit proprieties
// transparensy and multiple layers of traversability

#pragma once

#include <px/rl/traverse_options.hpp>

namespace px::rl {

    template <typename E, size_t Size = static_cast<size_t>(E::max_value)>
    class mass
        : public traverse_options<E, Size>
    {
    public:
        constexpr bool is_transparent() const noexcept {
            return transparent;
        }

        // setters

        void make_transparent() noexcept {
            transparent = true;
        }

        void make_transparent(bool transparency) noexcept {
            transparent = transparency;
        }

        void make_opaque() noexcept {
            transparent = false;
        }

        // templates

        template <bool BoolValue>
        void make_transparent() noexcept {
            transparent = BoolValue;
        }

        // utility

        void make_solid() {
            make_opaque();
            make_blocking();
        }

        void make_empty() {
            make_transparent();
            make_traversable();
        }

        // serialization

        template <typename Archive>
        void serialize(Archive & archive) {
            archive(static_cast<traverse_options<E, Size> &>(*this));
            archive(transparent);
        }

    public:
        constexpr mass() noexcept
            : mass(false)
        {
        }
        constexpr mass(bool transparency) noexcept
            : transparent(transparency)
        {
        }
        constexpr mass(bool transparency, bool can_traverse)
            : transparent(transparency)
        {
            make_traversable(can_traverse);
        }
        constexpr mass(bool transparency, traverse_options const& opts)
            : traverse_options(opts)
            , transparent(transparency)
        {
        }

    private:
        bool transparent;
    };
}