// name: character_works.hpp
// type: c++

#pragma once

#include "character_component.hpp"

#include "../rl/skill.hpp"

#include <px/es/pool_manager.hpp>
#include <px/rl/skill/skill_functional.hpp>

#include <map>
#include <string>
#include <tuple>
#include <utility>

namespace px {

    class character_works
        : public pool_manager<character_works, character_component, 1024> {
    public:
        uq_ptr<character_component> setup(uq_ptr<character_component> element) {
            element->assign_book(&book);
            return std::move(element);
        }

        void turn(int time_span) {
            objects.enumerate([&](character_component & element) {
                element.reduce_cooldown(time_span);
            });
        }

        void add(std::string const& tag, skill::state_type const& start, rl::skill_functional<body_component *, body_component *, point2> && impact) {
            auto & fn = lib[tag] = std::move(impact);
            book[tag] = { start, &fn };
        }

    private:
        std::map<std::string, rl::skill_functional<body_component *, body_component *, point2>> lib;
        std::map<std::string, std::tuple<skill::state_type, skill::impact_type * >> book;
    };
}