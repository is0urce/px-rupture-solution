// name: row.hpp
// type: c++ template class
// auth: is0urce
// desc: array container with fixed max length

#pragma once

#include <stdexcept>    // exception
#include <utility>      // move

namespace px {

    template <typename T, size_t N>
    class row {
    public:
        using element_type = T;

    public:

        size_t size() const noexcept {
            return cursor;
        }

        T * data() noexcept {
            return &raw;
        }

        T * begin() noexcept {
            return &reinterpret()[0];
        }
        T * end() noexcept {
            return &reinterpret()[cursor];
        }
        T const* begin() const noexcept {
            return &reinterpret()[0];
        }
        T const* end() const noexcept {
            return &reinterpret()[cursor];
        }
        T const* cbegin() const noexcept {
            return &reinterpret()[0];
        }
        T const* cend() const noexcept {
            return &reinterpret()[cursor];
        }

        void erase(T * ptr) {
            *ptr = std::move(back());
            back().~T();
            --cursor;
        }

        T & back() {
            if (cursor == 0) throw std::runtime_error("px::row::back() - no elements");
            return reinterpret()[cursor - 1];
        }
        T const& back() const {
            if (cursor == 0) throw std::runtime_error("px::row::back() const - no elements");
            return reinterpret()[cursor - 1];
        }
        T & operator[](size_t index) noexcept {
            return reinterpret()[index];
        }
        T const& operator[](size_t index) const noexcept {
            return reinterpret()[index];
        }
        T & at(size_t index) {
            if (index >= cursor) throw std::runtime_error("px::row::at() - out of range");
            return reinterpret()[index];
        }
        T const& at(size_t index) const {
            if (index >= cursor) throw std::runtime_error("px::row::at() - out of range");
            return reinterpret()[index];
        }

        T & push_back(T const& value) {
            if (cursor == N) throw std::runtime_error("px::row::push_back(value) - out of memory");
            T * ptr = &(reinterpret()[cursor++]);
            new (ptr) T(value);
            return *ptr;
        }

        T & push_back(T && value) {
            if (cursor == N) throw std::runtime_error("px::row::push_back(value) - out of memory");
            T * ptr = &(reinterpret()[cursor++]);
            new (ptr) T(std::forward<T>(value));
            return *ptr;
        }

        template <typename ...Args>
        T & emplace_back(Args && ...args) {
            if (cursor == N) throw std::runtime_error("px::row::emplace_back(value) - out of memory");
            T * ptr = &(reinterpret()[cursor++]);
            new (ptr) T(std::forward<Args>(args)...);
            return *ptr;
        }

        void pop() {
            if (cursor == 0) throw std::runtime_error("px::row::pop(value) - no elements");
            reinterpret()[--cursor].~T();
        }

        bool empty() const noexcept {
            return cursor == 0;
        }

        void clear() {
            for (auto i = begin(), e = end(); i != e; ++i) {
                i->~T();
            }
            cursor = 0;
        }

    public:
        ~row() {
            clear();
        }
        row()
            : cursor(0)
        {
        }
        row(row const& that)
            : row()
        {
            *this = that;
        }
        row(row && that)
            : row()
        {
            *this = std::move(that);
        }
        row & operator=(row const& that) {
            clear();
            T * arr = reinterpret();
            for (auto i = that.begin(), e = that.end(); i != e; ++i) {
                new (&arr[cursor++]) T(*i);
            }
            return *this;
        }
        row & operator=(row && that) {
            clear();
            T * arr = reinterpret();
            for (auto i = that.begin(), e = that.end(); i != e; ++i) {
                new (&arr[cursor++]) T(std::move(*i));
            }
            return *this;
        }

    private:
        T * reinterpret() noexcept {
            return reinterpret_cast<T*>(&raw);
        }
        T const* reinterpret() const noexcept {
            return reinterpret_cast<T const*>(&raw);
        }

    private:
        alignas(T) char			raw[N * (sizeof T)];
        size_t					cursor;
    };
}