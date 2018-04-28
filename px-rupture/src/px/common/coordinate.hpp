// name: coordinate.hpp
// type: c++ header
// desc: base template class for points and vectors

#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <numeric>

namespace px {

    template <typename Component, size_t Dim>
    class coordinate {
    public:
        static const size_t depth = Dim;
        using component = Component;

    public:
        component const& operator[](size_t i) const {
            return m_array[i];
        }

        component & operator[](size_t i) {
            return m_array[i];
        }

        component & at(size_t i) {
            return m_array.at(i);
        }

        component const& at(size_t i) const {
            return m_array.at(i);
        }

        template <size_t Index>
        constexpr component get() const noexcept {
            static_assert(Index < Dim, "Index < Dim");
            return m_array[Index];
        }

        template <size_t Index>
        void set(component val) noexcept {
            static_assert(Index < Dim, "Index < Dim");
            m_array[Index] = val;
        }

        bool empty() const noexcept {
            bool result = true;
            for (size_t i = 0; i != Dim; ++i) {
                result &= m_array[i] == 0;
            }
            return result;
        }

        // transforms

        void negate() noexcept {
            for (size_t i = 0; i != Dim; ++i) {
                m_array[i] = -m_array[i];
            }
        }

        template <size_t Axis>
        void mirror() noexcept {
            static_assert(Axis < Dim, "Axis < Dim");
            m_array[Axis] = -m_array[Axis];
        }

        void mirror(size_t axis) {
            m_array[axis] = -m_array[axis];
        }

        // move along specified axis
        template <size_t Axis>
        void move_axis(component distance) {
            static_assert(Axis < Dim, "Axis < Dim");
            m_array[Axis] += distance;
        }

        // move along axis
        void move_axis(size_t axis, component distance) {
            m_array[axis] += distance;
        }

        template <size_t Axis>
        void increment_axis() {
            static_assert(Axis < Dim, "Axis < Dim");
            m_array[Axis] += 1;
        }

        void clamp(coordinate const& min, coordinate const& max) {
            for (size_t i = 0; i != Dim; ++i) {
                m_array[i] = (std::min)((std::max)(min.m_array[i], m_array[i]), max.m_array[i]);
            }
        }

        // vector transforms

        template<typename ConvertElement>
        void move(coordinate<ConvertElement, depth> const& move) {
            for (size_t i = 0; i != Dim; ++i) {
                m_array[i] += move[i];
            }
        }

        template<typename ConvertElement>
        void reverse(coordinate<ConvertElement, depth> const& move) {
            for (size_t i = 0; i != Dim; ++i) {
                m_array[i] -= move[i];
            }
        }

        template<typename ConvertElement>
        void multiply(coordinate<ConvertElement, depth> const& multiplier) {
            for (size_t i = 0; i != Dim; ++i) {
                m_array[i] *= multiplier[i];
            }
        }

        template<typename ConvertElement>
        void divide(coordinate<ConvertElement, depth> const& divisor) {
            for (size_t i = 0; i != Dim; ++i) {
                m_array[i] /= divisor[i];
            }
        }

        // component transforms

        void multiply(component multiplier) {
            for (size_t i = 0; i != Dim; ++i) {
                m_array[i] *= multiplier;
            }
        }
        void divide(component divisor) {
            for (size_t i = 0; i != Dim; ++i) {
                m_array[i] /= divisor;
            }
        }

        // distances

        // chebyshev distance to specified coordinate
        component king_distance(coordinate const& target) const {
            component distance = 0;
            for (size_t i = 0; i != Dim; ++i) {
                distance = (std::max)(distance, (std::abs)(m_array[i] - target.m_array[i]));
            }
            return distance;
        }

        static component king_distance(coordinate const& from, coordinate const& target) {
            component distance = 0;
            for (size_t i = 0; i != Dim; ++i) {
                distance = (std::max)(distance, (std::abs)(from.m_array[i] - target.m_array[i]));
            }
            return distance;
        }

        // manhattan distance to specified coordinate
        component block_distance(coordinate const& target) const noexcept {
            component distance = 0;
            for (size_t i = 0; i != Dim; ++i) {
                distance += (std::abs)(m_array[i] - target.m_array[i]);
            }
            return distance;
        }

        // distance to to specified coordinate, squared for performance reasons
        component distance2(coordinate const& target) const noexcept {
            component distance = 0;
            for (size_t i = 0; i != Dim; ++i) {
                component delta = m_array[i] - target.m_array[i];
                distance += delta * delta;
            }
            return distance;
        }

        component magnitude2() const noexcept {
            component distance = 0;
            for (size_t i = 0; i != Dim; ++i) {
                distance += m_array[i] * m_array[i];
            }
            return distance;
        }

        // distance to to specified coordinate
        auto distance(coordinate const& target) const {
            return std::sqrt(distance2(target));
        }

        // size of vector
        auto magnitude() const {
            return std::sqrt(magnitude2());
        }

        // i/o

        template <typename Memory>
        void read(Memory const* memory) {
            for (size_t i = 0; i != Dim; ++i) {
                m_array[i] = static_cast<component>(memory[i]);
            }
        }

        template <typename Memory>
        void write(Memory * memory) const {
            for (size_t i = 0; i != Dim; ++i) {
                memory[i] = static_cast<Memory>(m_array[i]);
            }
        }

        bool lex_less(coordinate const& cmp) const {
            return std::lexicographical_compare(m_array.cbegin(), m_array.cend(), cmp.m_array.cbegin(), cmp.m_array.cend());
        }

        static bool lex_less(coordinate const& a, coordinate const& b) {
            return a.lex_less(b);
        }

        template <typename Archive>
        void serialize(Archive & archive) {
            for (size_t i = 0; i != Dim; ++i) {
                archive(m_array[i]);
            }
        }

    public:
        // default constructor is for uninitialized state
        // use coordinate(0,..) for constexpr
        coordinate() noexcept = default;

        template <typename... Args>
        constexpr coordinate(Args &&... args) noexcept
            : m_array{ std::forward<Args>(args)... } {
        }

    protected:
        std::array<component, depth> m_array;

        template <typename C, size_t D>
        friend bool operator==(coordinate<C, D> const& a, coordinate<C, D> const& b);
    };

    template <typename C, size_t D>
    bool operator==(coordinate<C, D> const& lh, coordinate<C, D> const& rh) {
        return lh.m_array == rh.m_array;
    }

    template <typename C, size_t D>
    bool operator!=(coordinate<C, D> const& lh, coordinate<C, D> const& rh) {
        return !operator==(lh, rh);
    }
}