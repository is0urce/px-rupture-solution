// name: inventory.hpp

#pragma once

#include "item.hpp"

#include <px/rl/pack.hpp>

#include <vector>

namespace px::rl {

    class inventory
        : public pack<item>
    {
    public:
        void acquire(item_ptr new_item) {
            // try stack
            enumerate([&](item & current) {
                if (new_item) {
                    if (current.do_stack(*new_item)) {
                        new_item.reset();
                    }
                }
            });

            // add rest
            if (new_item) {
                add(std::move(new_item));
            }

            arrange(); // sort by name
        }

        item_ptr unacquire(size_t idx, item::stack_type n) {
            item_ptr result;
            if (item * current = get(idx)) {
                const auto count = current->count();
                if (count >= n) {
                    if (count == n) {
                        result = remove(idx);
                        arrange(); // item removed, sort
                    }
                    else {
                        result = make_uq<item>(*current); // copy constructor
                        result->set_current_stack(n);
                        current->decrease(n);
                    }
                }
            }
            return result;
        }

        void give(pack & loot) {
            loot.unload([&](item_ptr && item) {
                acquire(std::move(item));
            });
        }

        template <typename Op>
        void give(pack & loot, Op && msg) {
            loot.unload([&](item_ptr && item) {
                msg(*item);
                acquire(std::move(item));
            });
        }

        // sort by name
        void arrange() {
            sort([](item_ptr const& lh, item_ptr const& rh) {
                return lh->name() < rh->name();
            });
        }
    };
}