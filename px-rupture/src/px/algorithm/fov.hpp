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
            reach = radius;
            grid.resize(reach * 2 + 1, reach * 2 + 1);
            grid.fill(false);

            recursive_shadowcasting::light(center.x(), center.y(), reach, std::forward<Predicate>(predicate), [this](int x, int y) {
                point2 const relative = point2(x, y) - center + point2(reach, reach);
                if (grid.contains(relative)) {
                    grid.set(true, relative);
                }
            });
        }

        bool contains(point2 const& absolute) const {
            point2 const relative = absolute - center + point2(reach, reach);
            return grid.contains(relative) ? grid.get(relative) : false;
        }

        unsigned int range() const noexcept {
            return reach;
        }

    public:
        fov()
            : reach(0) {
        }

    private:
        matrix2<bool>   grid;
        point2          center;
        unsigned int    reach;      // radius
    };
}