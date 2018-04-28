// name: bar.hpp
// type: c++ header
// desc: template struct
// auth: is0urce

// resource bar
// value that have maximal cap
// starting value is maximal

#pragma once

#include <algorithm>

namespace px::rl {

    template <typename T>
    class bar {
    public:
        using value_type = T;

    public:
        // querry

        constexpr T current() const noexcept {
            return m_current;
        }

        constexpr T maximum() const noexcept {
            return m_max;
        }

        constexpr bool is_empty() const noexcept {
            return m_current <= 0;
        }

        constexpr bool is_full() const noexcept {
            return m_current == m_max;
        }

        // restore value to maximum

        T restore() noexcept {
            T const last = m_current;
            m_current = m_max;
            return m_current - last;
        }

        T restore(T magnitude) noexcept {
            T const last = m_current;
            m_current = cap(m_current + magnitude);
            return m_current - last;
        }

        void modify(T magnitude) noexcept {
            m_current = cap(m_current + magnitude);
        }

        T damage(T magnitude) noexcept {
            T const last = m_current;
            m_current = cap(m_current - magnitude);
            return last - m_current;
        }

        // mutation operators

        bar & operator=(T c) noexcept {
            set(c);
            return *this;
        }

        bar & operator+=(T c) noexcept {
            modify(c);
            return *this;
        }

        bar & operator-=(T c) noexcept {
            modify(-c);
            return *this;
        }

        // comparison operators

        constexpr bool operator<(T c) const noexcept {
            return m_current < c;
        }

        constexpr bool operator==(T c) const noexcept {
            return m_current == c;
        }

        constexpr bool operator<=(T c) const noexcept {
            return operator<(*this, c) || operator==(this*, c);
        }

        constexpr bool operator>=(T c) const noexcept {
            return !operator<(*this, c);
        }

        constexpr bool operator!=(T c) const noexcept {
            return !operator==(*this, c);
        }

        constexpr bool operator>(T c) const noexcept {
            return !operator<=(*this, c);
        }

        // value cast operators

        constexpr operator value_type() const noexcept {
            return m_current;
        }

        constexpr operator bool() const noexcept {
            return !empty();
        }

        void set_current(T current) noexcept {
            m_current = cap(current);
        }

        void set_maximum(T max) noexcept {
            m_max = max;
            m_current = cap(m_current);
        }

        void set(T current_and_max) noexcept {
            m_max = current_and_max;
            m_current = current_and_max;
        }

        void set(T current, T max) noexcept {
            m_max = max;
            m_current = cap(current);
        }

        // io
        template <typename Archive>
        void serialize(Archive & archive) {
            archive(m_current, m_max);
        }

        // constructor & destructor
    public:
        constexpr bar() noexcept
            : m_current(0)
            , m_max(0) {
        }

        constexpr bar(T max) noexcept
            : m_current(max)
            , m_max(max) {
        }

        constexpr bar(T current, T max) noexcept
            : m_current(current)
            , m_max(max) {
        }

        bar(bar const&) = default;
        bar & operator=(bar const&) noexcept = default;

    private:
        constexpr T cap(T val) const noexcept {
            return std::min(val, m_max);
        }

    private:
        value_type m_current;
        value_type m_max;
    };
}