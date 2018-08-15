// name: factory_dispatcher.hpp

#include <px/memory/memory.hpp>

#include <utility>

namespace px {

    template <typename CR>
    class factory_dispatcher abstract
    {
    public:
        template <typename Line, typename ...Args>
        auto product(Args && ...args) {
            return static_cast<Line&>(static_cast<CR&>(*this)).make(std::forward<Args>(args)...);
        }

        template <typename Factory>
        Factory & get_factory() noexcept {
            return static_cast<Factory&>(static_cast<CR&>(*this));
        }

        template <typename Factory>
        Factory const& get_factory() const noexcept {
            return static_cast<Factory const&>(static_cast<CR const&>(*this));
        }
    };
}