// name: facility_line.hpp

#include <px/memory/memory.hpp>

#include <utility>

namespace px {

    template <typename Product, typename Line>
    class factory_line
        : public Line
    {
    public:
        template <typename... Args>
        uq_ptr<Product> create_product(Args && ...args) {
            return make(std::forward<Args>(args)...);
        }
    };

    template <typename CR>
    class factory_dispatcher abstract
    {
    public:
        template <typename Line, typename Product, typename ...Args>
        uq_ptr<Product> produce(Args && ...args) {
            return static_cast<factory_line<Product, Line>&>(static_cast<CR&>(*this)).create_product(std::forward<Args>(args)...);
        }

        template <typename F>
        F & get_factory() noexcept {
            return static_cast<F&>(static_cast<CR&>(*this));
        }

        template <typename F>
        F const& get_factory() const noexcept {
            return static_cast<F const&>(static_cast<CR const&>(*this));
        }
    };
}