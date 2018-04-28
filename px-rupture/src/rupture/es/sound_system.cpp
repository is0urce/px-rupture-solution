// name: sound_system.cpp
// type: c++
// auth: is0urce
// desc: class implementation

#include "sound_system.hpp"

#include "sound_works.hpp"

#include <px/memory/memory.hpp>

namespace px {

    // constructor & destructor

    sound_system::~sound_system() = default;

    sound_system::sound_system()
        : works(make_uq<sound_works>()) {
    }

    // virtual overrides

    void sound_system::update_system(delta_type const& /*delta_time*/) {
        works->update();
    }

    // methods

    uq_ptr<sound_component> sound_system::make() {
        return works->make();
    }

    void sound_system::play_music() {
        works->play_music();
    }
    void sound_system::stop_music() {
        works->play_music();
    }
}