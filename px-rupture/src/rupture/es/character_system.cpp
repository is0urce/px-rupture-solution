// name: character_system.cpp
// type: c++
// auth: is0urce
// desc: class implementation

#include "character_system.hpp"
#include "character_works.hpp"

#include "../app/document.hpp"
#include "../app/settings.hpp"
#include "../script/script_system.hpp"

#include <px/memory/memory.hpp>

namespace px {

    // constructor & destructor

    character_system::~character_system() = default;

    character_system::character_system()
        : works(make_uq<character_works>()) {
    }

    // virtual overload

    void character_system::turn_update_system(delta_type const& /*delta_time*/) {
        works->turn(1);
    }

    // methods

    uq_ptr<character_component> character_system::make() {
        return works->make();
    }

    void character_system::load_skills(script_system * mashine) {
        auto doc = document::load_document(settings::skills_path);
        auto skill_list = doc["skills"];
        for (auto const& skill_node : skill_list) {
            std::string const script = skill_node.at("script");
            std::string const tag = skill_node.at("tag");

            std::string const name = skill_node.value("name", tag);
            std::string const alias = skill_node.value("alias", name);

            skill::state_type state;
            state.set_tag(tag);
            state.set_name(name);
            state.set_description(skill_node.value("description", ""));
            state.set_alias(alias);
            state.set_cost(skill_node.value("cost", 0));
            state.set_cooldown(skill_node.value("cd", 0));
            state.set_hostile(skill_node.value("hostile", true));
            state.set_duration(skill_node.value("duration", 1));

            works->add(tag, state, mashine->impact(settings::scripts_path + script + ".lua"));
        }
    }
}