// name: craft_task
// type: c++
// auth: is0urce
// desc: template class

#pragma once

// reagents aggregated for crafting

#include <px/memory/uq_ptr.hpp>

#include <algorithm>
#include <vector>

namespace px::rl {

    template <typename Item>
    class craft_task {
    public:
        using item_type = Item;
        using ptr = uq_ptr<item_type>;

    public:
        void close() {
            slots.clear();
        }
        void reset(size_t n) {
            slots.resize(n);
        }
        size_t size() const noexcept {
            return slots.size();
        }

        bool add(ptr reagent) {
            for (size_t idx = 0, size = slots.size(); idx != size; ++idx) {
                if (!slots[idx]) {
                    slots[idx] = std::move(reagent);
                    sort();
                    return true;
                }
            }
            return false;
        }
        ptr remove(size_t idx) {
            ptr result;
            if (idx < slots.size()) {
                result = std::move(slots[idx]);
                slots[idx] = std::move(slots.back());
                sort();
            }
            return result;
        }
        template <size_t Index>
        ptr remove() {
            ptr result;
            if (Index < slots.size()) {
                result = std::move(slots[Index]);
                slots[Index] = std::move(slots.back());
                sort();
            }
            return result;
        }
        ptr remove() {
            return remove<0>();
        }
        bool is_empty() const noexcept {
            for (size_t idx = 0, size = slots.size(); idx != size; ++idx) {
                if (slots[idx]) return false;
            }
            return true;
        }
        bool is_complete() const noexcept {
            for (size_t idx = 0, size = slots.size(); idx != size; ++idx) {
                if (!slots[idx]) return false;
            }
            return true;
        }

        template <typename Operator>
        void enumerate(Operator && function) const {
            for (auto const& ptr : slots) {
                if (ptr) {
                    function(*ptr);
                }
            }
        }

        item_type const* operator[](size_t idx) const {
            return slots[idx].get();
        }

    private:
        // sort out empty slots
        void sort() {
            std::sort(slots.begin(), slots.end(), [](auto const& a, auto const& b) { return !b && a; });
        }

    private:
        std::vector<ptr> slots;
    };
}