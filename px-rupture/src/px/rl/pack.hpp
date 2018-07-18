// name: pack.hpp
// type: c++ template class
// auth: is0urce
// desc: template class for inventory

#pragma once

#include <px/memory/uq_ptr.hpp>
#include <px/memory/memory.hpp>

#include <vector>
#include <utility>

namespace px::rl {

    template <typename Item>
    class pack {
    public:
        using item_type = Item;
        using item_ptr = uq_ptr<item_type>;

    public:
        bool is_empty() const noexcept {
            return items.empty();
        }

        size_t item_count() const noexcept {
            return items.size();
        }

        item_ptr & add(item_ptr item) {
            items.push_back(std::move(item));
            return items.back();
        }

        item_type * get(size_t idx) noexcept {
            return idx >= items.size() ? nullptr : items[idx].get();
        }

        template <size_t Index>
        item * get() noexcept {
            return Index >= items.size() ? nullptr : items[Index].get();
        }

        item_ptr remove(size_t idx) {
            if (idx >= items.size()) return nullptr;
            item_ptr result = std::move(items[idx]);
            items[idx] = std::move(items.back());
            items.pop_back();
            return result;
        }

        void clear_pack() {
            items.clear();
        }

        template <typename Op>
        void unload(Op && fn) {
            for (auto & i : items) {
                fn(std::move(i));
            }
            items.clear();
        }

        template <typename Op>
        void sort(Op && comparator) {
            std::sort(items.begin(), items.end(), std::forward<Op>(comparator));
        }

        template <typename Op>
        void enumerate(Op && fn) {
            for (item_ptr & ptr : items) {
                fn(*ptr);
            }
        }

        template <typename Op>
        void enumerate(Op && fn) const {
            for (item_ptr const& prt : items) {
                fn(*ptr);
            }
        }

        template <typename Archive>
        void serialize(Archive & archive) {
            size_t size = items.size();
            archive(size);
            items.resize(size);
            for (size_t i = 0; i != size; ++i) {
                if (!items[i]) items[i] = make_uq<item_type>();
                item_ptr & r = items[i];
                item_type & it = *r;
                it;
                archive(it);
            }
        }

    public:
        pack() = default;

    private:
        std::vector<uq_ptr<item>> items;
    };
}