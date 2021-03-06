// name: animator_component.hpp
// type: c++ header
// auth: is0urce
// type: class declaration

#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>

#include <string>

namespace px {

    struct animation;
    struct animation_set;
    class sprite_component;

    class animator_component
        : public component
        , public link<sprite_component>
        , public link_dispatcher<animator_component>
    {
    public:
        void                            assign(animation_set const* ptr) noexcept;
        bool                            play(size_t animation_index);
        bool                            play(std::string const& animation_name);
        void                            stop() noexcept;
        void                            resume() noexcept;
        bool                            is_playing() const noexcept;
        char const* const               get_id() const noexcept;
        void                            set_id(char const * const) noexcept;
        void                            clear();
        animation const*                current() const noexcept;
        size_t                          size() const noexcept;

        template <typename Archive>
        void                            serialize(Archive & archive) {
            archive(current_index, playing);
            select();
        }

    public:
        virtual	                        ~animator_component() override;
        animator_component() noexcept;
        animator_component(animator_component const&) = delete;
        animator_component & operator=(animator_component const&) = delete;

    private:
        void                            select();

    private:
        char const*                     name;           // label
        animation_set const*            animations;
        animation const*                current_ptr;
        size_t                          current_index;
        bool                            playing;
    };
}