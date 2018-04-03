// name: character_works.hpp
// type: c++

#pragma once

#include "character_component.hpp"

#include "../rl/skill.hpp"

#include <px/common/pool_chain.hpp>
#include <px/rl/skill/skill_functional.hpp>

#include <map>
#include <string>
#include <tuple>

namespace px {

    class character_works {
    public:
        uq_ptr<character_component> make() {
            auto result = pool.make_uq();
            result->assign_book(&book);
            return result;

        }

        void turn(int time_span) {
            pool.enumerate([&](character_component & character) {
                character.reduce_cooldown(time_span);
            });
        }

        void add(std::string const& tag, skill::state_type const& start, rl::skill_functional<body_component *, body_component *, point2> && impact) {
            auto & fn = lib[tag] = std::move(impact);
            book[tag] = { start, &fn };
        }

    private:
        std::map<std::string, rl::skill_functional<body_component *, body_component *, point2>> lib;
        std::map<std::string, std::tuple<skill::state_type, skill::impact_type * >> book;
        pool_chain<character_component, 1024> pool;
    };
}