// name: bresenham.hpp
// type: c++

#pragma once

#include <cmath>

namespace px {

    class bresenham {
    public:
        template <typename Coordinate, typename Plot>
        static void line(Coordinate && a, Coordinate && b, Plot && fn) {
            int x0 = a.x();
            int y0 = a.y();
            int x1 = b.x();
            int y1 = b.y();

            int dx = std::abs(x1 - x0);
            int dy = std::abs(y1 - y0);
            int error = 0;

            int sx = x0 > x1 ? -1 : 1;
            int sy = y0 > y1 ? -1 : 1;

            int y = y0;

            for (int x = x0; x != x1; x += sx) {

                fn(Coordinate{ x, y });

                error += dy;

                if (2 * error >= dx) {
                    y += sy;
                    error -= dx;
                }
            }
        }
    };
}