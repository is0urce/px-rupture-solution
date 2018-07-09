// name: sound_system.hpp
// type: c++
// auth: is0urce
// desc: class declaration

#pragma once

#include <px/es/system.hpp>
#include <px/es/delta.hpp>

#include <px/common/vector.hpp>
#include <px/memory/uq_ptr.hpp>
#include <px/snd/sound_channel.hpp>

#include <string>

namespace px {

    class sound_component;
    class sound_works;
    class transform_component;

    class sound_system
        : public system<delta>
    {
    public:
        //uq_ptr<sound_component> make();
        unsigned int            play_music(std::string const& name, double volume);
        unsigned int            play_sound(std::string const& name, double volume, vector2 const& location);
        unsigned int            play_sound(std::string const& name, double volume);
        void                    set_master_volume(double volume);
        void                    set_volume(sound_channel channel, double volume);
        void                    stop_music();
        void                    target(transform_component const* camera) noexcept;

    public:
        virtual                 ~sound_system() override;
        sound_system();

    protected:
        virtual void            update_system(delta_type const& delta_time) override;

    private:
        uq_ptr<sound_works>     works;
    };
}