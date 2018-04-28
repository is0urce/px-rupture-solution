// name: transform_works.hpp
// type: c++

#pragma once

#include "transform_component.hpp"

#include <px/common/pool_chain.hpp>
#include <px/common/qtree.hpp>

namespace px {

    class transform_works final {
    public:
        uq_ptr<transform_component> make() {
            auto result = pool.make_uq();
            result->incarnate(&space);
            return result;
        }

        void store() {
            pool.enumerate([](auto & pawn) {
                pawn.store();
            });
        }

    public:
        transform_works() noexcept
            : space(64) {
        }

    private:
        qtree<transform_component*>             space;
        pool_chain<transform_component, 1000>   pool;
    };
}