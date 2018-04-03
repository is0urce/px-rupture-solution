// name: character_component.hpp
// type: c++
// auth: is0urce
// desc: class

#pragma once

#include <px/es/component.hpp>
#include <px/es/link_dispatcher.hpp>

#include "../rl/skill.hpp"
#include <px/rl/skill/skill_set.hpp>
#include <px/rl/specialty.hpp>

namespace px {

    class character_component
        : public component
        , public link_dispatcher<character_component>
        , public rl::skill_set<skill>
        , public specialty
    {
    public:
        template <typename Archive>
        void serialize(Archive & archive) {
            archive(static_cast<rl::skill_set<skill> &>(*this));
            archive(static_cast<specialty&>(*this));
        }

    public:
        virtual ~character_component() override = default;
        character_component() = default;
        character_component(character_component const&) = delete;
        character_component const& operator=(character_component const&) = delete;
    };
}