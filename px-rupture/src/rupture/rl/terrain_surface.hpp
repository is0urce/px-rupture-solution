// name: terrain_surface.hpp
// type: c++ header

#pragma once

#include "../app/settings.hpp"

#include "terrain_patch.hpp"
#include "tile_library.hpp"

#include <px/io/resource_file.hpp>

#include <px/common/matrix.hpp>
#include <px/common/point.hpp>
#include <px/common/vector.hpp>
#include <px/common/stream.hpp>

#include <px/memory/uq_ptr.hpp>

#include <tuple>

namespace px {

    template <size_t L, size_t R>
    class terrain_surface {
    private:
        static size_t const range = R * 2 + 1;

    public:
        using patch_type = terrain_patch<L, L>;
        using stream_type = stream<terrain_patch<L, L>>;
        using stream_ptr = uq_ptr<stream_type>;
        using container_type = matrix2<stream_ptr, range, range>;

    public:
        bool is_transparent(point2 const& world) const {
            patch_type const* chunk;
            point2 remainder;
            std::tie(chunk, remainder) = select(world);
            return chunk && chunk->is_transparent(remainder);
        }

        bool is_traversable(point2 const& world, rl::traverse_options<rl::traverse> const& opts) const {
            patch_type const* chunk;
            point2 remainder;
            std::tie(chunk, remainder) = select(world);
            return chunk && chunk->is_traversable(remainder, opts);
        }

        void pset(std::uint32_t block_id, point2 const& world) {
            patch_type * chunk;
            point2 remainder;
            std::tie(chunk, remainder) = select(world);

            if (chunk) {
                chunk->pset(block_id, remainder);
            }
        }

        void focus(point2 const& world_absolute) {
            point2 cell = get_cell(world_absolute);
            if (current_cell == cell) return;

            container_type origin;
            origin.swap(terrain);

            // copy old
            point2 shift = cell - current_cell;
            terrain.enumerate([&](size_t ox, size_t oy, stream_ptr & ptr) {
                point2 index = shift + point2(static_cast<int>(ox), static_cast<int>(oy));

                if (index.x() >= 0 && index.y() >= 0 && origin.contains(index)) {
                    ptr.swap(origin[index]);
                }
            });

            current_cell = cell;
        }

        void wait() {
            terrain.enumerate([&](size_t /*ox*/, size_t /*oy*/, stream_ptr & ptr) {
                if (ptr) {
                    ptr->wait();
                }
            });
        }

        void clear() {
            terrain.enumerate([&](size_t /*ox*/, size_t /*oy*/, stream_ptr & ptr) {
                ptr.reset();
            });
        }

        template <typename Operator>
        void enumerate(Operator && function) {
            terrain.enumerate([&](size_t ox, size_t oy, stream_ptr & ptr) {
                function(index_to_cell(ox, oy), ptr);
            });
        }

        template <typename Operator>
        void enumerate(Operator && function) const {
            terrain.enumerate([&](size_t ox, size_t oy, stream_ptr const& ptr) {
                function(index_to_cell(ox, oy), ptr);
            });
        }

        static point2 get_cell(point2 const& world) {
            return (vector2(world) / vector2(L, L)).floor();
        }

    public:
        terrain_surface()
            : current_cell(0, 0)
            , file("data/terrain.res") {
        }

    private:
        static std::tuple<point2, point2> divide(point2 const& a, point2 const& b) {
            point2 div = (vector2(a) / b).floor();
            point2 mod = a - div * b;
            return { div, mod };
        }

        static std::tuple<point2, point2> divide(point2 const& a) {
            return divide(a, point2(L, L));
        }

        point2 cell_to_index(point2 cell) const {
            cell -= current_cell;
            cell += point2(R, R);
            return cell;
        }

        point2 index_to_cell(size_t ox, size_t oy) const {
            point2 index(static_cast<int>(ox), static_cast<int>(oy));
            index += current_cell;
            index -= point2(R, R);
            return index;
        }

        std::tuple<patch_type *, point2> select(point2 const& world) {
            point2 cell;
            point2 remainder;
            std::tie(cell, remainder) = divide(world);
            point2 index = cell_to_index(cell);

            return { contains_index(index) ? chunk(index) : nullptr, remainder };
        }

        std::tuple<patch_type const*, point2> select(point2 const& world) const {
            point2 cell;
            point2 remainder;
            std::tie(cell, remainder) = divide(world);
            point2 index = cell_to_index(cell);

            return { contains_index(index) ? chunk(index) : nullptr, remainder };
        }

        patch_type * chunk(point2 const& index) {
            stream_ptr & ptr = terrain[index];
            return (ptr && ptr->is_loaded()) ? ptr->get() : nullptr;
        }

        patch_type * chunk(point2 const& index) const {
            stream_ptr const& ptr = terrain[index];
            return (ptr && ptr->is_loaded()) ? ptr->get() : nullptr;
        }

        bool contains_index(point2 const& index) const {
            return index.x() >= 0 && index.y() >= 0 && terrain.contains(index);
        }

    private:
        container_type  terrain;
        point2          current_cell;
        resource_file   file;
    };
}