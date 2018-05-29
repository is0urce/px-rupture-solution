// name: sound_works.hpp
// type: c++
// auth: is0urce
// desc: internal sound system

#pragma once

#include "sound_component.hpp"
#include "transform_component.hpp"

#include <px/dev/assert.hpp>
#include <px/common/pool_chain.hpp>
#include <px/common/vector.hpp>

#include <fmod/fmod.hpp>
#include <fmod/fmod_errors.h>

#include <cmath>
#include <map>
#include <string>
#include <stdexcept>
#include <vector>

namespace px {

    class sound_works final {
    public:
        using sound_map = std::map<std::string, FMOD::Sound*>;
        using channel_map = std::map<int, FMOD::Channel*>;

    public:
        void target(transform_component const* pan) noexcept {
            camera = pan;
        }

        uq_ptr<sound_component> make(/*std::string const&*/ /*name*/) {
            auto result = pool.make_uq();
            return result;
        }

        void update() {
            std::vector<channel_map::iterator> releasing;
            for (auto it = channels.begin(), end = channels.end(); it != end; ++it) {
                bool is_playing = false;
                it->second->isPlaying(&is_playing);
                if (!is_playing) {
                    releasing.push_back(it);
                }
            }
            for (auto it : releasing) {
                channels.erase(it);
            }

            FMOD_VECTOR const position = to_fmod(camera ? camera->position() : vector2{0, 0});
            static FMOD_VECTOR const velocity{ 0, 0, 0 };
            static FMOD_VECTOR const forward{ 0, 0, 1 };
            static FMOD_VECTOR const up{ 0, 1, 0 };
            system->set3DListenerAttributes(0, &position, &velocity, &forward, &up);

            system->update();
        }

        void play_music() {
        }

        void stop_music() {
        }

        void set_master_volume(double volume) {
            if (master) {
                master->setVolume(static_cast<float>(volume));
            }
        }

        void set_volume(sound_channel group, double volume) {
            switch (group) {
            case sound_channel::master:
                if (master) {
                    master->setVolume(static_cast<float>(volume));
                }
                break;
            }
        }

        // returns channel_id
        unsigned int play_sound(std::string const& name, double volume, vector2 const& location) {
            return play_sound(name, volume, location, false);
        }

        unsigned int play_sound(std::string const& name, double volume) {
            return play_sound(name, volume, { 0, 0 }, true);
        }

        void load_sound(std::string const& name, bool positioned, bool looped, bool streamed) {
            auto it = sounds.find(name);
            if (it == sounds.end()) {
                FMOD::Sound * sound = nullptr;
                FMOD_MODE mode = FMOD_DEFAULT;
                mode |= (positioned ? FMOD_2D : FMOD_3D);
                mode |= (looped ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
                mode |= (streamed ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE);
                FMOD_RESULT result = system->createSound(name.c_str(), mode, nullptr, &sound);
                
                if (result != FMOD_OK) {
                    px_debug(std::string("px::sound_works::load_sound - cannot load sound, error=") + std::string(FMOD_ErrorString(result)));
                }

                if (sound) {
                    sounds[name] = sound;
                }
            }
        }

        void unload_sound(std::string const& name) {
            auto it = sounds.find(name);
            if (it != sounds.end()) {
                it->second->release();
                sounds.erase(it);
            }
        }

    public:
        ~sound_works() {
            clear_sounds();
            system->release();
        }

        sound_works()
            : system(nullptr)
            , master(nullptr)
            , channel_id(0)
            , camera(nullptr) {
            init();
            //load_sound("data/snd/a.mp3", false, true, true);
            //set_master_volume(0.1);
            //play_sound("data/snd/a.mp3", { 0, 0 }, 0.1);
        }

    private:
        void init() {
            FMOD_RESULT result = FMOD::System_Create(&system);
            if (result != FMOD_OK) {
                throw std::runtime_error(std::string("px::sound_works::ctor() - cannot create sound system, error=") + std::string(FMOD_ErrorString(result)));
            }
            result = system->init(64, FMOD_INIT_NORMAL, nullptr);
            if (result != FMOD_OK) {
                throw std::runtime_error(std::string("px::sound_works::ctor() - cannot init sound system, error=") + std::string(FMOD_ErrorString(result)));
            }

            result = system->createChannelGroup("master", &master);
            if (result != FMOD_OK) {
                throw std::runtime_error(std::string("px::sound_works::ctor() - cannot init sound group, error=") + std::string(FMOD_ErrorString(result)));
            }
        }

        void clear_sounds() {
            for (auto & sound : sounds) {
                sound.second->release();
            }
            sounds.clear();
        }

        // returns channel_id
        unsigned int play_sound(std::string const& name, double volume, vector2 const& location, bool relative) {
            unsigned int const id = ++channel_id;

            FMOD::Channel * channel = nullptr;
            if (auto snd = acquire_sound(name, true, false, false)) {
                system->playSound(snd, master, true, &channel);
            }
            else {
                px_debug("px::sound_works::play_sound(..) - no sound by name=" + name);
            }

            if (channel) {
                FMOD_VECTOR const position = to_fmod(location);
                static FMOD_VECTOR const velocity{ 0, 0, 0 };
                channel->set3DAttributes(&position, &velocity);
                channel->setVolume(static_cast<float>(volume));
                channel->setPaused(false);
                if (relative) {
                    channel->setMode(FMOD_3D_HEADRELATIVE);
                }
                channels[id] = channel;
            }

            return id;
        }

        FMOD::Sound * acquire_sound(std::string const& name, bool positioned, bool looped, bool streamed) {
            auto it = sounds.find(name);
            if (it == sounds.end()) {
                load_sound(name, positioned, looped, streamed);
                it = sounds.find(name);
            }
            return it == sounds.end() ? nullptr : it->second;
        }

        // +X = right, +Y = up, +Z = forward.
        static FMOD_VECTOR to_fmod(vector2 const& location) {
            FMOD_VECTOR result;
            result.x = static_cast<float>(location.x());
            result.y = static_cast<float>(location.y());
            result.z = 0;
            return result;
        }

        static double volume_to_db(double volume) {
            return 20 * std::log10(volume);
        }

        static double db_to_volume(double db) {
            return std::pow(10, db * 0.05);
        }

    private:
        FMOD::System *                      system;
        FMOD::ChannelGroup *                master;
        sound_map                           sounds;
        channel_map                         channels;
        unsigned int                        channel_id;
        pool_chain<sound_component, 10000>  pool;
        transform_component const*          camera;
    };
}