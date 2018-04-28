// name: pool_chain.hpp
// type: c++ header
// desc: template class
// auth: is0urce

#pragma once

// pool chain is a chain of pools
// if root pool run out, new pools requested from heap

#include <px/common/pool.hpp>

#include <memory>

namespace px {

    template <typename T, size_t Size>
    class pool_chain {
    public:
        using element_type = T;
        using pointer = T * ;
        using pool_type = pool<T, Size>;
        using uq_ptr = typename pool_type::uq_ptr;

    public:
        class node {
        public:
            pool_type               chunk;
            std::unique_ptr<node>   next;
        };

    public:
        size_t size() const noexcept {
            size_t result = 0;
            for (node const* i = &m_root; i != nullptr; i = i->next.get()) {
                result += i->chunk.size();
            }
            return result;
        }

        bool is_empty() const noexcept {
            for (node const* i = &m_root; i != nullptr; i = i->next.get()) {
                if (!i->chunk.empty()) return false;
            }
            return true;
        }

        // exposed for debug / performance queries, always has at least 1
        size_t chunks() const noexcept {
            return m_chunks;
        }

        bool contains(T const* ptr) const noexcept {
            for (node const* i = &m_root; i != nullptr; i = i->next.get()) {
                if (i->chunk.can_contain(ptr)) {
                    return i->chunk.contains(ptr);
                }
            }
            return false;
        }

        template <typename... Args>
        T * request(Args ...args) {
            return aquire_free_pool().request(std::forward<Args>(args)...);
        }

        // it's safe to release already released objects
        void release(T * ptr) {
            for (node * i = &m_root; i != nullptr; i = i->next.get()) {
                if (i->chunk.can_contain(ptr)) {
                    i->chunk.release(ptr);
                    return;
                }
            }
        }

        template <typename... Args>
        uq_ptr make_uq(Args ... args) {
            return acquire_free_pool().make_uq(std::forward<Args>(args)...);
        }

        template <typename UnaryFunction>
        void enumerate(UnaryFunction && op) {
            for (node* i = &m_root; i != nullptr; i = i->next.get()) {
                i->chunk.enumerate(op);
            }
        }

        template <typename UnaryFunction>
        void enumerate(UnaryFunction && op) const {
            for (node const* i = &m_root; i != nullptr; i = i->next.get()) {
                i->chunk.enumerate(op);
            }
        }

        void clear() {
            m_root.chunk.clear();
            m_root.next.reset();
            m_chunks = 1;
        }

        // remove unused chunks
        // don't actualy make pool chain ''optimal''
        // just frees currently unused extra memory
        // chain always has at least one root chunk after invoke even it's empty
        void optimize() noexcept {
            for (std::unique_ptr<node> * i = &m_root.next; *i; i = &(*i)->next) {
                if ((*i)->chunk.size() == 0) {
                    std::swap(*i, (*i)->next);
                    --m_chunks;
                }
            }
        }

    public:
        pool_chain() noexcept
            : m_chunks(1) {
        }

        pool_chain(pool_chain const&) = delete;
        pool_chain & operator=(pool_chain const&) = delete;

    private:
        pool_type & acquire_free_pool() {
            for (node * i = &m_root; true; i = i->next.get()) {
                if (!i->chunk.full()) {
                    return i->chunk;
                }
                if (i->next == nullptr) {
                    i->next = std::make_unique<node>();
                    ++m_chunks;
                }
            }
        }


    private:
        node    m_root;
        size_t  m_chunks; // current number of chunks
    };
}