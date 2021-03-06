// name: composition.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#pragma once

// composition of components
// has ownership of component collection

#include <px/es/component.hpp>
#include <px/memory/uq_ptr.hpp>

#include <algorithm>
#include <vector>

namespace px {

    class composition {
    public:
        using component_ptr = uq_ptr<component>;

    public:
        component_ptr & add(component_ptr && component) {
            m_components.push_back(std::forward<component_ptr>(component));
            return m_components.back();
        }

        // remove specified component (O=n)
        void remove(component * element) {
            m_components.erase(std::remove_if(std::begin(m_components), std::end(m_components), [&](component_ptr const& ptr) { return ptr.get() == element; }));
        }

        // remove component by type (O=n)
        template <typename C>
        void remove() {
            m_components.erase(std::remove_if(std::begin(m_components), std::end(m_components), [&](auto const& element) { return dynamic_cast<C*>(element.get()); }));
        }

        // remove all components
        void clear() {
            m_components.clear();
        }

        // number of presented components
        size_t size() const noexcept {
            return m_components.size();
        }

        template <typename Operator>
        void enumerate_components(Operator && fn) {
            std::for_each(std::begin(m_components), std::end(m_components), std::forward<Operator>(fn));
        }

        template <typename Operator>
        void enumerate_components(Operator && fn) const {
            std::for_each(std::cbegin(m_components), std::cend(m_components), std::forward<Operator>(fn));
        }

        // activate all components in a collection
        void enable() {
            enumerate_components([](auto & component) { component->activate(); });
        }

        // deactivate all components in a collection
        void disable() {
            enumerate_components([](auto & component) { component->deactivate(); });
        }

        // querry component by type
        template <typename U>
        U * query() const {
            U * result = nullptr;
            for (auto const& c : m_components) {
                result = dynamic_cast<U*>(c.get());
                if (result) break;
            }
            return result;
        }

        void swap(composition & that) noexcept {
            std::swap(m_components, that.m_components);
        }

    public:
        composition() = default;

        composition(composition && that) noexcept
            : composition() {
            swap(that);
            //m_components = std::move(that.m_components);
        }

        composition & operator=(composition && that) noexcept {
            swap(that);
            //m_components = std::move(that.m_components);
            return *this;
        }

        composition(composition const&) = delete;
        composition & operator=(composition const&) = delete;

    private:
        std::vector<component_ptr> m_components;
    };
}