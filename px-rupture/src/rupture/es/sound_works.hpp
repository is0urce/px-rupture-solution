// name: sound_works.hpp
// type: c++
// auth: is0urce
// desc: internal sound system

#pragma once

#include "sound_component.hpp"

#include <px/common/pool_chain.hpp>

namespace px {

    class sound_works final {
    public:
        uq_ptr<sound_component> make(/*std::string const&*/ /*name*/) {
            auto result = pool.make_uq();
            return result;
        }

        void update() {
        }

        void play_music() {
        }

        void stop_music() {
        }

    public:
        sound_works() noexcept {
        }

    private:
        pool_chain<sound_component, 10000>     pool;
    };
}