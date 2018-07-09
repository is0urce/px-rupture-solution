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

        void update() {
            if (system) {
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

                FMOD_VECTOR const position = to_fmod(camera ? camera->position() : vector2(0, 0));
                static FMOD_VECTOR const velocity{ 0, 0, 0 };
                static FMOD_VECTOR const forward{ 0, 0, 1 };
                static FMOD_VECTOR const up{ 0, 1, 0 };
                system->set3DListenerAttributes(0, &position, &velocity, &forward, &up);
                system->update();
            }
        }

        // returns channel_id
        unsigned int play_sound(std::string const& name, double volume, vector2 const& location) {
            return play(load_sound(name, true, false, false), sfx, volume, location, false);
        }

        unsigned int play_sound(std::string const& name, double volume) {
            return play(load_sound(name, true, false, false), sfx, volume, { 0, 0 }, true);
        }

        unsigned int play_music(std::string const& name, double volume) {
            return play(load_sound(name, false, false, true), music, volume, { 0, 0 }, true);
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
            case sound_channel::music:
                if (music) {
                    music->setVolume(static_cast<float>(volume));
                }
                break;
            case sound_channel::sfx:
                if (sfx) {
                    sfx->setVolume(static_cast<float>(volume));
                }
                break;
            default:
                px_debug("invalid sound channel group");
                break;
            }
        }

        FMOD::Sound * load_sound(std::string const& name, bool positioned, bool looped, bool streamed) {
            if (system) {
                auto it = sounds.find(name);
                if (it == sounds.end()) {
                    FMOD::Sound * sound(nullptr);
                    FMOD_MODE const mode = FMOD_DEFAULT
                        | (positioned ? FMOD_2D : FMOD_3D)
                        | (looped ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF)
                        | (streamed ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE);
                    FMOD_RESULT const result_code = system->createSound(name.c_str(), mode, nullptr, &sound);

                    if (result_code != FMOD_OK) {
                        px_debug(std::string("px::sound_works::load_sound - cannot load sound, error=") + std::string(FMOD_ErrorString(result_code)));
                    }

                    if (sound) {
                        sounds[name] = sound;
                    }
                    return sound;
                }
                else {
                    return it->second;
                }
            }
            return nullptr;
        }

        bool release_sound(std::string const& name) {
            auto it = sounds.find(name);
            if (it != sounds.end()) {
                it->second->release();
                sounds.erase(it);
                return true;
            }
            return false;
        }

    public:
        ~sound_works() {
            release();
        }

        sound_works()
            : system(nullptr)
            , master(nullptr)
            , channel_id(0)
            , camera(nullptr) {
            init();
        }

    private:
        ///
        /// release system object memory
        ///
        void release() {
            clear_sounds();
            if (system) {
                auto result_code = system->release();
                if (result_code != FMOD_OK) {
                    px_debug(std::string("px::sound_works::release() - error while releasing sound system, error=") + std::string(FMOD_ErrorString(result_code)));
                }
                system = nullptr;
            }
        }

        bool init() {
            FMOD_RESULT result_code = FMOD::System_Create(&system);
            if (result_code != FMOD_OK) {
                px_debug(std::string("px::sound_works::init() - cannot create sound system, error=") + std::string(FMOD_ErrorString(result_code)));
                return false;
            }

            result_code = system->init(64, FMOD_INIT_NORMAL, nullptr);
            if (result_code != FMOD_OK) {
                px_debug(std::string("px::sound_works::init() - cannot init sound system, error=") + std::string(FMOD_ErrorString(result_code)));
                release();
                return false;
            }

            // mixing groups
            create_group("master", &master);
            create_group("music", &music);
            create_group("sfx", &sfx);

            // groups hierarchy
            if (master) {
                master->addGroup(music);
                master->addGroup(sfx);
            }

            return true;
        }

        void clear_sounds() {
            for (auto & sound : sounds) {
                sound.second->release();
            }
            sounds.clear();
        }

        // returns channel_id
        unsigned int play(FMOD::Sound * sound, FMOD::ChannelGroup * group, double volume, vector2 const& location, bool head_relative) {
            unsigned int const id = ++channel_id;
            if (system && sound) {
                FMOD::Channel * channel(nullptr);
                FMOD_RESULT result_code = system->playSound(sound, group, true, &channel);
                if (result_code != FMOD_OK) {
                    px_debug("px::sound_works::play() - cannot play sound" + std::string(FMOD_ErrorString(result_code)));
                }

                if (channel) {
                    FMOD_VECTOR const position = to_fmod(location);
                    FMOD_VECTOR const velocity{ 0, 0, 0 };
                    channel->set3DAttributes(&position, &velocity);
                    channel->setVolume(static_cast<float>(volume));
                    channel->setPaused(false);
                    channel->setMode(head_relative ? FMOD_3D_HEADRELATIVE : FMOD_3D_WORLDRELATIVE);
                    channels[id] = channel;
                }
            }
            return id;
        }

        ///
        /// create mixing group
        ///
        bool create_group(char const* name, FMOD::ChannelGroup ** group) {
            if (system) {
                FMOD_RESULT const result = system->createChannelGroup(name, group);
                if (result != FMOD_OK) {
                    throw std::runtime_error(std::string("px::sound_works::create_group() - cannot init sound group name=") + std::string(name) + ", error=" + std::string(FMOD_ErrorString(result)));
                }
                return true;
            }
            return false;
        }

        ///
        /// Converts px::vector2 to FMOD_VECTOR
        ///
        static constexpr FMOD_VECTOR to_fmod(vector2 const& location) {
            return { static_cast<float>(location.x()), static_cast<float>(location.y()), 0 };
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
        FMOD::ChannelGroup *                music;
        FMOD::ChannelGroup *                sfx;
        transform_component const*          camera;
        sound_map                           sounds;
        channel_map                         channels;
        unsigned int                        channel_id;
    };
}