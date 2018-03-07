// name: bresenham.hpp
// type: c++

#pragma once

#include <algorithm>
#include <cmath>

namespace px {

    class bresenham {
    public:

        template <typename Coordinate, typename Plot>
        static void line(Coordinate && a, Coordinate && b, Plot && fn) {
            line(a.x(), a.y(), b.x(), b.y(), std::forward<Plot>(fn));
        }

        template <typename Plot>
        static void line(int x0, int y0, int x1, int y1, Plot && fn) {
            int dx = std::abs(x1 - x0);
            int dy = std::abs(y1 - y0);
            int steps = std::max(dx, dy) + 1;

            if (dx >= dy) {
                if (x1 > x0) {
                    if (y1 > y0)    line_h<+1, +1>(x0, y0, dx, dy, steps, std::forward<Plot>(fn));
                    else            line_h<+1, -1>(x0, y0, dx, dy, steps, std::forward<Plot>(fn));
                }
                else {
                    if (y1 > y0)    line_h<-1, +1>(x0, y0, dx, dy, steps, std::forward<Plot>(fn));
                    else            line_h<-1, -1>(x0, y0, dx, dy, steps, std::forward<Plot>(fn));
                }
            }
            else {
                if (x1 > x0) {
                    if (y1 > y0)    line_v<+1, +1>(x0, y0, dx, dy, steps, std::forward<Plot>(fn));
                    else            line_v<+1, -1>(x0, y0, dx, dy, steps, std::forward<Plot>(fn));
                }
                else {
                    if (y1 > y0)    line_v<-1, +1>(x0, y0, dx, dy, steps, std::forward<Plot>(fn));
                    else            line_v<-1, -1>(x0, y0, dx, dy, steps, std::forward<Plot>(fn));
                }
            }
        }

    private:
        template <int MX, int MY, typename Plot>
        static void line_h(int x, int y, int dx, int dy, int steps, Plot && fn) {
            int error = 0;
            for (; steps != 0; --steps) {
                fn(x, y);

                x += MX;

                error += dy;
                if (2 * error > dx) {
                    error -= dx;
                    y += MY;
                }
            }
        }

        template <int MX, int MY, typename Plot>
        static void line_v(int x, int y, int dx, int dy, int steps, Plot && fn) {
            int error = 0;
            for (; steps != 0; --steps) {
                fn(x, y);

                y += MY;

                error += dx;
                if (2 * error > dy) {
                    error -= dy;
                    x += MX;
                }
            }
        }
    };
}