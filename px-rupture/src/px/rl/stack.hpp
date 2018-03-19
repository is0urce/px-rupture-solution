// name: stack.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#pragma once

// base item class with operations for staking

namespace px::rl {

    template <typename UInteger>
    class stack {
    public:
        using stack_type = UInteger;

    public:
        // returns number of items failed to stack (due max stack size restrictions)
        // zero indicates everything is stacked
        stack_type give(stack & source) {
            return give(source, source.m_stack);
        }

        stack_type give(stack & source, stack_type n) {
            if (&source == this) return m_stack; // stacking onto same item has no point

            if (source.m_stack < n) n = source.m_stack;

            stack_type total = m_stack + n;

            if (m_max_stack == 0 || total <= m_max_stack) {
                m_stack = total;
                source.m_stack = 0;
            }
            else {
                m_stack = m_max_stack;
                source.m_stack = total - m_stack;
            }
            return source.m_stack;
        }

        stack_type increase(stack_type n) {
            UInteger total = m_stack + n;
            return m_stack = (total < m_max_stack || m_max_stack == 0) ? total : m_max_stack;
        }

        stack_type decrease(stack_type n) {
            return m_stack -= (n < m_stack) ? n : m_stack;
        }

        stack_type count() const noexcept {
            return m_stack;
        }

        stack_type maximum() const noexcept {
            return m_max_stack;
        }

        bool is_full() const noexcept {
            return m_stack == m_max_stack;
        }

        bool is_last() const noexcept {
            return m_stack == 1;
        }

        bool is_single() const noexcept {
            return m_max_stack == 1;
        }

        bool is_stacking() const noexcept {
            return m_max_stack != 1;
        }

        bool is_valid() const noexcept {
            return m_stack != 0;
        }

        void set_current_stack(stack_type n) noexcept {
            m_stack = n;
        }

        // size - maximum size of a stack, use 0 for unlimited
        void set_maximum_stack(stack_type n) noexcept {
            m_max_stack = n;
        }

        // unlimited max size
        void make_stacking() noexcept {
            m_max_stack = 0;
        }

        // disallow stacking
        void make_single() noexcept {
            m_max_stack = 1;
        }

        template <typename Archive>
        void serialize(Archive & archive) {
            archive(m_stack, m_max_stack);
        }

    public:
        stack() noexcept
            : m_stack(1)
            , m_max_stack(1)
        {
        }
        stack(UInteger stack, UInteger max) noexcept
            : m_stack(stack)
            , m_max_stack(max)
        {
        }

    private:
        stack_type m_stack;
        stack_type m_max_stack; // default is 1, if max_stack is 0, allow unlimited stacking
    };
}