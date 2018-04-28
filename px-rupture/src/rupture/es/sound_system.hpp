// name: sound_system.hpp
// type: c++
// auth: is0urce
// desc: class declaration

#pragma once

#include <px/es/system.hpp>
#include <px/es/delta.hpp>

#include <px/memory/uq_ptr.hpp>

namespace px {

    class sound_component;
    class sound_works;

    class sound_system
        : public system<delta>
    {
    public:
        uq_ptr<sound_component> make();
        void                    play_music();
        void                    stop_music();

    public:
        virtual                 ~sound_system() override;
        sound_system();

    protected:
        virtual void            update_system(delta_type const& delta_time) override;

    private:
        uq_ptr<sound_works>     works;
    };
}