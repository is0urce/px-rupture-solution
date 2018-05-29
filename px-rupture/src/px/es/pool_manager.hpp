// name: pool_manager.hpp
// type: c++

#pragma once

#include <px/common/pool_chain.hpp>

#include <utility>

namespace px {

    template <typename Implementation, typename Element, size_t Size>
    class pool_manager {
    public:
        using element_type = Element;

    public:
        template <typename ... Args>
        uq_ptr<element_type> make(Args ... args) {
            return static_cast<Implementation&>(*this).setup(objects.make_uq(), std::forward<Args>(args)...);
        }

    protected:
        uq_ptr<element_type> setup(uq_ptr<element_type> obj) {
            return std::move(obj);
        }

    protected:
        pool_chain<element_type, Size> objects;
    };
}