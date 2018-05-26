// name: animator_component.cpp
// type: c++
// auth: is0urce
// desc: class implementation

#include "animator_component.hpp"

#include "../draw/animation.hpp"
#include "../draw/animation_set.hpp"

namespace px {

    // constructor & desctructor

    animator_component::~animator_component() = default;

    animator_component::animator_component() noexcept
        : name(nullptr)
        , playing(false)
        , current_index(0)
        , current_ptr(nullptr) {
    }

    // methods

    void animator_component::select() {
        if (animations) {
            current_ptr = &animations->clips[current_index];
        }
    }

    bool animator_component::play(size_t animation_index) {
        if (animations && animation_index < animations->clips.size()) {
                current_index = animation_index;
                current_ptr = &animations->clips[current_index];
                playing = true;
                return true;
            }
        return false;
    }

    bool animator_component::play(std::string const& animation_name) {
        if (animations) {
            auto it = animations->names.find(animation_name);
            if (it != animations->names.end()) {
                current_index = it->second;
                current_ptr = &animations->clips[current_index];
                playing = true;
                return true;
            }
        }
        return false;
    }

    void animator_component::stop() noexcept {
        playing = false;
    }

    void animator_component::resume() noexcept {
        playing = true;
    }

    bool animator_component::is_playing() const noexcept {
        return playing;
    }

    char const * const animator_component::get_id() const noexcept {
        return name;
    }

    void animator_component::set_id(char const * const id) noexcept {
        name = id;
    }

    void animator_component::clear() {
        animations = nullptr;
        current_index = 0;
        current_ptr = nullptr;
        stop();
    }

    void animator_component::assign(animation_set const* ptr) noexcept {
        animations = ptr;
    }

    animation const* animator_component::current() const noexcept {
        return current_ptr;
    }

    size_t animator_component::size() const noexcept {
        return animations ? animations->clips.size() : 0;
    }
}