// name: coordinate.hpp
// type: c++ header
// desc: class for 2d point with integer components
// auth: is0urce

#pragma once

#include "coordinate.hpp"
#include "coordinate_operation.hpp"

namespace px {

    class point2
        : public coordinate<int, 2>
        , public coordinate_operation<point2>
    {
    public:
        constexpr component x() const noexcept {
            return m_array[0];
        }

        constexpr component y() const noexcept {
            return m_array[1];
        }

    public:
        // default constructor is for uninitialized state
        // use point2(0, 0) for constexpr
        point2() noexcept = default;

        constexpr point2(component x, component y) noexcept
            : coordinate(x, y) {
        }

        constexpr point2(component i) noexcept
            : coordinate(i, i) {
        }

        constexpr point2(point2 const&) noexcept = default;
    };

    namespace {

        point2 operator+(point2 lhs, point2 const& rhs) {
            lhs += rhs;
            return lhs;
        }

        point2 operator-(point2 lhs, point2 const& rhs) {
            lhs -= rhs;
            return lhs;
        }

        point2 operator*(point2 lhs, point2 const& rhs) {
            lhs *= rhs;
            return lhs;
        }

        point2 operator/(point2 lhs, point2 const& rhs) {
            lhs /= rhs;
            return lhs;
        }
    }
}