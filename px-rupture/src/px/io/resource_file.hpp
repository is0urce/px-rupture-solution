// name: resource_file.hpp

#pragma once

// This 'documentation' was saved by git after storm outage
// Chunk format
// ID SIZE DATA

// File format

// RESF optional
// KEYS
// EXTR optional
// DATA = VOID*

// Keys data:
// strz:key pos:LEFT pos:RIGHT pos:CURR

#include "resource_file.hpp"

#include <cstdint>
#include <cstring>

namespace px {

    class resource_file {
    public:
        using pos_t = std::uint32_t;
        using name_t = std::uint32_t;

        struct header {
            name_t  name;
            pos_t   size;
        };

    public:
        static auto compare(char const* a, char const* b) {
            return std::strcmp(a, b);
        }

        static constexpr bool is_valid(pos_t position) {
            return position > 0;
        }

        static constexpr name_t keys_name() {
            return 'KEYS';
        }

        static constexpr name_t data_name() {
            return 'DATA';
        }

        static constexpr name_t void_name() {
            return 'VOID';
        }

        static constexpr name_t noluck_name() {
            return '!LCK';
        }
    };
}