// name: recursive_shadowcasting.hpp
// type: c++ header
// auth: is0urce
// desc: recursive shadowcasting

#pragma once

namespace px {

    class recursive_shadowcasting final {
    public:
        template <typename Predicate, typename Light>
        static void light(int center_x, int center_y, unsigned int radius_size, Predicate && predicate_fn, Light && light_op) {
            // octants
            static int const oct[4][8] = {
                { 1, 0, 0, -1, -1, 0, 0, 1 },
                { 0, 1, -1, 0, 0, -1, 1, 0 },
                { 0, 1, 1, 0, 0, -1, -1, 0 },
                { 1, 0, 0, 1, -1, 0, 0, -1 }
            };

            if (radius_size == 0) return;

            light_op(center_x, center_y);

            for (unsigned int i = 0; i != 8; ++i) {
                octant(center_x, center_y, radius_size, 1, 1.0, 0.0, oct[0][i], oct[1][i], oct[2][i], oct[3][i], predicate_fn, light_op); // calculate every octant
            }
        }

    private:
        template <typename Predicate, typename Light>
        static void octant(int start_x, int start_y, unsigned int radius, unsigned int row, float start, float end, int xx, int xy, int yx, int yy, Predicate && predicate, Light && light_op) {
            if (start < end) return;

            float next_start_slope = start;
            for (unsigned int i = row; i != radius; ++i) {
                int dx = 0 - i - 1;
                int const dy = 0 - i;
                bool blocked = false;
                while (dx <= 0) {
                    ++dx;
                    float const l_slope = (dx - 0.5f) / (dy + 0.5f);
                    float const r_slope = (dx + 0.5f) / (dy - 0.5f);

                    if (start < r_slope) {
                        continue;
                    }
                    else if (end > l_slope) {
                        break;
                    }
                    else {
                        int const current_x = start_x + dx * xx + dy * xy;
                        int const current_y = start_y + dx * yx + dy * yy;
                        float const slope = static_cast<float>(dx) / static_cast<float>(dy);

                        // to avoid behind-collumn peek you have to see center of a tile to actualy see it (or it shoult be wall)
                        bool const is_wall = !predicate(current_x, current_y);
                        bool const is_center = start >= slope && slope >= end;

                        if (is_center || is_wall) light_op(current_x, current_y);

                        if (blocked) {
                            if (is_wall) {
                                next_start_slope = r_slope;
                                continue;
                            }
                            else {
                                blocked = false;
                                start = next_start_slope;
                            }
                        }
                        else {
                            if (is_wall && i < radius) {
                                blocked = true;
                                octant(start_x, start_y, radius, i + 1, start, l_slope, xx, xy, yx, yy, predicate, light_op); // call recursive
                                next_start_slope = r_slope;
                            }
                        }
                    }
                }
                if (blocked) break;
            }
        }
    };
}