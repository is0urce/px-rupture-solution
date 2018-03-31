// name: ant_generator.hpp

#pragma once

#include <px/common/matrix.hpp>

namespace px::fn {
    class ant_generator {
    public:
        using block_type = unsigned char;

    public:
        template <typename Random>
        static matrix2<block_type> generate(Random && rng, size_t w, size_t h, unsigned int steps) {
            matrix2<block_type> map(w, h, 0);

            int x = static_cast<int>(w / 2);
            int y = static_cast<int>(h / 2);

            block_type border = 0;
            for (; steps != 0; --steps) {
                map.get_or(border, x, y) = 1;

                switch (rng() % 4) {
                case 0: ++x; break;
                case 1: --x; break;
                case 2: ++y; break;
                case 3: --y; break;
                }
            }

            return map;
        }
    };
}