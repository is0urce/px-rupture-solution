// name: fov.hpp
// type: c++ header
// auth: is0urce
// desc: field of view

#pragma once

#include <px/common/matrix.hpp>
#include <px/common/point.hpp>

#include <px/algorithm/recursive_shadowcasting.hpp>

namespace px {

    class fov {
    public:
        template <typename Predicate>
        void calculate(point2 source, unsigned int radius, Predicate && predicate) {
            center = source;
            r = radius;
            grid.resize(r * 2 + 1, r * 2 + 1);
            grid.fill(false);

            recursive_shadowcasting::light(center.x(), center.y(), r, std::forward<Predicate>(predicate), [this](int x, int y) {
                point2 relative = point2(x, y) - center + point2(r, r);
                if (grid.contains(relative)) {
                    grid.set(true, relative);
                }
            });
        }

        bool contains(point2 const& absolute) const {
            point2 relative = absolute - center + point2(r, r);
            return grid.contains(relative) ? grid.get(relative) : false;
        }

        unsigned int range() const noexcept {
            return r;
        }

    public:
        fov()
            : r(0)
        {
        }

    private:
        matrix2<bool>   grid;
        point2          center;
        unsigned int    r;      // radius
    };
}