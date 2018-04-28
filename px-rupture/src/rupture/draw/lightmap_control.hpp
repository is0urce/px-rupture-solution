// name: lightmap_control.hpp
// type: c++ struct

#pragma once

#include "lightmap_data.hpp"

namespace px {

    class lightmap_control {
    public:
        bool is_dirty() const noexcept {
            return current && last && (dirty || current->version != prev_version);
        }

        void notify_cashing() noexcept {
            dirty = false;
            prev_version = current ? current->version : ((unsigned int)-1);
        }

        size_t width() const noexcept {
            return current ? current->width : 0;
        }

        size_t height() const noexcept {
            return current ? current->height : 0;
        }

        int current_ox() const noexcept {
            return current ? current->ox : 0;
        }

        int current_oy() const noexcept {
            return current ? current->oy : 0;
        }

        int last_ox() const noexcept {
            return last ? last->ox : 0;
        }

        int last_oy() const noexcept {
            return last ? last->oy : 0;
        }

        int dx() const noexcept {
            return current_ox() - last_ox();
        }

        int dy() const noexcept {
            return current_oy() - last_oy();
        }

        float max_luminance() const noexcept {
            return current ? current->luminance_max : 0;
        }
        float last_max_luminance() const noexcept {
            return last ? last->luminance_max : 0;
        }

    public:
        lightmap_data const*    current;
        lightmap_data const*    last;
        unsigned int            prev_version;
        bool                    dirty;

    public:
        lightmap_control() noexcept
            : current(nullptr)
            , dirty(true)
            , last(0)
            , prev_version((unsigned int)-1) {
        }
    };
}