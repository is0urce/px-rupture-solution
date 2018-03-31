// name: uq_ptr
// type: c++ header

#pragma once

// unique_ptr but with erased deleter

#include <px/memory/abstract_release_block.hpp>

#include <cstddef> // for std::nullptr_t

namespace px {

    template <typename T>
    class uq_ptr final {
    public:
        using element_type = T;
        using pointer = element_type * ;

    public:
        void reset() noexcept {
            uq_ptr().swap(*this);
        }

        void reset(element_type * pointer, abstract_release_block * control_block) noexcept {
            uq_ptr(pointer, control_block).swap(*this);
        }

        void swap(uq_ptr & other) noexcept {
            std::swap(ptr, other.ptr);
            std::swap(ctrl, other.ctrl);
        }

        element_type * get() const noexcept {
            return ptr;
        }

        abstract_release_block * control() const noexcept {
            return ctrl;
        }

        explicit operator bool() const noexcept {
            return ptr != nullptr;
        }

        element_type * operator->() const noexcept {
            return ptr;
        }

        T const& operator*() const {
            return *ptr;
        }

        T & operator*() {
            return *ptr;
        }

        // ownership released (transferred to returned value) after call
        template <typename U>
        uq_ptr<U> downcast() noexcept {
            U * obj = static_cast<U*>(ptr);
            ptr = nullptr;
            return uq_ptr<U>(obj, ctrl);
        }

        // ownership released (transferred to returned value) after call
        // object released, if cast not succeed
        template <typename U>
        uq_ptr<U> upcast() noexcept {
            U * obj = dynamic_cast<U*>(ptr);
            if (!obj) release(); // cast unsuccessful, obect destructed here
            ptr = nullptr;
            return uq_ptr<U>(obj, ctrl);
        }

    public:
        ~uq_ptr() {
            if (ptr) {
                ctrl->release();
            }
        }
        uq_ptr(element_type * pointer, abstract_release_block * control_block) noexcept
            : ptr(pointer)
            , ctrl(control_block)
        {
        }
        uq_ptr() noexcept
            : ptr(nullptr)
        {
        }
        uq_ptr(std::nullptr_t /* null */) noexcept
            : ptr(nullptr)
        {
        }

        // copy, move

        uq_ptr(uq_ptr const& other) = delete;
        uq_ptr & operator=(uq_ptr const& other) = delete;
        uq_ptr(uq_ptr && other) noexcept
            : uq_ptr()
        {
            other.swap(*this);
        }
        uq_ptr & operator=(uq_ptr && other) {
            other.swap(*this);
            return *this;
        }
        template <typename U>
        uq_ptr(uq_ptr<U> && other) noexcept
            : uq_ptr()
        {
            other.downcast<T>().swap(*this);
        }

    private:
        pointer                     ptr;
        abstract_release_block *    ctrl;
    };

    template<typename T, typename U> inline bool operator==(uq_ptr<T> const& a, uq_ptr<U> const& b) { return a.get() == b.get(); }
    template<typename T, typename U> inline bool operator!=(uq_ptr<T> const& a, uq_ptr<U> const& b) { return a.get() != b.get(); }
    template<typename T, typename U> inline bool operator==(uq_ptr<T> const& a, U const* b) { return a.get() == b; }
    template<typename T, typename U> inline bool operator!=(uq_ptr<T> const& a, U const* b) { return a.get() != b; }
    template<typename T, typename U> inline bool operator==(T const* a, uq_ptr<U> const& b) { return a == b.get(); }
    template<typename T, typename U> inline bool operator!=(T const* a, uq_ptr<U> const& b) { return a != b.get(); }
    template<typename T> inline bool operator<(uq_ptr<T> const& a, uq_ptr<T> const& b) { return std::less<T*>()(a.get(), b.get()); }
}