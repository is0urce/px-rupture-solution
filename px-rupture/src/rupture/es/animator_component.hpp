// name: animator_component.hpp
// type: c++ header
// auth: is0urce
// type: class declaration

#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>

#include <string>
#include <vector>

namespace px {

    class animation;
    class sprite_component;

    class animator_component
        : public component
        , public link<sprite_component>
        , public link_dispatcher<animator_component>
    {
    public:
        bool                            play(size_t animation_id);
        void                            stop();
        void                            resume();
        bool                            is_playing() const noexcept;
        char const* const               get_id() const noexcept;
        void                            set_id(char const * const) noexcept;
        void                            clear();
        void                            push_animation(animation const* ptr);
        animation const*                current() const;
        size_t                          size() const;

        template <typename Archive>
        void                            serialize(Archive & /*archive*/) {
        }

    public:
        virtual	                        ~animator_component() override;
        animator_component() noexcept;
        animator_component(animator_component const&) = delete;
        animator_component & operator=(animator_component const&) = delete;

    private:
        std::vector<animation const*>   animations;
        bool                            playing;
        size_t                          current_index;
        char const*                     name;
    };
}