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
        void aquire(item_ptr new_item) {
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

            // sort by name
            sort([](item_ptr const& lh, item_ptr const& rh) {
                return lh->name() < rh->name();
            });
        }
    };
}