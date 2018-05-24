// name: coordinate.hpp
// type: c++ header
// desc: class for 2d verctor with real components

#pragma once

#include <px/common/coordinate.hpp>
#include <px/common/coordinate_operation.hpp>
#include <px/common/point.hpp>

namespace px {

    class vector2
        : public coordinate<double, 2>
        , public coordinate_operation<vector2>
    {
    public:
        constexpr component x() const noexcept {
            return m_array[0];
        }

        constexpr component y() const noexcept {
            return m_array[1];
        }

        point2 floor() const {
            point2 result;
            for (size_t i = 0; i != depth; ++i) {
                result[i] = static_cast<point2::component>(std::floor(m_array[i]));
            }
            return result;
        }

        point2 ceil() const {
            point2 result;
            for (size_t i = 0; i != depth; ++i) {
                result[i] = static_cast<point2::component>(std::ceil(m_array[i]));
            }
            return result;
        }

        point2 round() const {
            point2 result;
            for (size_t i = 0; i != depth; ++i) {
                result[i] = static_cast<point2::component>(std::round(m_array[i]));
            }
            return result;
        }

        template <typename TargetType, typename Converter>
        TargetType convert(Converter convert_fn) const {
            TargetType result;
            for (size_t i = 0; i != depth; ++i) {
                result[i] = convert_fn(m_array[i]);
            }
            return result;
        }

        // mutations

        void normalize() {
            auto len = magnitude();
            if (len != 0) {
                divide(len);
            }
        }

        vector2 normalized() {
            vector2 result(*this);
            result.normalize();
            return result;
        }

        vector2 lerp(vector2 b, component t) const {
            b.multiply(t);
            b.move(multiplied(vector2{ 1 - t, 1 - t }));
            return b;
        }

    public:
        // default constructor is for uninitialized state
        // use vector2(0.0, 0.0) for constexpr
        vector2() noexcept = default;

        constexpr vector2(component x, component y) noexcept
            : coordinate(x, y) {
        }

        constexpr vector2(component i) noexcept
            : coordinate(i, i) {
        }

        constexpr vector2(point2 const& that) noexcept
            : coordinate(static_cast<component>(that.x()), static_cast<component>(that.y())) {
        }

        constexpr vector2(vector2 const&) noexcept = default;
        vector2 & operator=(vector2 const&) noexcept = default;
    };

    namespace {

        vector2 operator+(vector2 lhs, vector2 const& rhs) {
            lhs += rhs;
            return lhs;
        }

        vector2 operator-(vector2 lhs, vector2 const& rhs) {
            lhs -= rhs;
            return lhs;
        }

        vector2 operator*(vector2 lhs, vector2 const& rhs) {
            lhs *= rhs;
            return lhs;
        }

        vector2 operator/(vector2 lhs, vector2 const& rhs) {
            lhs /= rhs;
            return lhs;
        }
    }
}