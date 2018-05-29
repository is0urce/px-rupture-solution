// name: transform_works.hpp
// type: c++

#pragma once

#include "transform_component.hpp"

#include <px/es/pool_manager.hpp>
#include <px/common/qtree.hpp>

namespace px {

    class transform_works final
        : public pool_manager<transform_works, transform_component, 1024> {
    public:
        uq_ptr<transform_component> setup(uq_ptr<transform_component> element) {
            element->incarnate(&space);
            return std::move(element);
        }

        void store() {
            objects.enumerate([](auto & pawn) {
                pawn.store();
            });
        }

    public:
        transform_works() noexcept
            : space(64) {
        }

    private:
        qtree<transform_component*>             space;
    };
}